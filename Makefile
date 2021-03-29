# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lde-moul <lde-moul@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/04 15:30:01 by lde-moul          #+#    #+#              #
#    Updated: 2021/03/29 14:29:14 by lde-moul         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = lemipc

CC = clang
CFLAGS = -Wall -Wextra -Werror

SRCDIR = src
OBJDIR = obj

SRC = chase.c\
      exit.c\
      find_target.c\
      get_resources.c\
      join_map.c\
      main.c\
      map.c\
      message.c\
      print_map.c\
      semaphore.c\
      util.c\

OBJ := $(SRC:%.c=$(OBJDIR)/%.o)
SRC := $(SRC:%=$(SRCDIR)/%)
DEP := $(OBJ:%.o=%.d)


all: $(NAME)

clean:
	@/bin/rm -f $(OBJ) $(DEP)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

$(NAME): $(OBJDIR) $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $(OBJ)

-include $(DEP)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) -MMD -c -o $@ $(CFLAGS) $<

$(OBJDIR):
	@/bin/mkdir $(OBJDIR)

.PHONY: all clean fclean re
