#nom de l'executable
NAME =  minishell

#compiler ; compilateur, flag de debogue, flag d'exigence
CC = cc
CFLAGS = -g3 -Wall -Werror -Wextra -g3

#include
INCL = inc/libft/libft.a -lreadline

#fichiers sources
SRCS = src/builtins/cd.c \
	src/builtins/echo.c \
	src/builtins/env.c \
	src/builtins/exit.c \
	src/builtins/export.c \
	src/builtins/pwd.c \
	src/builtins/unset.c \
	src/errors/print_errors.c \
	src/errors/print_errors2.c \
	src/exec/build_cmds.c \
	src/exec/child.c \
	src/exec/exec.c \
	src/exec/execute.c \
	src/exec/loop.c \
	src/exec/exec_secours.c \
	src/exec/parent.c \
	src/exec/s.c \
	src/expansions/expansions.c \
	src/expansions/mark_quotes.c \
	src/expansions/mark_vars_2.c \
	src/expansions/mark_vars.c \
	src/expansions/mitigate.c \
	src/init/init_env.c \
	src/init/reinit_pwd.c \
	src/parsing/initial_split.c \
	src/parsing/parsing.c \
	src/path_expansion/path_expansion.c \
	src/redirections/file_reader.c \
	src/redirections/heredoc.c \
	src/redirections/redirections.c \
	src/signals/pid_list.c \
	src/signals/signals.c \
	src/utils/build_cmds/cmd_list1.c \
	src/utils/build_cmds/redir_list1.c \
	src/utils/build_cmds/redir_list2.c \
	src/utils/build_cmds/build_cmd.c \
	src/utils/build_cmds/build_cmd2.c \
	src/utils/build_cmds/build_cmd3.c \
	src/utils/init/init_utils.c \
	src/utils/env/manage_env.c \
	src/utils/env/manage_env2.c \
	src/utils/env/env_export.c \
	src/utils/parsing/detect_quoting.c \
	src/utils/parsing/detect_quoting2.c \
	src/utils/parsing/IS_utils.c \
	src/utils/parsing/IS_utils2.c \
	src/utils/parsing/IS_utils3.c \
	src/utils/parsing/IS_utils4.c \
	src/utils/parsing/paths.c \
	src/utils/parsing/heredoc.c \
	src/utils/parsing/redirections.c \
	src/utils/parsing/redirections2.c \
	src/utils/parsing/redirections3.c \
	src/utils/parsing/str_manipulation.c \
	src/utils/type_validators/paths.c \
	src/utils/type_validators/separators.c \
	src/utils/type_validators/vars.c \
	src/utils/type_validators/multicheck.c \
	src/utils/data_printers.c \
	src/exec/ES1.c \
	src/main.c \

#objets
OBJS = $(SRCS:.c=.o)

#les fichiers obj prennent le nom des fichiers sources "*.c"
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

${NAME}: ${OBJS}
	make -C inc/libft/
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(INCL)

#commandes
# test : $(NAME)

test:
	gcc -Wall -Wextra -Werror -g3 src/main.c src/path_expansion/*.c src/errors/*.c src/exec/*.c src/builtins/*.c src/expansions/*.c src/init/*.c src/parsing/*.c src/signals/*.c src/utils/env/*.c inc/libft/*.c inc/libft/garbage_collector/*.c src/utils/type_validators/*.c src/utils/parsing/*.c inc/libft/gnl/*.c src/redirections/*.c src/utils/*.c src/utils/build_cmds/*.c -lreadline -o test

retest :
	rm test && make test

all : $(NAME)

clean :
	rm -rf $(OBJS)
	@cd inc/libft/ && $(MAKE) clean

fclean : clean
	rm -rf $(NAME)
	@cd inc/libft/ && $(MAKE) fclean

re : fclean all
