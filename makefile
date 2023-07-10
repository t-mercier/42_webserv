BIN			=		webserv
SRC			=		src

SRCS		=		main.cpp

OBJS 		= 		$(addprefix out/,$(SRCS:.cpp=.o))

CXXFLAGS 	+= 		-std=c++98
LFLAGS		+=		-lm

# ifndef DEV
# CXXFLAGS	+=		-Ofast -Wall -Wextra -Werror 
# else
# CXXFLAGS    +=		-g -fsanitize=address
# endif

$(shell mkdir -p out)

all: $(BIN)
	@printf "$(GREEN)$(BIN) ✓\n$(RESET)"

$(BIN): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

out/%.o: $(SRC)/%.cpp $(SRC)/webserv.hpp
	@printf "$(GREEN)Compiling: $(RESET)$*.c\n"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(BIN)

re: fclean all

GREEN			:=	\033[1;32m
LIGHT_RED		:=	\033[1;31m
LIGHT_CYAN		:=	\033[1;36m
RESET			:= \033[0m

.PHONY: all clean fclean re
