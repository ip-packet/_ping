CC = cc
SRC = $(wildcard src/*.c)
UID = $(shell id -u)
HR = $(wildcard include/*.h)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
NAME = _ping

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

install:
	@if [ $(UID) -eq 0 ]; then\
		if [ -e $(shell pwd)/_ping ]; then\
			cp _ping /bin/;\
			chmod u+s /bin/_ping;\
		else\
			echo "error: need to \"make\" first";\
		fi;\
	else\
		echo "error: need root privileges to install";\
	fi


build/%.o: src/%.c $(HR)
	@mkdir -p $(dir $@)
	$(CC) -Iinclude -c $< -o $@

clean:
	rm -rf build

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean
