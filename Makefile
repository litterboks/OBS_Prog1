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

zip: clean
	        zip -r OBS_Prog1_if14b062_if14b049.zip . -x .git/ -x .gitignore -x .git/**\* -x *.pdf -x *.in -x inputs/


run: all
	./${EXECUTABLE}
