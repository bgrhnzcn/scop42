/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VulkanInstance.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrhnzcn <bgrhnzcn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 03:45:21 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/10/25 19:18:37 by bgrhnzcn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <VulkanInstance.hpp>

VulkanInstance::VulkanInstance()
{
	//Not mandotory, useful to make driver work better.
	VkApplicationInfo appInfo = InitAppInfo();
	//Get used extensions by our window management API and add debug for later use.
	//Checks is therre any problem with extensions.
	if (!GetRequiredExtensions())
		throw std::runtime_error("Error encountered while getting required extensions.");
	//Checks validation layers for debug mode. If requested layer can't be find, it throws exception.
	if (enableValidationLayers && !CheckSupportedValidationLayers())
		throw std::runtime_error("Some validation layers not supported.");
	//Mandatory. We use this data to initialize Vulkan.
	//Every entry in here used for some important aspect of Vulkan Instance.
	VkInstanceCreateInfo createInfo = InitCreateInfo(appInfo);
	//Finally create our Vulkan Instance. (Similiar to OpenGL Context)
	if (vkCreateInstance(&createInfo, nullptr, &m_vkInst) != VK_SUCCESS)
		throw std::runtime_error("Can't initialize VkInstance!");
	if (!CreateDebugMessenger())
		throw std::runtime_error("Failed to load extension function: vkCreateDebugUtilsMessengerEXT");
	if (!SelectPhysicalDevice())
		throw std::runtime_error("No Suitable Device for selection.");
}

VulkanInstance::~VulkanInstance()
{
	vkDestroyInstance(m_vkInst, nullptr);
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

bool VulkanInstance::GetRequiredExtensions()
{
	//Variables to store data about GLFW extensions.
	uint32_t extCount = 0;
	const char **glfwExtNames = nullptr;
	//We get how many extansion used by glfw and what is their names.
	//We will use this data to make Vulkan to load this extansions.
	glfwExtNames = glfwGetRequiredInstanceExtensions(&extCount);
	if (glfwExtNames == nullptr)
		return false;
	m_extensions = std::vector<const char *>(glfwExtNames, glfwExtNames + extCount);
	m_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	std::cout << "Required Extensions: " << m_extensions.size() << std::endl;
	for (const char *val : m_extensions)
		std::cout << "\tExtension Name: " << val << std::endl;
	return true;
}

bool VulkanInstance::CheckSupportedValidationLayers()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	//Variables to store data about Validation layers.
	std::vector<VkLayerProperties> availableLayers(layerCount);
	//Get available layers to check if our requested layer is available.
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	//We are adding requested layers.
	m_layers.push_back("VK_LAYER_KHRONOS_validation");
	//Not mandotory. Checks existence of validation layers. If fail program aborts.
	std::cout << "Checking for layers: " << m_layers.size() << std::endl;
	for (const char* layerName : m_layers)
	{
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}
		if (!layerFound)
			return false;
		std::cout << "\tFind layer: " << layerName << std::endl;
	}
	return true;
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

VkDeviceQueueCreateInfo VulkanInstance::InitDeviceQueueCreateInfo(QueueFamilyIndices queueFamilyIndices)
{
	float queuePriority = 1.0f;
	return VkDeviceQueueCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = {},
		.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(),
		.queueCount = 1,
		.pQueuePriorities = &queuePriority,
	};
}

bool VulkanInstance::CreateDebugMessenger()
{
	//This part creates debug messenger for callbacks and validation layers.
	//We will load this function manually because it is part of an extension.
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_vkInst, "vkCreateDebugUtilsMessengerEXT");
	if (func == nullptr)
		return false;
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = InitDebugMessenger();
	func(m_vkInst, &debugCreateInfo, nullptr, &m_debugMessenger);
	return true;
}

bool VulkanInstance::SelectPhysicalDevice()
{
	m_physicalDevice = nullptr;
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_vkInst, &deviceCount, nullptr);
	if (deviceCount == 0)
		throw std::runtime_error("Can't find any Physical Device supports Vulkan.");
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_vkInst, &deviceCount, devices.data());
	for (auto &device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			m_physicalDevice = device;
			break;
		}
	}
	if (m_physicalDevice == nullptr)
		return false;
	return true;
}

bool VulkanInstance::IsDeviceSuitable(VkPhysicalDevice &device)
{
	QueueFamilyIndices indices = FindQueueFamilies(device);
	if (indices.IsComplete())
		return true;
	return false;
}

VulkanInstance::QueueFamilyIndices VulkanInstance::FindQueueFamilies(VkPhysicalDevice &device)
{
	QueueFamilyIndices indices;
	uint32_t familyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
	std::vector<VkQueueFamilyProperties> families(familyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());
	for (uint32_t i = 0; i < families.size(); i++)
	{
		if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;
		if (indices.IsComplete())
			break;
	}
	return indices;
}

VkInstanceCreateInfo VulkanInstance::InitCreateInfo(const VkApplicationInfo &appInfo)
{
	return VkInstanceCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = nullptr,
		.flags = {},
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = static_cast<uint32_t>(m_layers.size()),
		.ppEnabledLayerNames = m_layers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(m_extensions.size()),
		.ppEnabledExtensionNames = m_extensions.data()
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

bool VulkanInstance::QueueFamilyIndices::IsComplete()
{
	if (graphicsFamily.has_value())
		return true;
	return false;
}
