
// Include from DontCrashEngine
#include <WindowManager.h>
#include <ResourceFile.h>
#include <WindowConfig.h>

// Include from STL
#include <string>
#include <vector>
#include <fstream>

#include <iostream>


int main(int argc, char* args[])
{
	if (argc < 2)
		return 1;
	std::string path = args[1];
	DC_Engine::WindowConfig systemConfig(DC_Engine::ApiType::kSDL, "ResourcePacker", 1024, 768);
	auto pSystem = DC_Engine::WindowManager::Create(systemConfig);
	std::vector<std::string> files = pSystem->GetAllFiles(path);

	DC_Engine::ResourceFile resources;
	for (auto& file : files)
	{
		std::string resourcePath = path + "/" + file;
		std::vector<char> data = pSystem->GetBinaryFromFile(resourcePath);
		resources.AddResource(file, std::move(data));
	}
	resources.Save(args[2]);

	return 0;

}