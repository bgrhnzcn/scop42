/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Project.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgrhnzcn <bgrhnzcn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:37:45 by buozcan           #+#    #+#             */
/*   Updated: 2024/10/24 22:39:12 by bgrhnzcn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROJECT_HPP
#	define PROJECT_HPP

#	include "VulkanInstance.hpp"

class Project
{
	private:
		GLFWwindow *window;
		VulkanInstance *vulkan;
	public:
		void InitWindow();
		void InitVulkan();
		void Run();
		void Close();
	public:
		Project();
};

#endif //PROJECT_HPP
