/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrhnzcn <bgrhnzcn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:37:29 by bgrhnzcn          #+#    #+#             */
/*   Updated: 2024/10/18 01:46:10 by bgrhnzcn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <glfw3.h>

int main()
{
	if (glfwInit() == GLFW_FALSE)
		return (1);
	return (0);
}
