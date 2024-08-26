NAME = gnl.a
SOURCES = get_next_line_utils.c get_next_line.c
HEADER = get_next_line.h 
OBJS = $(SOURCES:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	ar -r $(NAME) $(OBJS)

clean: 
	$(RM) $(OBJS)

fclean: clean 
	$(RM) $(NAME)

re: fclean all
