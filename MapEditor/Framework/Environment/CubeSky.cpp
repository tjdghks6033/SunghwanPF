#include "Framework.h"
#include "CubeSky.h"

CubeSky::CubeSky(wstring file, Shader * shader)
	: shader(shader)
{
	if (shader == NULL)
	{
		this->shader = new Shader(L"15_CubeSky.fxo");
		bCreateShader = true;
	}


	sphereRender = new MeshRender(this->shader, new MeshSphere(0.5f));
	sphereRender->AddTransform()->Scale(500, 500, 500);

	wstring temp = L"../../_Textures/" + file;
	Check(D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), temp.c_str(), NULL, NULL, &srv, NULL
	));

	sSrv = this->shader->AsSRV("SkyCubeMap");


	//DepthStencilState
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		desc.DepthEnable = false;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
		desc.StencilEnable = true;
		desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		D3D11_DEPTH_STENCILOP_DESC opDesc;
		ZeroMemory(&opDesc, sizeof(D3D11_DEPTH_STENCILOP_DESC));
		opDesc.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		opDesc.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		opDesc.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		opDesc.StencilFunc = D3D11_COMPARISON_EQUAL;

		desc.FrontFace = opDesc;
		desc.BackFace = opDesc;

		Check(D3D::GetDevice()->CreateDepthStencilState(&desc, &dss));
	}
	sDss = shader->AsDepthStencil("SkyDepthStencil");
}

CubeSky::~CubeSky()
{
	if (bCreateShader == true)
		SafeDelete(shader);

	SafeRelease(srv);
	SafeRelease(dss);
	SafeDelete(sphereRender);
}

void CubeSky::Update()
{
	Vector3 position;
	Context::Get()->GetCamera()->Position(&position);
	sphereRender->GetTransform(0)->Position(position);
	sphereRender->UpdateTransforms();
}

void CubeSky::Render()
{
	sDss->SetDepthStencilState(0, dss);
	sSrv->SetResource(srv);

	sphereRender->Pass(pass);
	sphereRender->Render();
}
