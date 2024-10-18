/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buozcan <buozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:37:29 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/10/18 21:06:22 by buozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <glfw3.h>

int main()
{
	GLFWwindow *win;
	if (glfwInit() == GLFW_FALSE)
		return (1);
	win = glfwCreateWindow(1920, 1920, "SCOP", nullptr, nullptr);
	if (win == nullptr)
		return (std::cout << "Failed when open window.\n", 1);
	while (glfwWindowShouldClose(win) != GLFW_TRUE)
	{
		std::cout << "Updating Window." << std::endl;
	}
	return (0);
}
