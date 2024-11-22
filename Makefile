##
## EPITECH PROJECT, 2023
## B-YEP-400-LYN-4-1-zappy-alexandre.douard
## File description:
## Makefile
##

all:
	@$(MAKE) -C tools
	@$(MAKE) -C network

dbg:
	@$(MAKE) dbg -C tools
	@$(MAKE) dbg -C network

clean:
	@$(MAKE) clean -C tools
	@$(MAKE) clean -C network

fclean:
	@$(MAKE) fclean -C tools
	@$(MAKE) fclean -C network
	rm -rf lib
	rm -f $(wildcard *.a)

re:
	@$(MAKE) re -C tools
	@$(MAKE) re -C network

redbg:
	@$(MAKE) redbg -C tools
	@$(MAKE) redbg -C network

.PHONY: all dbg clean fclean re redbg
