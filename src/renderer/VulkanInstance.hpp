/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VulkanInstance.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrhnzcn <bgrhnzcn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 03:20:58 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/10/25 18:54:35 by bgrhnzcn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VULKAN_INSTANCE_HPP
#	define VULKAN_INSTANCE_HPP

#	ifdef DEBUG
		const static bool enableValidationLayers = true;
#	else
		const static bool enableValidationLayers = false;
#	endif

#	define GLFW_INCLUDE_NONE
#	include <vulkan/vulkan.hpp>
#	include <glfw3.h>
#	include <iostream>
#	include <vector>
#	include <optional>

class VulkanInstance
{
	private:
		struct QueueFamilyIndices;
	public:
		VulkanInstance();
		~VulkanInstance();
	private:
		std::vector<const char *> m_extensions;
		std::vector<const char *> m_layers;
		VkInstance m_vkInst;
		VkDebugUtilsMessengerEXT m_debugMessenger;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_logicalDevice;
	private:
		VkApplicationInfo InitAppInfo();
		VkInstanceCreateInfo InitCreateInfo(const VkApplicationInfo &appInfo);
		VkDebugUtilsMessengerCreateInfoEXT InitDebugMessenger();
		VkDeviceQueueCreateInfo InitDeviceQueueCreateInfo(QueueFamilyIndices queueFamilyIndices);
		bool GetRequiredExtensions();
		bool CheckSupportedValidationLayers();
		bool CreateDebugMessenger();
		bool SelectPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice &device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice &device);
	private: //Static Members
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData);
	private:
		struct QueueFamilyIndices
		{
			public:
				std::optional<uint32_t> graphicsFamily;
			public:
				bool IsComplete();
		};
};

#endif //VULKAN_INSTANCE_HPP
