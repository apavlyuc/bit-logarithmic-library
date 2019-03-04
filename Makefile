CXX := c++
CXXFLAGS := -std=c++17 -Wall -Werror -Wextra -pedantic

SRC_EXT := cc
OBJ_EXT := o

SRCS := $(shell find . -type f -name "*.$(SRC_EXT)")
OBJS := $(SRCS:.$(SRC_EXT)=.$(OBJ_EXT))

PROG := program

%.$(OBJ_EXT): %.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

all: $(PROG)

clean:
	rm -f $(OBJS) $(PROG)

re: clean all

$(PROG): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^
