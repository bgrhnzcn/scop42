/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VulkanInstance.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrhnzcn <bgrhnzcn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 03:20:58 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/10/20 04:03:58 by bgrhnzcn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VULKAN_INSTANCE_HPP
#	define VULKAN_INSTANCE_HPP

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
	private:
		VkApplicationInfo InitAppInfo();
		VkInstanceCreateInfo InitCreateInfo(const VkApplicationInfo &appInfo,
				const uint32_t glfwExtCount, const char **glfwExtNames);
		void GetValidationLayers(uint32_t &layerCountOut,
				std::vector<VkLayerProperties> &availableLayers,
				std::vector<const char *> &layerRequests);
};

#endif //VULKAN_INSTANCE_HPP
