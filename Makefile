# Makefile for your project

CC = gcc
CFLAGS = -Wall -Wextra -g # Add warnings for better code quality
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Define the program to build
PROG = mygame

# Source files
SRCS = main.c resources/other_funcs/cube.c resources/other_funcs/CA1d_io.c resources/other_funcs/stl_handling.c

# Rule to build the program
$(PROG): $(SRCS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# Cleaning rule
clean:
	rm -f $(PROG)


# Add more specific targets as needed if you have other code
# Example:
#
# cube: resources/other_funcs/cube.o
# 	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

# Additional targets (optional):

# If you have a test file:
# test:
#	./$(PROG)


# You will likely want to specify include directories
# if the headers are not in the standard locations
# example
#CFLAGS += -I/path/to/include/directory