NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

SRC_DIR		= src
INC_DIR		= include
OBJ_DIR		= obj

SRC			= main.c \
			  parse_args.c \
			  sim_init.c \
			  cleanup.c \
			  simulation.c \
			  dongle.c \
			  dongle_queue.c \
			  coder.c \
			  coder_stats.c \
			  monitor.c \
			  logger.c \
			  utils.c \
			  heap.c

OBJ			= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus