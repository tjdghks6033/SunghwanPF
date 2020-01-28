#include "stdafx.h"
#include "Main.h"
#include "Systems/Window.h"

#include "WaterDemo.h"
#include "ScatteringDemo.h"
#include "DefferedShadowDemo.h"
#include "DefferedSpotLightDemo.h"
#include "DefferedPointLightsArrayDemo.h"
#include "DefferedPointLightsDemo.h"
#include "DefferedDemo.h"
#include "TessDistanceDemo.h"
#include "TessQuadDemo.h"
#include "TessTriangleDemo.h"
#include "DynamicCubeMapDemo.h"
#include "ShadowDemo.h"
#include "ProjectorDemo.h"
#include "ViewportDemo.h"
#include "MrtDemo.h"
#include "RtvDemo.h"
#include "LightingDemo.h"
#include "NormalMapDemo.h"
#include "SpecularDemo.h"
#include "InstanceColliderDemo.h"
#include "ByteAddressDemo.h"
#include "ThreadDemo.h"
#include "InstanceFrameworkDemo.h"
#include "InstancingDemo.h"
#include "GeometryBillboardDemo.h"
#include "WeatherDemo.h"
//#include "CpuBillboardDemo.h"
#include "ColliderDemo.h"
#include "TerrainSplattingDemo.h"
#include "TerrainLineDemo.h"
#include "TerrainBrushDemo.h"
#include "CubeSkyDemo.h"
#include "FrameworkDemo.h"
#include "RaycastDemo.h"
#include "ProjectionDemo.h"
#include "VerticalPickDemo.h"
#include "GetHeightDemo.h"
#include "TextureDemo.h"
#include "MeshDemo.h"
#include "TerrainDemo.h"
#include "GridDemo.h"
#include "IndexDemo.h"
#include "WorldDemo.h"
#include "RectDemo.h"
#include "VertexDemo.h"

void Main::Initialize()
{
	Push(new WaterDemo());
	//Push(new ScatteringDemo());
	//Push(new DefferedShadowDemo());
	//Push(new DefferedSpotLightDemo());
	//Push(new DefferedPointLightsArrayDemo());
	//Push(new DefferedPointLightsDemo());
	//Push(new DefferedDemo());
	//Push(new TessDistanceDemo());
	//Push(new TessQuadDemo());
	//Push(new TessTriangleDemo());
	//Push(new DynamicCubeMapDemo());
	//Push(new ShadowDemo());
	//Push(new ProjectorDemo());
	//Push(new ViewportDemo());
	//Push(new MrtDemo());
	//Push(new RtvDemo());
	//Push(new LightingDemo());
	//Push(new NormalMapDemo());
	//Push(new SpecularDemo());
	//Push(new InstanceColliderDemo());
	//Push(new ByteAddressDemo());
	//Push(new ThreadDemo());
	//Push(new InstanceFrameworkDemo());
	//Push(new InstancingDemo());
	//Push(new GeometryBillboardDemo());
	//Push(new WeatherDemo());
	//Push(new CpuBillboardDemo());
	//Push(new ColliderDemo());
	//Push(new TerrainSplattingDemo());
	//Push(new TerrainLineDemo());
	//Push(new TerrainBrushDemo());
	//Push(new CubeSkyDemo());
	//Push(new FrameworkDemo());
	//Push(new RaycastDemo());
	//Push(new ProjectionDemo());
	//Push(new VerticalPickDemo());
	//Push(new GetHeightDemo());
	//Push(new TextureDemo());
	//Push(new MeshDemo());
	//Push(new TerrainDemo());
	//Push(new GridDemo());
	//Push(new IndexDemo());
	//Push(new WorldDemo());
	//Push(new RectDemo());
	//Push(new VertexDemo());
}

void Main::Ready()
{

}

void Main::Destroy()
{
	for (IExecute* exe : executes)
	{
		exe->Destroy();
		SafeDelete(exe);
	}
}

void Main::Update()
{
	for (IExecute* exe : executes)
		exe->Update();
}

void Main::PreRender()
{
	for (IExecute* exe : executes)
		exe->PreRender();
}

void Main::Render()
{
	for (IExecute* exe : executes)
		exe->Render();
}

void Main::PostRender()
{
	for (IExecute* exe : executes)
		exe->PostRender();
}

void Main::ResizeScreen()
{
	for (IExecute* exe : executes)
		exe->ResizeScreen();
}

void Main::Push(IExecute * execute)
{
	executes.push_back(execute);

	execute->Initialize();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	D3DDesc desc;
	desc.AppName = L"D3D Game";
	desc.Instance = instance;
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.Handle = NULL;
	desc.Width = 1280;
	desc.Height = 720;
	desc.Background = Color(0.3f, 0.3f, 0.3f, 1.0f);
	D3D::SetDesc(desc);

	Main* main = new Main();
	WPARAM wParam = Window::Run(main);

	SafeDelete(main);

	return wParam;
}