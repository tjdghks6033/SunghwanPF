#include "stdafx.h"
#include "InstancingDemo.h"
#include "Viewer/Freedom.h"

void InstancingDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -17);
	((Freedom *)Context::Get()->GetCamera())->Speed(100, 2);

	shader = new Shader(L"26_Instancing.fx");

	perFrame = new PerFrame(shader);
	
	material = new Material(shader);
	material->DiffuseMap(L"Box.png");

	for (int i = 0; i < 500; i++)
	{
		//transforms[i] = new Transform(shader);
		transforms[i] = new Transform();
		transforms[i]->Position(Math::RandomVec3(-30, 30));
		transforms[i]->Scale(Math::RandomVec3(1.0, 2.5f));
		transforms[i]->RotationDegree(Math::RandomVec3(-180, 180));

		worlds[i] = transforms[i]->World();
	}	

	CreateMesh();

	instanceBuffer = new VertexBuffer(worlds, 500, sizeof(Matrix), 1);
}

void InstancingDemo::Destroy()
{
	
}

void InstancingDemo::Update()
{
	perFrame->Update();
	
	//for (int i = 0; i < 500; i++)
	//	transforms[i]->Update();
}

void InstancingDemo::Render()
{
	perFrame->Render();

	material->Render();

	vertexBuffer->Render();
	instanceBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//for (int i = 0; i < 500; i++)
	//{
		//transforms[i]->Render();
		//shader->DrawIndexed(0, 0, indices.size());
	//}

	shader->DrawIndexedInstanced(0, 0, indices.size(), 500);
}

void InstancingDemo::CreateMesh()
{
	float w, h, d;
	w = h = d = 0.5f;

	//Front
	vertices.push_back(Mesh::MeshVertex(-w, -h, -d, 0, 1, 0, 0, -1, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(-w, +h, -d, 0, 0, 0, 0, -1, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, -d, 1, 0, 0, 0, -1, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, -h, -d, 1, 1, 0, 0, -1, 1, 0, 0));

	//Back
	vertices.push_back(Mesh::MeshVertex(-w, -h, +d, 1, 1, 0, 0, 1, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, -h, +d, 0, 1, 0, 0, 1, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, +d, 0, 0, 0, 0, 1, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(-w, +h, +d, 1, 0, 0, 0, 1, -1, 0, 0));

	//Top
	vertices.push_back(Mesh::MeshVertex(-w, +h, -d, 0, 1, 0, 1, 0, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(-w, +h, +d, 0, 0, 0, 1, 0, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, +d, 1, 0, 0, 1, 0, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, -d, 1, 1, 0, 1, 0, 1, 0, 0));

	//Bottom
	vertices.push_back(Mesh::MeshVertex(-w, -h, -d, 1, 1, 0, -1, 0, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, -h, -d, 0, 1, 0, -1, 0, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, -h, +d, 0, 0, 0, -1, 0, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(-w, -h, +d, 1, 0, 0, -1, 0, -1, 0, 0));

	//Left
	vertices.push_back(Mesh::MeshVertex(-w, -h, +d, 0, 1, -1, 0, 0, 0, 0, -1));
	vertices.push_back(Mesh::MeshVertex(-w, +h, +d, 0, 0, -1, 0, 0, 0, 0, -1));
	vertices.push_back(Mesh::MeshVertex(-w, +h, -d, 1, 0, -1, 0, 0, 0, 0, -1));
	vertices.push_back(Mesh::MeshVertex(-w, -h, -d, 1, 1, -1, 0, 0, 0, 0, -1));

	//Right
	vertices.push_back(Mesh::MeshVertex(+w, -h, -d, 0, 1, 1, 0, 0, 0, 0, 1));
	vertices.push_back(Mesh::MeshVertex(+w, +h, -d, 0, 0, 1, 0, 0, 0, 0, 1));
	vertices.push_back(Mesh::MeshVertex(+w, +h, +d, 1, 0, 1, 0, 0, 0, 0, 1));
	vertices.push_back(Mesh::MeshVertex(+w, -h, +d, 1, 1, 1, 0, 0, 0, 0, 1));


	indices = 
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	vertexBuffer = new VertexBuffer(&vertices[0], vertices.size(), sizeof(Mesh::MeshVertex));
	indexBuffer = new IndexBuffer(&indices[0], indices.size());
}
