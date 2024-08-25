# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/22 19:42:41 by sgoldenb          #+#    #+#              #
#    Updated: 2024/08/25 12:18:49 by sgoldenb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

test:
	gcc -Wall -Wextra -Werror -g3 src/main.c src/builtins/*.c src/expansions/*.c src/init/*.c src/parsing/*.c src/signals/*.c src/utils/*.c inc/libft/*.c inc/libft/garbage_collector/*.c src/utils/type_validators/*.c -lreadline -o test

re :
	rm test && make test