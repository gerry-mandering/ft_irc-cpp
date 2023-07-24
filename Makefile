CXXFLAGS := -Wall -Wextra -Werror -std=c++98 $(ASAN)
NAME := ircserver

SRCS := $(wildcard *.cpp)
DRIVER := main.cpp \
			test_shared_ptr.cpp 

SRCS := $(filter-out $(DRIVER) $(VAR),$(SRCS))
OBJS := $(SRCS:.cpp=.o)

all: $(NAME)

# $(NAME): $(OBJS) main.cpp
# 	$(LINK.cc) $^ -o $(NAME)

$(NAME): $(OBJS) main.o
	$(CXX) $(CXXFLAGS) -o $(NAME) $?

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test_shared_ptr: $(OBJS) test_shared_ptr.o
	$(CXX) $(CXXFLAGS) -o test_shared_ptr $?

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) test_shared_ptr

re:
	make fclean
	make all

.PHONY: clean fclean re all