## EPITECH PROJECT, 2023
## B-YEP-400-LYN-4-1-zappy-alexandre.douard
## File description:
## Makefile
##

CC = gcc

CFLAGS = -Wall -Wextra -Werror -O2 -I../tools/includes/ -Iincludes/network -g
DBGFLAGS	=	-g3 -O0
DEPSFLAGS	=	-MMD -MP

LIBNAME = libnetwork.a

BUILD_DIR = build

SRC =	message_queue.c	\
		socket.c		\
		server.c		\
		select.c		\
		client.c		\
		buffer_queue.c	\

DEPS	=	$(patsubst %.c,$(BUILD_DIR)/%.d,$(SRC))

OBJS	=	$(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))

all:
	@$(MAKE) $(BUILD_DIR)
	@$(MAKE) $(LIBNAME)
	cp $(LIBNAME) ..
dbg:
	@$(MAKE) $(BUILD_DIR)
	CFLAGS += $(DBGFLAGS)
	$(LIBNAME)
dbgs:
	@$(MAKE) $(BUILD_DIR)
	CFLAGS += $(DBGFLAGS) -fsanitize=address
	$(LIBNAME)

-include $(DEPS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIBNAME): $(OBJS)
	ar rcs $@ $(OBJS)

$(BUILD_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(DEPSFLAGS) $< -o $@

vpath %.c sources/

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -rf $(LIBNAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

redbg:
	@$(MAKE) fclean
	@$(MAKE) dbg

.PHONY: all dbg clean fclean re redbg
