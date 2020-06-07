#include "Framework.h"
#include "FFT.h"


FFT::FFT()
{
	shader = new Shader(L"50_FFT.fxo");
}


FFT::FFT(Shader* shader)
	:shader(shader)
{
}



FFT::~FFT()
{
	Destroy_Plan();
}

void FFT::Radix008A(ID3D11UnorderedAccessView* pUAV_Dst,
	ID3D11ShaderResourceView* pSRV_Src,
	UINT thread_count,
	UINT istride)
{
	// Setup execution configuration
	UINT grid = thread_count / COHERENCY_GRANULARITY;

	// Buffers
	shader->AsSRV("FFT_SrcData")->SetResource(pSRV_Src);

	shader->AsUAV("FFT_DstData")->SetUnorderedAccessView(pUAV_Dst);

	// Shader
	// Execute
	if (istride > 1)
		shader->Dispatch(0, passCS, grid, 1, 1);
	else
		shader->Dispatch(0, passCS2, grid, 1, 1);
}

void FFT::Create_Plan(UINT slices)
{
	this->slices = slices;
	for (UINT i = 0; i < 6; i++)
	{
		renderSrvs[i] = new Render2D();
		renderSrvs[i]->GetTransform()->Position(D3D::Width() - 100 * i - 60, 260, 0);
		renderSrvs[i]->GetTransform()->Scale(100, 100, 1);
	}
	
	// Constants
	// Create 6 cbuffers for 512x512 transform
	CreateCBuffers_512x512(slices);

	// Temp buffer
	D3D11_BUFFER_DESC buf_desc;
	buf_desc.ByteWidth = sizeof(float) * 2 * (512 * slices) * 512;
	buf_desc.Usage = D3D11_USAGE_DEFAULT;
	buf_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	buf_desc.CPUAccessFlags = 0;
	buf_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	buf_desc.StructureByteStride = sizeof(float) * 2;

	Check(D3D::GetDevice()->CreateBuffer(&buf_desc, NULL, &tmpBuffer));
	

	// Temp undordered access view
	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_UNKNOWN;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav_desc.Buffer.FirstElement = 0;
	uav_desc.Buffer.NumElements = (512 * slices) * 512;
	uav_desc.Buffer.Flags = 0;

	Check(D3D::GetDevice()->CreateUnorderedAccessView(tmpBuffer, &uav_desc, &tmpUAV));
	

	// Temp shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv_desc.Buffer.FirstElement = 0;
	srv_desc.Buffer.NumElements = (512 * slices) * 512;

	Check(D3D::GetDevice()->CreateShaderResourceView(tmpBuffer, &srv_desc, &tmpSRV));
	

	textSB = new StructuredBuffer(NULL, sizeof(float) * 2, (512 * slices) * 512);
}

void FFT::Destroy_Plan()
{
	SafeDelete(shader);
	SafeRelease(tmpSRV);
	SafeRelease(tmpUAV);
	SafeRelease(tmpBuffer);

	for (int i = 0; i < 6; i++)
		SafeDelete(RadixCBuffers[i]);
}

void FFT::CreateCBuffers_512x512(UINT slices)
{
	// Create 6 cbuffers for 512x512 transform.
	// 512*512 = 8^6
	// Buffer 0
	const UINT thread_count = slices * (512 * 512) / 8;
	UINT ostride = 512 * 512 / 8;
	UINT istride = ostride;
	double phase_base = -TWO_PI / (512.0 * 512.0);

	D3D11_BUFFER_DESC cb_desc;
	cb_desc.Usage = D3D11_USAGE_IMMUTABLE;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.CPUAccessFlags = 0;
	cb_desc.MiscFlags = 0;
	cb_desc.ByteWidth = 32;//sizeof(float) * 5;
	cb_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA cb_data;
	cb_data.SysMemPitch = 0;
	cb_data.SysMemSlicePitch = 0;


	CB_Structure cb_data_buf0 = { thread_count, ostride, istride, 512, (float)phase_base };
	cb_data.pSysMem = &cb_data_buf0;

	Check(D3D::GetDevice()->CreateBuffer(&cb_desc, &cb_data, &RadixCBuffers[0]));
	

	// Buffer 1
	istride /= 8;
	phase_base *= 8.0;

	CB_Structure cb_data_buf1 = { thread_count, ostride, istride, 512, (float)phase_base };
	cb_data.pSysMem = &cb_data_buf1;

	Check(D3D::GetDevice()->CreateBuffer(&cb_desc, &cb_data, &RadixCBuffers[1]));

	// Buffer 2
	istride /= 8;
	phase_base *= 8.0;

	CB_Structure cb_data_buf2 = { thread_count, ostride, istride, 512, (float)phase_base };
	cb_data.pSysMem = &cb_data_buf2;

	Check(D3D::GetDevice()->CreateBuffer(&cb_desc, &cb_data, &RadixCBuffers[2]));

	// Buffer 3
	istride /= 8;
	phase_base *= 8.0;
	ostride /= 512;

	CB_Structure cb_data_buf3 = { thread_count, ostride, istride, 1, (float)phase_base };
	cb_data.pSysMem = &cb_data_buf3;

	Check(D3D::GetDevice()->CreateBuffer(&cb_desc, &cb_data, &RadixCBuffers[3]));

	// Buffer 4
	istride /= 8;
	phase_base *= 8.0;

	CB_Structure cb_data_buf4 = { thread_count, ostride, istride, 1, (float)phase_base };
	cb_data.pSysMem = &cb_data_buf4;

	Check(D3D::GetDevice()->CreateBuffer(&cb_desc, &cb_data, &RadixCBuffers[4]));

	// Buffer 5
	istride /= 8;
	phase_base *= 8.0;

	CB_Structure cb_data_buf5 = { thread_count, ostride, istride, 1, (float)phase_base };
	cb_data.pSysMem = &cb_data_buf5;

	Check(D3D::GetDevice()->CreateBuffer(&cb_desc, &cb_data, &RadixCBuffers[5]));
}

void FFT::fft_512x512_c2c(ID3D11UnorderedAccessView * pUAV_Dst, ID3D11ShaderResourceView * pSRV_Dst, ID3D11ShaderResourceView * pSRV_Src)
{
	const UINT thread_count = slices * (512 * 512) / 8;
	UINT istride = 512 * 512 / 8;
	
	shader->AsConstantBuffer("CB_ChangePerCall")->SetConstantBuffer(RadixCBuffers[0]);
	Radix008A(tmpUAV, pSRV_Src, thread_count, istride);
	//Radix008A(textSB->UAV(), pSRV_Src, thread_count, istride);
	//SaveFile(0);
	renderSrvs[0]->SRV(pSRV_Src);

	istride /= 8;
	shader->AsConstantBuffer("CB_ChangePerCall")->SetConstantBuffer(RadixCBuffers[1]);
	Radix008A(pUAV_Dst, tmpSRV, thread_count, istride);
	//Radix008A(pUAV_Dst, textSB->SRV(), thread_count, istride);
	//SaveFile(1);
	renderSrvs[1]->SRV(tmpSRV);

	istride /= 8;
	shader->AsConstantBuffer("CB_ChangePerCall")->SetConstantBuffer(RadixCBuffers[2]);
	Radix008A(tmpUAV, pSRV_Dst, thread_count, istride);
	//Radix008A(textSB->UAV(), pSRV_Dst, thread_count, istride);
	//SaveFile(2);
	renderSrvs[2]->SRV(pSRV_Dst);

	istride /= 8;
	shader->AsConstantBuffer("CB_ChangePerCall")->SetConstantBuffer(RadixCBuffers[3]);
	Radix008A(pUAV_Dst, tmpSRV, thread_count, istride);
	//Radix008A(pUAV_Dst, textSB->SRV(), thread_count, istride);
	//SaveFile(3);
	renderSrvs[3]->SRV(tmpSRV);

	istride /= 8;
	shader->AsConstantBuffer("CB_ChangePerCall")->SetConstantBuffer(RadixCBuffers[4]);
	Radix008A(tmpUAV, pSRV_Dst, thread_count, istride);
	//Radix008A(textSB->UAV(), pSRV_Dst, thread_count, istride);
	//SaveFile(4);
	renderSrvs[4]->SRV(pSRV_Dst);

	istride /= 8;
	shader->AsConstantBuffer("CB_ChangePerCall")->SetConstantBuffer(RadixCBuffers[5]);
	Radix008A(pUAV_Dst, tmpSRV, thread_count, istride);
	//Radix008A(pUAV_Dst, textSB->SRV(), thread_count, istride);
	//SaveFile(5);
	renderSrvs[5]->SRV(tmpSRV);

	//RenderFFT();
	//exit(0);
}

void FFT::RenderFFT()
{
	renderSrvs[0]->Render();

	renderSrvs[1]->Render();

	renderSrvs[2]->Render();

	renderSrvs[3]->Render();

	renderSrvs[4]->Render();

	renderSrvs[5]->Render();
}

void FFT::SaveFile(UINT num)
{
	UINT size = (512 * slices) * 512;
	Vector2* output = new Vector2[size];
	textSB->Copy(output, sizeof(Vector2)* size);

	string str = "TestFFT_" + to_string(num) + ".csv";
	FILE* file;
	fopen_s(&file, str.c_str(), "w");
	for (UINT i = 0; i < size; i++)
	{
		Vector2 temp = output[i];

		fprintf(file,
			"%d,%f,%f\n",
			i,
			temp.x,temp.y
		);
	}
	fclose(file);

	SafeDeleteArray(output);
}
