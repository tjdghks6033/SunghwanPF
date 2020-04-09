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

	// �ϴ� ��� ���̴� ���� �Ű� ������ �����մϴ�.
	m_scale = 0.3f;
	m_brightness = 0.5f;

	// ������ 0���� �ʱ�ȭ�մϴ�.
	m_translation = 0.0f;

	// �ϴ� ����� ����ϴ�.
	float positionX = 0.0f;
	float positionY = 0.0f;
	float positionZ = 0.0f;
	float tu = 0.0f;
	float tv = 0.0f;

	// �ϴ� ��� ��ǥ�� ���� �� �迭�� ����ϴ�.
	skyPlane = new SkyPlaneType[(skyPlaneResolution + 1) * (skyPlaneResolution + 1)];	

	// �ϴ� ��鿡�� �� ������ ũ�⸦ �����մϴ�.
	float quadSize = skyPlaneWidth / (float)skyPlaneResolution;

	// �ʺ� �������� �ϴ� ����� �������� ����մϴ�.
	float radius = skyPlaneWidth / 2.0f;

	// ���� �� ���� ����� ����մϴ�.
	float constant = (skyPlaneTop - skyPlaneBottom) / (radius * radius);

	// �ؽ�ó ��ǥ ���� ���� ����մϴ�.
	float textureDelta = (float)textureRepeat / (float)skyPlaneResolution;

	// �ϴ� ����� �ݺ��ϰ� ������ ���� ���� ������� ��ǥ�� ����ϴ�.
	for (int j = 0; j <= skyPlaneResolution; j++)
	{
		for (int i = 0; i <= skyPlaneResolution; i++)
		{
			// ���� ��ǥ�� ����մϴ�.
			positionX = (-0.5f * skyPlaneWidth) + ((float)i * quadSize);
			positionZ = (-0.5f * skyPlaneWidth) + ((float)j * quadSize);
			positionY = skyPlaneTop - (constant * ((positionX * positionX) + (positionZ * positionZ)));

			// �ؽ�ó ��ǥ�� ����մϴ�.
			tu = (float)i * textureDelta;
			tv = (float)j * textureDelta;

			// �� ��ǥ�� �߰��ϱ� ���� �ϴ� ��� �迭�� �ε����� ����մϴ�.
			int index = j * (skyPlaneResolution + 1) + i;

			// �ϴ� ��� �迭�� ��ǥ�� �߰��մϴ�.
			skyPlane[index].x = positionX;
			skyPlane[index].y = positionY;
			skyPlane[index].z = positionZ;
			skyPlane[index].tu = tu;
			skyPlane[index].tv = tv;
		}
	}


	// �ϴ� ��鿡 ���� ���� �� �ε��� ���۸� ����ϴ�.
	int index1 = 0;
	int index2 = 0;
	int index3 = 0;
	int index4 = 0;

	// �ϴ� ��� �޽��� ���� ���� ����մϴ�.
	vertexCount = (skyPlaneResolution + 1) * (skyPlaneResolution + 1) * 6;

	// �ε��� ���� ������ ���� ���� �����մϴ�.
	indexCount = vertexCount;

	// ���� �迭�� ����ϴ�.
	VertexType* vertices = new VertexType[vertexCount];
	
	// �ε��� �迭�� ����ϴ�.
	UINT* indices = new UINT[indexCount];

	// �ε����� ���� �迭�� �ʱ�ȭ �մϴ�.
	int index = 0;

	// �ϴ� ��� �迭 �����ͷ� �������� �ε��� �迭�� �ε��մϴ�.
	for (int j = 0; j < skyPlaneResolution; j++)
	{
		for (int i = 0; i < skyPlaneResolution; i++)
		{
			index1 = j * (skyPlaneResolution + 1) + i;
			index2 = j * (skyPlaneResolution + 1) + (i + 1);
			index3 = (j + 1) * (skyPlaneResolution + 1) + i;
			index4 = (j + 1) * (skyPlaneResolution + 1) + (i + 1);

			// �ﰢ�� 1 - ���� ��
			vertices[index].position = Vector3(skyPlane[index1].x, skyPlane[index1].y, skyPlane[index1].z);
			vertices[index].texture = Vector2(skyPlane[index1].tu, skyPlane[index1].tv);
			indices[index] = index;
			index++;

			// �ﰢ�� 1 - ������ ��
			vertices[index].position = Vector3(skyPlane[index2].x, skyPlane[index2].y, skyPlane[index2].z);
			vertices[index].texture = Vector2(skyPlane[index2].tu, skyPlane[index2].tv);
			indices[index] = index;
			index++;

			// �ﰢ�� 1 - ���� �Ʒ�
			vertices[index].position = Vector3(skyPlane[index3].x, skyPlane[index3].y, skyPlane[index3].z);
			vertices[index].texture = Vector2(skyPlane[index3].tu, skyPlane[index3].tv);
			indices[index] = index;
			index++;

			// �ﰢ�� 2 - ���� �Ʒ�
			vertices[index].position = Vector3(skyPlane[index3].x, skyPlane[index3].y, skyPlane[index3].z);
			vertices[index].texture = Vector2(skyPlane[index3].tu, skyPlane[index3].tv);
			indices[index] = index;
			index++;

			// �ﰢ�� 2 - ������ ��
			vertices[index].position = Vector3(skyPlane[index2].x, skyPlane[index2].y, skyPlane[index2].z);
			vertices[index].texture = Vector2(skyPlane[index2].tu, skyPlane[index2].tv);
			indices[index] = index;
			index++;

			// �ﰢ�� 2 - ������ �Ʒ�
			vertices[index].position = Vector3(skyPlane[index4].x, skyPlane[index4].y, skyPlane[index4].z);
			vertices[index].texture = Vector2(skyPlane[index4].tu, skyPlane[index4].tv);
			indices[index] = index;
			index++;
		}
	}


	// ���� ���۸� ����ϴ�.
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexType));

	// �ε��� ���۸� ����ϴ�.
	indexBuffer = new IndexBuffer(indices, indexCount);


	// �ϴ� ��� �ؽ�ó�� �ε��մϴ�. 
	// Ŭ���� �ؽ�ó ��ü�� �����մϴ�.
	CloudTexture = new Texture(L"Environment/Cloud.dds");

	sSRVCloud = shader->AsSRV("cloudTexture");

	// ���� �ؽ�ó ������Ʈ�� �����մϴ�.
	PerturbTexture = new Texture(L"Environment/Perturb.dds");

	sSRVPertube = shader->AsSRV("perturbTexture");

	// ���ؽ��� �ε��� ���۰� �����ǰ� �ε�� �迭�� �����մϴ�.
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