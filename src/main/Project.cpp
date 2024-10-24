#include "Project.hpp"

void Project::InitWindow()
{
	if (glfwInit() == GLFW_FALSE)
		throw std::runtime_error("Failed to init GLFW");
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(1920, 1920, "SCOP", nullptr, nullptr);
	if (window == nullptr)
		throw std::runtime_error("GLFW failed to open window!");
}

void Project::InitVulkan()
{
	vulkan = new VulkanInstance();
}

void Project::Run()
{
	while (glfwWindowShouldClose(window) != GLFW_TRUE)
	{
		glfwPollEvents();
		//std::cout << "Updating Window." << std::endl;
	}
}

void Project::Close()
{
	delete vulkan;
	glfwDestroyWindow(window);
	glfwTerminate();
}

Project::Project()
{
	InitWindow();
	InitVulkan();
	Run();
	Close();
}
