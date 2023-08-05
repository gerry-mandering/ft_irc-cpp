CXXFLAGS = -Wall -Wextra -Werror -std=c++98 $(INCLUDE_PATH) $(ASAN)
# TODO: 플래그 사용하기!!!!!!!!!!!!!!!!!!

# CXXFLAGS = -std=c++98 $(INCLUDE_PATH) $(ASAN)

VALID_LOG_LEVELS := TRACE DEBUG INFO WARN ERROR
LOG_VALUE := $(filter $(LOG),$(VALID_LOG_LEVELS))

ifeq ($(LOG_VALUE),)
    LOG_VALUE := OFF
endif

# $(info $$LOG_VALUE is [${LOG_VALUE}])
CXXFLAGS += -DLOGGING -D LOG_LEVEL=LEVEL_$(LOG_VALUE)

NAME = ircserver

DIR_INCLUDES = includes
DIR_SRCS = srcs

DIR_CHANNEL = channel
DIR_CHANNEL_REPOSITORY = channel_repository
DIR_CLIENT = client
DIR_CLIENT_REPOSITORY = client_repository
DIR_ENV_MANGER = env_manager
DIR_EXCEPTION = exception
DIR_EXECUTOR = executor
DIR_PARSER = parser
DIR_REACTOR = reactor
DIR_REQUEST = request
DIR_UTILS = utils
DIR_VALIDATOR = validator
DIR_VISITOR = visitor_pattern
DIR_LOGGER = logger


DRIVER_SRC := $(DIR_SRCS)/main.cpp
DRIVER_OBJ := $(DRIVER_SRC:.cpp=.o)

DIR_TESTS = tests
TEST_FILENAMES = test_shared_ptr \
					test_parser \
					test_logger \
					
TEST_SRCS := $(addprefix $(DIR_SRCS)/$(DIR_TESTS)/, $(addsuffix .cpp, $(TEST_FILENAMES)))
TEST_OBJS := $(TEST_SRCS:.cpp=.o)


# AS IS
#SRCS := $(wildcard $(DIR_SRCS)/$(DIR_REQUEST)/*.cpp $(DIR_SRCS)/$(DIR_REACTOR)/*.cpp $(DIR_SRCS)/$(DIR_PARSER)/*.cpp)

# TO BE
 SRCS := $(wildcard $(DIR_SRCS)/$(DIR_CHANNEL)/*.cpp \
  					$(DIR_SRCS)/$(DIR_CHANNEL_REPOSITORY)/*.cpp \
  					$(DIR_SRCS)/$(DIR_CLIENT)/*.cpp \
  					$(DIR_SRCS)/$(DIR_CLIENT_REPOSITORY)/*.cpp \
  					$(DIR_SRCS)/$(DIR_ENV_MANGER)/*.cpp \
  					$(DIR_SRCS)/$(DIR_EXCEPTION)/*.cpp \
  					$(DIR_SRCS)/$(DIR_EXECUTOR)/*.cpp \
  					$(DIR_SRCS)/$(DIR_REACTOR)/*.cpp \
  					$(DIR_SRCS)/$(DIR_REQUEST)/*.cpp \
  					$(DIR_SRCS)/$(DIR_UTILS)/*.cpp \
  					$(DIR_SRCS)/$(DIR_VALIDATOR)/*.cpp \
  					$(DIR_SRCS)/$(DIR_VISITOR)/*.cpp \
 					$(DIR_SRCS)/$(DIR_PARSER)/*.cpp \
 					$(DIR_SRCS)/$(DIR_LOGGER)/*.cpp) \

# SRCS := $(filter-out $(DRIVER) $(VAR),$(SRCS))
OBJS := $(SRCS:.cpp=.o)

# INCLUDE_PATH = $(addprefix -I, $(DIR_INCLUDES))
INCLUDE_PATH := $(addprefix -I, $(DIR_INCLUDES) \
								$(DIR_INCLUDES)/$(DIR_CHANNEL) \
								$(DIR_INCLUDES)/$(DIR_CHANNEL_REPOSITORY) \
								$(DIR_INCLUDES)/$(DIR_CLIENT) \
								$(DIR_INCLUDES)/$(DIR_CLIENT_REPOSITORY) \
								$(DIR_INCLUDES)/$(DIR_ENV_MANGER) \
								$(DIR_INCLUDES)/$(DIR_EXCEPTION) \
								$(DIR_INCLUDES)/$(DIR_EXECUTOR) \
								$(DIR_INCLUDES)/$(DIR_PARSER) \
								$(DIR_INCLUDES)/$(DIR_REACTOR) \
								$(DIR_INCLUDES)/$(DIR_REQUEST) \
								$(DIR_INCLUDES)/$(DIR_UTILS) \
								$(DIR_INCLUDES)/$(DIR_VALIDATOR) \
								$(DIR_INCLUDES)/$(DIR_VISITOR) \
								$(DIR_INCLUDES)/$(DIR_LOGGER)) \

all: $(NAME)

# $(NAME): $(OBJS) main.cpp
# 	$(LINK.cc) $^ -o $(NAME)

$(NAME): $(OBJS) $(DRIVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test_shared_ptr: $(OBJS) $(DIR_SRCS)/$(DIR_TESTS)/test_shared_ptr.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test_parser: $(OBJS) $(DIR_SRCS)/$(DIR_TESTS)/test_parser.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test_logger: $(OBJS) $(DIR_SRCS)/$(DIR_TESTS)/test_logger.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) $(OBJS) $(DRIVER_OBJ) $(TEST_OBJS)

fclean: clean
	$(RM) $(NAME) $(TEST_FILENAMES)

re:
	make fclean
	make all

.PHONY: clean fclean re all