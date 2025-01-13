NAME=ex00
SRC= $(wildcard src/**/*.cpp) $(wildcard src/*.cpp)
CXX= c++
CXXFLAGS= -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all