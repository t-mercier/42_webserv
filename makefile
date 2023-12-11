NAME		=		webserv
SRC			=		src
OUT 		= 		out
SRCS		=		$(wildcard $(SRC)/*.cpp)
OBJS 		= 		$(patsubst $(SRC)/%.cpp, $(OUT)/%.o, $(SRCS))
CXXFLAGS 	+= 		-std=c++17
LFLAGS		+=		-lm

OS := $(shell uname -s)

ifndef DEV
CXXFLAGS	+=		-Ofast -Wextra -Wall -Wextra
else
CXXFLAGS    +=		
endif

ifeq ($(OS),Linux)
CFLAGS 		+= 		-DLINUX
endif

$(shell mkdir -p $(OUT))

all: $(NAME)

run: $(NAME)
	./webserv public/configfile.conf 

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LFLAGS)

$(OUT)/%.o: $(SRC)/%.cpp 
	@printf "$(GREEN)Compiling: $(RESET)$*.c\n"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

GREEN			:=	\033[1;32m
LIGHT_RED		:=	\033[1;31m
LIGHT_CYAN		:=	\033[1;36m
RESET			:= \033[0m

.PHONY: all clean fclean re
