CC = g++ 
CFLAGS=-I -Wall
SRC=${wildcard *.cpp }
OBJ=${patsubst %.cpp,build/%.o,${SRC}}

TARGET = my_visitor

all: ${OBJ}
	${CXX}  ${CFLAGS} ${OBJ} -o build/main ${LDFLAGS}

build/%.o : %.cpp
	mkdir -p ${dir $@}
	${CXX} -o $@ $< -c ${CFLAGS}

clean: 
	rm -rf build 
	$(RM) count  *~