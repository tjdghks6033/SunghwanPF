#include "Framework.h"
#include "Billboard.h"

Billboard::Billboard(Shader * shader)
	: Renderer(shader)
{
	Topology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	sMaps = shader->AsSRV("Maps");
}

Billboard::~Billboard()
{
	SafeDelete(textures);
}

void Billboard::Add(Vector3 & position, Vector2 & scale, float & random)
{
	VertexScale vertex;
	vertex.Position = position;
	vertex.Scale = scale;
	vertex.Random = random;

	vertices.push_back(vertex);
}

void Billboard::AddTexture(wstring file)
{
	SafeDelete(textures);

	textureFiles.push_back(file);
}

void Billboard::Update()
{
	static float uvrandom;
	Super::Update();
	
	if(is_right)
		uvrandom += 0.001f * Time::Get()->Delta();
	else if (!is_right)
		uvrandom -= 0.001f * Time::Get()->Delta();

	if (uvrandom > 0.02f)
		is_right = false;
	if (uvrandom < 0.0f)
		is_right = true;

	shader->AsScalar("uvrandom")->SetFloat(uvrandom);

	
}

void Billboard::Render()
{
	if (vertices.size() != vertexCount)
	{
		vertexCount = vertices.size();

		SafeDelete(vertexBuffer);
		vertexBuffer = new VertexBuffer(&vertices[0], vertices.size(), sizeof(VertexScale));
	}

	if (textureFiles.size() > 0 && textures == NULL)
		textures = new TextureArray(textureFiles);

	Super::Render();

	sMaps->SetResource(textures->SRV());
	shader->Draw(0, Pass(), vertexCount);

}
