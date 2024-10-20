/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VulkanInstance.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrhnzcn <bgrhnzcn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 03:45:21 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/10/20 04:03:49 by bgrhnzcn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <VulkanInstance.hpp>

VulkanInstance::VulkanInstance()
{
	//Variables to store data about GLFW extensions.
	uint32_t glfwExtCount = 0;
	const char **glfwExtNames = nullptr;
	//We get how many extansion used by glfw and what is their names.
	//We will use this data to make Vulkan to load this extansions.
	glfwExtNames = glfwGetRequiredInstanceExtensions(&glfwExtCount);
	//Not mandotory, useful to make driver work better.
	VkApplicationInfo appInfo = InitAppInfo();
	//Mandatory. We use this data to initialize Vulkan.
	//Every entry in here used for some important aspect of Vulkan Instance.
	VkInstanceCreateInfo createInfo = InitCreateInfo(appInfo, glfwExtCount, glfwExtNames);
	//Variables to store data about Validation layers.
	uint32_t layerCount;
	std::vector<VkLayerProperties> availableLayers;
	std::vector<const char *> layerRequests;
	//We are adding requested layers.
	layerRequests.push_back("VK_LAYER_KHRONOS_validation");
	//Not mandotory. Checks existence of validation layers. If fail program aborts.
	GetValidationLayers(layerCount, availableLayers, layerRequests);
	//Finally create our Vulkan Instance. (Similiar to OpenGL Context)
	vkCreateInstance(&createInfo, nullptr, &vkInst);
}

VulkanInstance::~VulkanInstance()
{
	vkDestroyInstance(vkInst, nullptr);
}

VkApplicationInfo VulkanInstance::InitAppInfo()
{
	return VkApplicationInfo
	{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = nullptr,
		.pApplicationName = "scop",
		.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
		.apiVersion = VK_API_VERSION_1_0,
	};
}

VkInstanceCreateInfo VulkanInstance::InitCreateInfo(const VkApplicationInfo &appInfo,
	const uint32_t glfwExtCount, const char **glfwExtNames)
{
	return VkInstanceCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = nullptr,
		.flags = {},
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = glfwExtCount,
		.ppEnabledExtensionNames = glfwExtNames
	};
}

void VulkanInstance::GetValidationLayers(uint32_t &layerCountOut, std::vector<VkLayerProperties> &availableLayers, std::vector<const char *> &layerRequests)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	layerCountOut = layerCount;

	availableLayers.resize(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	std::cout << "Available Vulkan layers:" << layerCount << std::endl;
	for (const auto &layerProperties : availableLayers)
		std::cout << layerProperties.layerName << ": " << layerProperties.description << std::endl;

	for (const char *requestedLayer : layerRequests)
	{
		bool layerExist = false;
		for (const auto &layerProperties : availableLayers)
		{
			if (std::string(requestedLayer) == std::string(layerProperties.layerName))
				layerExist = true;
		}
		if (layerExist == false)
		{
			std::cerr << "validation layer " << requestedLayer
					  << " requested, but not available!" << std::endl;
			throw std::runtime_error("");
		}
	}
	std::cout << "Requested Validation layers Available!" << std::endl;
}
