/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VulkanInstance.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrhnzcn <bgrhnzcn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 03:45:21 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/10/21 00:01:09 by bgrhnzcn         ###   ########.fr       */
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
	std::vector<const char *> extensions(glfwExtNames, glfwExtNames + glfwExtCount);
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	for (const char *val : extensions)
		std::cout << val << std::endl;
	//Not mandotory, useful to make driver work better.
	VkApplicationInfo appInfo = InitAppInfo();
	//Variables to store data about Validation layers.
	uint32_t layerCount = 0;
	std::vector<VkLayerProperties> availableLayers;
	std::vector<const char *> layers;
	if (enableDebug)
		//We are adding requested layers.
		layers.push_back("VK_LAYER_KHRONOS_validation");
	//Not mandotory. Checks existence of validation layers. If fail program aborts.
	GetValidationLayers(layerCount, availableLayers, layers);
	//Mandatory. We use this data to initialize Vulkan.
	//Every entry in here used for some important aspect of Vulkan Instance.
	VkInstanceCreateInfo createInfo = InitCreateInfo(appInfo, extensions, layers);
	//Finally create our Vulkan Instance. (Similiar to OpenGL Context)
	vkCreateInstance(&createInfo, nullptr, &vkInst);
	CreateDebugMessanger();
	SelectPhysicalDevice();
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

VkDebugUtilsMessengerCreateInfoEXT VulkanInstance::InitDebugMessenger(
)
{
	return VkDebugUtilsMessengerCreateInfoEXT
	{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.pNext = nullptr,
		.flags = {},
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
						|  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
						|  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
		.pfnUserCallback = VulkanInstance::DebugCallback,
		.pUserData = nullptr
	};
}

void VulkanInstance::CreateDebugMessanger()
{
	//This part creates debug messenger for callbacks and validation layers.
	//We will load this function manually because it is part of an extension.
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInst, "vkCreateDebugUtilsMessengerEXT");
	if (func == nullptr)
		throw std::runtime_error("Failed to load extension function: vkCreateDebugUtilsMessengerEXT");
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = InitDebugMessenger();
	func(vkInst, &debugCreateInfo, nullptr, &debugMessenger);
}

void VulkanInstance::SelectPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(vkInst, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(vkInst, &deviceCount, devices.data());
	device = devices[0];
}

VkInstanceCreateInfo VulkanInstance::InitCreateInfo(
	const VkApplicationInfo &appInfo,
	std::vector<const char *> &extensions,
	std::vector<const char *> &layers)
{
	return VkInstanceCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = nullptr,
		.flags = {},
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = static_cast<uint32_t>(layers.size()),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
		.ppEnabledExtensionNames = extensions.data()
	};
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	(void)pUserData;
	if (messageType != VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
		return VK_FALSE;
	switch (messageSeverity)
	{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			std::cout << "[ INFO ] " << pCallbackData->pMessage << std::endl;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			std::cout << "[ WARNING ] " << pCallbackData->pMessage << std::endl;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			std::cout << "[ ERROR ] " << pCallbackData->pMessage << std::endl;
			break;
		default:
			std::cout << "[ OTHER ] " << pCallbackData->pMessage << std::endl;
			break;
	}
	return VK_FALSE;
}

void VulkanInstance::GetValidationLayers(
	uint32_t &layerCountOut,
	std::vector<VkLayerProperties> &availableLayers,
	std::vector<const char *> &layerRequests)
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

