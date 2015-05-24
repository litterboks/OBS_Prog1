CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu99
EXECUTABLE = myfind

############################### 
# Build
###############################

all: myfind.c
	${CC} ${CFLAGS} -o ${EXECUTABLE} $^

############################### 
# Misc
###############################

clean:
	rm -f *.zip ${EXECUTABLE}
	find . -name "*.o" -type f -delete

run: all
	./${EXECUTABLE}
