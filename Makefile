NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
DFLAGS = -g3 -fsanitize=address

SRCS_DIR = src
OBJS_DIR = objs

SRCS = $(shell find $(SRCS_DIR) -name "*.cpp")
OBJS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

INCLUDES = -I$(SRCS_DIR) \
          -I$(SRCS_DIR)/server \
          -I$(SRCS_DIR)/network \
          -I$(SRCS_DIR)/channel \
          -I$(SRCS_DIR)/user

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

debug: CXXFLAGS += $(DFLAGS)
debug: re

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ -MMD

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug

-include $(DEPS)