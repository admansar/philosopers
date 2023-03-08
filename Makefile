NAME    = philo

SRC     = philo.c

OBJ     = ${SRC:.c=.o}

CC      = cc

CFLAGS  = -Wall -Wextra -Werror

RED     = echo "\033[0;31m"

YELLOW  = echo "\033[0;33m"

GREEN   = echo "\033[0;32m"

BLUE    = echo "\033[0;34m"

CYAN    = echo "\033[0;36m"

END     = echo "\033[0m"


all : $(NAME)

$(NAME) : $(OBJ)
	@$(GREEN)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
	@$(END)

blue : 
	@$(BLUE)

clean : 
	@$(YELLOW)
	rm -f $(OBJ)
	@$(END)

fclean : clean
	@$(RED)
	rm -rf $(NAME)
	@$(END)


re : fclean all
	@$(CYAN)
	@echo "	       | |   (_) | "
	@echo "	  _ __ | |__  _| | ___ "
	@echo "	 |  _ \|  _ \| | |/ _ \ "
	@echo "	 | |_) | | | | | | (_) | "
	@echo "	 | .__/|_| |_|_|_|\___/ "
	@echo "	 | | "
	@echo "	 |_|         by admansar"
	@$(END)
