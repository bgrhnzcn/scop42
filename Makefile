# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bgrhnzcn <bgrhnzcn@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/18 00:36:23 by bgrhnzcn          #+#    #+#              #
#    Updated: 2024/11/04 20:36:33 by bgrhnzcn         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = scop

SHELL = /bin/bash

COMPILER = g++

FLAGS = -Wall -Werror -Wextra -std=c++17 -D DEBUG

SRC_DIR = src

OBJ_DIR = obj

SRCS =	$(SRC_DIR)/main/main.cpp				\
		$(SRC_DIR)/main/Project.cpp				\
		$(SRC_DIR)/renderer/VulkanInstance.cpp	\

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

INC = $(shell find $(SRC_DIR) -type f -name "*.hpp")

INC_DIR = $(addprefix -I , $(shell dirname $(INC)))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(COMPILER) $(FLAGS) $(INC_DIR) $(GLFW_INC) -c $< -o $@

LIB_DIR = libs

all: $(NAME)

################################################################################
#                                                                              #
#                                   VULKAN                                     #
#                                                                              #
################################################################################

VULKAN_LAYERS_DIR = $(LIB_DIR)/Vulkan-ValidationLayers

VULKAN_LAYERS = $(LIB_DIR)/Vulkan-ValidationLayers/build/layers/libVkLayer_khronos_validation.so

VULKAN_LAYER_EXIST = $(shell vulkaninfo | grep VK_LAYER_KHRONOS_validation | wc -l)

$(VULKAN_LAYERS_DIR):
	git clone https://github.com/KhronosGroup/Vulkan-ValidationLayers.git $(VULKAN_LAYERS_DIR)

.ONESHELL: $(VULKAN_LAYERS)
$(VULKAN_LAYERS): $(VULKAN_LAYERS_DIR)
	cd $(VULKAN_LAYERS_DIR)
	cmake -S . -B build -D UPDATE_DEPS=ON -D BUILD_WERROR=ON -D BUILD_TESTS=ON -D CMAKE_BUILD_TYPE=Debug
	cmake --build build --config Debug

VULKAN_KHRONOS_VAL_LAYER_LINK = -L $(VULKAN_LAYERS_DIR)/build/layers -lVkLayer_khronos_validation

################################################################################
#                                                                              #
#                                    GLFW3                                     #
#                                                                              #
################################################################################

GLFW_DIR = $(LIB_DIR)/glfw

GLFW_INC = -I $(GLFW_DIR)/include/GLFW

GLFW_LINK = -L $(GLFW_DIR)/src -lglfw3 -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

GLFW = $(GLFW_DIR)/src/libglfw3.a

$(GLFW): $(GLFW_DIR)
	cd $(GLFW_DIR) && cmake . && make

$(GLFW_DIR):
	git clone https://github.com/glfw/glfw.git $(GLFW_DIR)

ifeq ($(VULKAN_LAYER_EXIST), 1)
$(NAME): $(GLFW) $(OBJS) $(INC)
	$(COMPILER) $(FLAGS) $(OBJS) $(GLFW_LINK) -o $@
else
$(NAME): $(GLFW) $(VULKAN_LAYERS) $(OBJS) $(INC)
	$(COMPILER) $(FLAGS) $(OBJS) $(GLFW_LINK) $(VULKAN_KHRONOS_VAL_LAYER_LINK) -o $@
endif

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.ONESHELL: run
run: all
ifneq ($(VULKAN_LAYER_EXIST), 1)
	export VK_LAYER_PATH="$(PWD)/libs/Vulkan-ValidationLayers/build/layers"
endif
	./scop

.PHONY: all clean fclean re run

#NAME = minishell
#
#CC = gcc
#
#OBJ = obj
#
#SRC = src
#
#CFLAGS = -Wall -Wextra -Werror
#
#READLINE_DIR = ./lib/readline-8.2
#
#READLINE_V = $(READLINE_DIR)/lib/libreadline.a
#
#LIBFT_DIR = ./lib/libft
#
#LIBFT = $(LIBFT_DIR)/libft.a
#
#INCLUDES = -I$(READLINE_DIR)/include -I$(LIBFT_DIR) -I./includes/
#
#DYLIBS = -L$(READLINE_DIR)/lib -lreadline
#
#ifeq ($(shell uname), Linux)
#	DYLIBS += -ltinfo
#endif
#
#SRCS = $(SRC)/exec/main.c \
#	$(SRC)/exec/main_utils.c \
#	$(SRC)/exec/exec.c \
#	$(SRC)/exec/exec_utils.c \
#	$(SRC)/exec/exec_err.c \
#	$(SRC)/exec/signals.c \
#	$(SRC)/builtins/env.c \
#	$(SRC)/builtins/pwd.c \
#	$(SRC)/builtins/exit.c \
#	$(SRC)/builtins/cd.c \
#	$(SRC)/builtins/cd_utils.c \
#	$(SRC)/builtins/unset.c \
#	$(SRC)/builtins/unset_utils.c \
#	$(SRC)/builtins/export.c \
#	$(SRC)/builtins/export_utils.c \
#	$(SRC)/builtins/echo.c \
#	$(SRC)/parse/token.c \
#	$(SRC)/parse/token2.c \
#	$(SRC)/parse/tokenizer.c \
#	$(SRC)/parse/quotes.c \
#	$(SRC)/parse/syntax.c \
#	$(SRC)/parse/expansion.c \
#	$(SRC)/parse/expansion_utils.c \
#	$(SRC)/parse/argv.c \
#	$(SRC)/parse/commands.c \
#	$(SRC)/io_operations/pipe.c \
#	$(SRC)/io_operations/io_utils.c \
#	$(SRC)/io_operations/redirections.c \
#	$(SRC)/io_operations/heredoc.c \
#
#OBJS = $(SRCS:$(SRC)/%.c=$(OBJ)/%.o)
#
#all: $(NAME)
#
#$(OBJ):
#	@mkdir obj
#	@mkdir obj/parse
#	@mkdir obj/exec
#	@mkdir obj/io_operations
#	@mkdir obj/builtins
#
#$(READLINE_DIR):
#	@echo "Downloading Readline-8.2..."
#	@curl https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz -o readline-8.2.tar.gz 2>&1 | awk '{printf "."; fflush()}'
#	@echo ""
#	@mkdir ./lib/readline-8.2
#	@tar xvfz readline-8.2.tar.gz > /dev/null 2> /dev/null
#	@rm readline-8.2.tar.gz
#
#clean:
#	@rm -rf $(OBJ)
#	@make -C lib/libft fclean
#
#fclean: clean
#	@rm -f ${NAME}
#
#re: fclean all
#
#run: $(NAME)
#	@./minishell
#
#$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
#	$(CC) $(CFLAGS) -o $@ -c $(INCLUDES) $?
#
#$(LIBFT):
#	@make -C lib/libft && make -C lib/libft clean
#
#$(NAME): $(READLINE_V) $(LIBFT) $(OBJS)
#	$(CC) $(CFLAGS) -o $(NAME) $(INCLUDES) $(OBJS) $(DYLIBS) $(LIBFT)
#
#$(READLINE_V): | $(READLINE_DIR)
#	@echo "Compiling Readline-8.2..."
#	@cd readline-8.2 && ./configure --prefix=$(PWD)/$(READLINE_DIR) 2>&1 | awk '{printf "."; fflush()}'
#	@cd readline-8.2 && make install 2>&1 | awk '{printf "."; fflush()}'
#	@printf "\n"
#	@rm -rf readline-8.2
#
#.PHONY: all re fclean clean
