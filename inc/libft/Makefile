# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/08 18:19:26 by sgoldenb          #+#    #+#              #
#    Updated: 2024/09/26 15:17:57 by sgoldenb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
AR = ar rc
SRC = ft_powerofi.c ft_sqrt.c ft_arrloc.c ft_arrcmp.c ft_arrfree.c ft_strjoinsep.c ft_arrdup.c ft_frptr.c ft_arrlen.c ft_atoi.c ft_bzero.c ft_calloc.c ft_cisar.c ft_countwords.c ft_intlen.c ft_isalnum.c ft_strblen.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_lentillc.c ft_memalloc.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memdel.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putchar.c ft_putendl_fd.c ft_putendl.c ft_putnbr_fd.c ft_putnbr.c ft_putstr_fd.c ft_putstr.c ft_split.c ft_strcat.c ft_strchr.c ft_strclr.c ft_strcmp.c ft_strcpy.c ft_strdel.c ft_strdup.c ft_strequ.c ft_striter.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmap.c ft_strmapi.c ft_strncat.c ft_strncmp.c ft_strncpy.c ft_strnequ.c ft_strnew.c ft_strnstr.c ft_strrchr.c ft_strstr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c ft_putnbrbase.c ft_print_tab.c
SRCBONUS = ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c
SRCGNL = gnl/get_next_line_utils.c gnl/get_next_line.c
SRCPRINTF =	ft_printf.c\
		ft_printpointer.c\
		ft_putchar_f.c\
		ft_putnbr_base_f.c\
		ft_putstr_f.c\
		ft_putunsigned.c
SRCGARB = garbage_collector/add_refs.c \
	garbage_collector/debug_fcts.c \
	garbage_collector/flush.c \
	garbage_collector/gc_itoa.c \
	garbage_collector/gc_split.c \
	garbage_collector/gc_strdup.c \
	garbage_collector/gc_strjoin.c \
	garbage_collector/gc_strjoinsep.c \
	garbage_collector/gc_strnew.c \
	garbage_collector/gc_strtrim.c \
	garbage_collector/ms_collect.c \
	garbage_collector/search_refs.c \
	garbage_collector/update_refs.c \
	garbage_collector/utils.c\
	garbage_collector/wrappers.c \

OBJ = $(SRC:.c=.o)
OBJBONUS = $(SRCBONUS:.c=.o)
OBJGNL = $(SRCGNL:.c=.o)
OBJPRINTF = $(SRCPRINTF:.c=.o)
OBJGARB = $(SRCGARB:.c=.o)



$(NAME): $(OBJ) ${OBJBONUS} ${OBJGNL} ${OBJPRINTF} $(OBJGARB)
	${AR} $@ $(OBJ) ${OBJBONUS} ${OBJGNL} ${OBJPRINTF} $(OBJGARB)

clean:
	$(RM) $(OBJ) ${OBJBONUS} ${OBJGNL} ${OBJPRINTF} $(OBJGARB)

fclean: clean
		$(RM) $(NAME)

re: fclean  all

bonus:	${OBJBONUS}
		${AR} ${NAME} ${OBJBONUS}

gnl:	${OBJGNL}
		${AR} ${NAME} ${OBJGNL}

printf:	${OBJPRINTF}
		${AR} ${NAME} ${OBJPRINTF}

garb :$(OBJGARB)
		${AR} ${NAME} $(OBJGARB)

all: $(NAME) bonus gnl printf garb

.PHONY: clean fclean re bonus gnl all printf
