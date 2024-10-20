/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrhnzcn <bgrhnzcn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:37:29 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/10/20 04:04:04 by bgrhnzcn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#include <iostream>
#include <vector>

#include <VulkanInstance.hpp>

int main()
{
	GLFWwindow *win;
	if (glfwInit() == GLFW_FALSE)
		return (1);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	win = glfwCreateWindow(1920, 1920, "SCOP", nullptr, nullptr);
	if (win == nullptr)
		return (std::cout << "Failed when open window.\n", 1);

	VulkanInstance vk;

	while (glfwWindowShouldClose(win) != GLFW_TRUE)
	{
		glfwPollEvents();
		//std::cout << "Updating Window." << std::endl;
	}
	glfwDestroyWindow(win);
	glfwTerminate();
	return (0);
}
