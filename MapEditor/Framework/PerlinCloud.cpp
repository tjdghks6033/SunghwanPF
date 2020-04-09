#include "Framework.h"
#include "PerlinCloud.h"

PerlinCloud::PerlinCloud(Shader * shader)
	: Renderer(shader)
{
	//sSRV = shader->AsSRV("CloudMap");

	render2D = new Render2D();
	render2D->GetTransform()->Position(400 + 100, D3D::Height() - 100, 0);
	render2D->GetTransform()->Scale(100, 100, 1);
	
	int skyPlaneResolution = 50;
	float skyPlaneWidth = 10.0f;
	float skyPlaneTop = 0.5f;
	float skyPlaneBottom = 0.0f;
	int textureRepeat = 2;

	// 하늘 평면 쉐이더 관련 매개 변수를 설정합니다.
	m_scale = 0.3f;
	m_brightness = 0.5f;

	// 번역을 0으로 초기화합니다.
	m_translation = 0.0f;

	// 하늘 평면을 만듭니다.
	float positionX = 0.0f;
	float positionY = 0.0f;
	float positionZ = 0.0f;
	float tu = 0.0f;
	float tv = 0.0f;

	// 하늘 평면 좌표를 보유 할 배열을 만듭니다.
	skyPlane = new SkyPlaneType[(skyPlaneResolution + 1) * (skyPlaneResolution + 1)];	

	// 하늘 평면에서 각 쿼드의 크기를 결정합니다.
	float quadSize = skyPlaneWidth / (float)skyPlaneResolution;

	// 너비를 기준으로 하늘 평면의 반지름을 계산합니다.
	float radius = skyPlaneWidth / 2.0f;

	// 증가 할 높이 상수를 계산합니다.
	float constant = (skyPlaneTop - skyPlaneBottom) / (radius * radius);

	// 텍스처 좌표 증가 값을 계산합니다.
	float textureDelta = (float)textureRepeat / (float)skyPlaneResolution;

	// 하늘 평면을 반복하고 제공된 증분 값을 기반으로 좌표를 만듭니다.
	for (int j = 0; j <= skyPlaneResolution; j++)
	{
		for (int i = 0; i <= skyPlaneResolution; i++)
		{
			// 정점 좌표를 계산합니다.
			positionX = (-0.5f * skyPlaneWidth) + ((float)i * quadSize);
			positionZ = (-0.5f * skyPlaneWidth) + ((float)j * quadSize);
			positionY = skyPlaneTop - (constant * ((positionX * positionX) + (positionZ * positionZ)));

			// 텍스처 좌표를 계산합니다.
			tu = (float)i * textureDelta;
			tv = (float)j * textureDelta;

			// 이 좌표를 추가하기 위해 하늘 평면 배열에 인덱스를 계산합니다.
			int index = j * (skyPlaneResolution + 1) + i;

			// 하늘 평면 배열에 좌표를 추가합니다.
			skyPlane[index].x = positionX;
			skyPlane[index].y = positionY;
			skyPlane[index].z = positionZ;
			skyPlane[index].tu = tu;
			skyPlane[index].tv = tv;
		}
	}


	// 하늘 평면에 대한 정점 및 인덱스 버퍼를 만듭니다.
	int index1 = 0;
	int index2 = 0;
	int index3 = 0;
	int index4 = 0;

	// 하늘 평면 메쉬의 정점 수를 계산합니다.
	vertexCount = (skyPlaneResolution + 1) * (skyPlaneResolution + 1) * 6;

	// 인덱스 수를 꼭지점 수와 같게 설정합니다.
	indexCount = vertexCount;

	// 정점 배열을 만듭니다.
	VertexType* vertices = new VertexType[vertexCount];
	
	// 인덱스 배열을 만듭니다.
	UINT* indices = new UINT[indexCount];

	// 인덱스를 정점 배열로 초기화 합니다.
	int index = 0;

	// 하늘 평면 배열 데이터로 꼭지점과 인덱스 배열을 로드합니다.
	for (int j = 0; j < skyPlaneResolution; j++)
	{
		for (int i = 0; i < skyPlaneResolution; i++)
		{
			index1 = j * (skyPlaneResolution + 1) + i;
			index2 = j * (skyPlaneResolution + 1) + (i + 1);
			index3 = (j + 1) * (skyPlaneResolution + 1) + i;
			index4 = (j + 1) * (skyPlaneResolution + 1) + (i + 1);

			// 삼각형 1 - 왼쪽 위
			vertices[index].position = Vector3(skyPlane[index1].x, skyPlane[index1].y, skyPlane[index1].z);
			vertices[index].texture = Vector2(skyPlane[index1].tu, skyPlane[index1].tv);
			indices[index] = index;
			index++;

			// 삼각형 1 - 오른쪽 위
			vertices[index].position = Vector3(skyPlane[index2].x, skyPlane[index2].y, skyPlane[index2].z);
			vertices[index].texture = Vector2(skyPlane[index2].tu, skyPlane[index2].tv);
			indices[index] = index;
			index++;

			// 삼각형 1 - 왼쪽 아래
			vertices[index].position = Vector3(skyPlane[index3].x, skyPlane[index3].y, skyPlane[index3].z);
			vertices[index].texture = Vector2(skyPlane[index3].tu, skyPlane[index3].tv);
			indices[index] = index;
			index++;

			// 삼각형 2 - 왼쪽 아래
			vertices[index].position = Vector3(skyPlane[index3].x, skyPlane[index3].y, skyPlane[index3].z);
			vertices[index].texture = Vector2(skyPlane[index3].tu, skyPlane[index3].tv);
			indices[index] = index;
			index++;

			// 삼각형 2 - 오른쪽 위
			vertices[index].position = Vector3(skyPlane[index2].x, skyPlane[index2].y, skyPlane[index2].z);
			vertices[index].texture = Vector2(skyPlane[index2].tu, skyPlane[index2].tv);
			indices[index] = index;
			index++;

			// 삼각형 2 - 오른쪽 아래
			vertices[index].position = Vector3(skyPlane[index4].x, skyPlane[index4].y, skyPlane[index4].z);
			vertices[index].texture = Vector2(skyPlane[index4].tu, skyPlane[index4].tv);
			indices[index] = index;
			index++;
		}
	}


	// 정점 버퍼를 만듭니다.
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexType));

	// 인덱스 버퍼를 만듭니다.
	indexBuffer = new IndexBuffer(indices, indexCount);


	// 하늘 평면 텍스처를 로드합니다. 
	// 클라우드 텍스처 객체를 생성합니다.
	CloudTexture = new Texture(L"Environment/Cloud.dds");

	sSRVCloud = shader->AsSRV("cloudTexture");

	// 섭동 텍스처 오브젝트를 생성합니다.
	PerturbTexture = new Texture(L"Environment/Perturb.dds");

	sSRVPertube = shader->AsSRV("perturbTexture");

	// 버텍스와 인덱스 버퍼가 생성되고 로드된 배열을 해제합니다.
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

}

PerlinCloud::~PerlinCloud()
{
	//SafeRelease(texture);
	//SafeRelease(srv);
	SafeDelete(CloudTexture);
	SafeDelete(PerturbTexture);
	SafeDelete(render2D);
}

void PerlinCloud::Update()
{
	Super::Update();

	m_translation += 0.1f;
	if (m_translation > 1.0f)
	{
		m_translation -= 1.0f;
	}

	//Vector3 position(0, 0, 0);
	////Context::Get()->GetCamera()->Position(&position);

	//ImGui::SliderFloat3("scale", scale, 0, 100);
	//ImGui::SliderFloat3("rotation", rotation, -3.14f, 3.14f);
	//ImGui::SliderFloat3("position2", position2, -100, 100);

	//transform->Scale(1000 + scale.x, 500 + scale.y, 1 + scale.z);
	//transform->Rotation(-Math::PI * 0.5f + rotation.x, 0 + rotation.y, 0 + rotation.z);
	//transform->Position(position.x + 95 + position2.x, position.y + 100.0f + position2.y, position.z + 300.0f + position2.z);
}

void PerlinCloud::Render()
{
	Super::Render();

	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	sSRVCloud->SetResource(CloudTexture->SRV());
	sSRVPertube->SetResource(PerturbTexture->SRV());

	//sSRV->SetResource(srv);
	//shader->Draw(0, Pass(), 6);
	shader->DrawIndexed(0, Pass(), indexCount);
}

void PerlinCloud::PostRender()
{
	render2D->Update();
	//render2D->SRV(srv);
	render2D->SRV(CloudTexture->SRV());
	render2D->SRV(PerturbTexture->SRV());
	render2D->Render();
}