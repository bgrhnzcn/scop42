/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VulkanInstance.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrhnzcn <bgrhnzcn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 03:20:58 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/10/20 22:48:57 by bgrhnzcn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VULKAN_INSTANCE_HPP
#	define VULKAN_INSTANCE_HPP

#	ifndef NDEBUG
	const static bool enableDebug = false;
#	else
	const static bool enableDebug = true;
#	endif

#	define GLFW_INCLUDE_VULKAN
#	include <glfw3.h>
#	include <iostream>
#	include <vector>

class VulkanInstance
{
	public:
		VulkanInstance();
		~VulkanInstance();
	private:
		VkInstance vkInst;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice device;
	private:
		VkApplicationInfo InitAppInfo();
		VkInstanceCreateInfo InitCreateInfo(const VkApplicationInfo &appInfo,
				std::vector<const char *> &extensions,
				std::vector<const char *> &layers);
		void GetValidationLayers(uint32_t &layerCountOut,
				std::vector<VkLayerProperties> &availableLayers,
				std::vector<const char *> &layerRequests);
		VkDebugUtilsMessengerCreateInfoEXT InitDebugMessenger(
				);
		void CreateDebugMessanger();
		void SelectPhysicalDevice();
	private: //Static Members
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData);
};

#endif //VULKAN_INSTANCE_HPP
