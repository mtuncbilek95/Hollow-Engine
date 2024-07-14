#include <Engine/Graphics/API/GraphicsAPI.h>

using namespace Hollow;

int main()
{
	GraphicsAPI* GraphicsManager = GraphicsAPI::GetAPI();

	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.AppName = "Editor";
	instanceDesc.AppVersion = { 1, 0, 0 };
	instanceDesc.EngineName = "Hollow";
	instanceDesc.EngineVersion = { 1, 0, 0 };

	SharedPtr<GraphicsInstance> mInstance = GraphicsManager->CreateInstance(instanceDesc);
	SharedPtr<GraphicsDevice> mDevice = GraphicsManager->CreateDevice();

	return 0;
}
