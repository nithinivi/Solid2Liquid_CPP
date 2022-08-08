CC = g++ 
CFLAGS=-I -Wall

TARGET = my_visitor

all: $(TARGET)

$(TARGET): visitors.o expression.o parser.o main.o 
	$(CC) $(CFLAGS) main.o expression.o  parser.o visitors.o -o $(TARGET)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp


expression.o:  expression.cpp expression.hpp
	$(CC) $(CFLAGS) -c expression.cpp

parser.o: parser.cpp parser.hpp
	$(CC) $(CFLAGS) -c parser.cpp

visitors.o: visitors.cpp visitor.hpp
	$(CC) $(CFLAGS) -c visitors.cpp

clean: 
	$(RM) count *.o *~