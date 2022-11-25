# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ktada <ktada@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/19 02:41:21 by ktada             #+#    #+#              #
#    Updated: 2022/11/25 23:26:24 by ktada            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		= srcs/main.cpp 
OBJ_DIR = ./obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:%.cpp=%.o))
CXX			= c++
CXXFLAGS	= -std=c++98 -Wall -Wextra -Werror -Wconversion
RM			= rm -f
RM_DIR 		= rm -rf
NAME		= a.out

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $@);
	${CXX} ${CXXFLAGS} -I ./includes -c $< -o $@

all:		${NAME}


${NAME}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}
	
clean:
	$(RM_DIR) $(OBJ_DIR)
	

fclean:		clean
	${RM} ${NAME}

re:			fclean ${NAME}

.PHONY:		all clean fclean re