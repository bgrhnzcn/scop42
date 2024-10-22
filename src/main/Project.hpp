/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Project.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: buozcan <buozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:37:45 by buozcan           #+#    #+#             */
/*   Updated: 2024/10/22 21:08:43 by buozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROJECT_HPP
# define PROJECT_HPP

# include "VulkanInstance.hpp"

class Project
{
    private:
        GLFWwindow *window;
        VulkanInstance *vulkan;
    public:
        void InitWindow();
        void Run();
        void Close();
    public:
        Project();
};

#endif //PROJECT_HPP
