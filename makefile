#raccourcis
CC = gcc
FLAGS = -Wall -Werror -Wextra -g3
INCL = inc/libft/libft.a -lreadline
EXECNAME = minishell
SRCS = src/main.c \
	src/builtins/cd.c \
	src/builtins/echo.c \
	src/builtins/env.c \
	src/builtins/exit.c \
	src/builtins/export.c \
	src/builtins/print_errors.c \
	src/builtins/pwd.c \
	src/builtins/unset.c \
	src/exec/exec.c \
	src/redirections/file_reader.c \
	src/redirections/redirections.c \
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
	src/signals/signals.c \
	src/utils/env/env_export.c \
	src/utils/env/manage_env.c \
	src/utils/env/manage_env2.c \
	src/utils/parsing/detect_quoting.c \
	src/utils/parsing/detect_quoting2.c \
	src/utils/parsing/paths.c \
	src/utils/parsing/str_manipulation.c \
	src/utils/type_validators/paths.c \
	src/utils/type_validators/separators.c \
	src/utils/type_validators/vars.c \
	src/utils/data_printers.c \

OBJS = $(SRCS:.c=.o)

#les fichiers obj prennent le nom des fichiers sources "*.c"
%.o : %.c
	$(CC) $(FLAGS) -c $< -o $(<:.c=.o)

${EXECNAME}: ${OBJS}
	make -C inc/libft/
	$(CC) $(FLAGS) $(OBJS) -o $(EXECNAME) $(INCL)

#commandes
# test : $(EXECNAME)

test:
	gcc -Wall -Wextra -Werror -g3 src/main.c src/errors/*.c src/builtins/*.c src/expansions/*.c src/init/*.c src/parsing/*.c src/signals/*.c src/utils/env/*.c inc/libft/*.c inc/libft/garbage_collector/*.c src/utils/type_validators/*.c src/path_expansion/*.c src/utils/parsing/*.c inc/libft/gnl/*.c src/redirections/*.c src/utils/*.c src/utils/build_cmds/*.c -lreadline -o test

retest :
	rm test && make test

clean :
	rm -rf $(OBJS)
	@cd inc/libft/ && $(MAKE) clean

fclean : clean
	rm -rf $(EXECNAME)
	@cd inc/libft/ && $(MAKE) fclean
