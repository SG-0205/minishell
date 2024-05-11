# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    custom.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/17 15:11:35 by sgoldenb          #+#    #+#              #
#    Updated: 2024/04/17 16:47:03 by sgoldenb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLACK_FG = "\033[30m"
RED_FG = "\033[31m"
GREEN_FG = "\033[32m"
YELLOW_FG = "\033[33m"
BLUE_FG = "\033[34m"
MAGENTA_FG = "\033[35m"
CYAN_FG = "\033[36m"
WHITE_FG = "\033[37m"
RESET_FG = "\033[39m"

# Couleur d'arrière-plan
BLACK_BG = "\033[40m"
RED_BG = "\033[41m"
GREEN_BG = "\033[42m"
YELLOW_BG = "\033[43m"
BLUE_BG = "\033[44m"
MAGENTA_BG = "\033[45m"
CYAN_BG = "\033[46m"
WHITE_BG = "\033[47m"
RESET_BG = "\033[49m"

BOLD    = "\033[1m"
DIM     = "\033[2m"
ITALIC  = "\033[3m"
UNDERLINE = "\033[4m"
BLINK   = "\033[5m"
INVERT  = "\033[7m"
HIDE    = "\033[8m"
RESET   = "\033[0m"


HEADER_MESSAGE = "$(YELLOW_BG)$(BLACK_FG)"
ERROR_MESSAGE = "$(RED_FG)$(BOLD)[ERROR] : $(RESET_FG)$(RESET)"
SUCCESS_MESSAGE = "$(GREEN_FG)$(BOLD)[SUCCESS] : $(RESET_FG)$(RESET)"
INFO_MESSAGE = "$(YELLOW_FG)$(ITALIC)[INFO] : $(RESET_FG)$(RESET)"
COMP_KO = "$(RED_FG)$(BOLD)[KO]$(RESET_FG)$(RESET)"
COMP_OK = "$(GREEN_FG)$(BOLD)[OK]$(RESET_FG)$(RESET)"
