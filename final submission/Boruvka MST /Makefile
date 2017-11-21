CC=g++
DEBUG=-g
INCLUDE_DIR=include
SRC_DIR=src
OBJ_DIR=object
CPP_STANDARD=c++11
CFLAGS=-c -pthread -std=$(CPP_STANDARD) -I$(INCLUDE_DIR)/
LFLAGS=-pthread -std=$(CPP_STANDARD) -I$(INCLUDE_DIR)/ 
OBJS= $(OBJ_DIR)/graph.o $(OBJ_DIR)/boruvka.o Main.cpp

default : all

MST: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o MST

$(OBJ_DIR)/graph.o : $(INCLUDE_DIR)/graph.h $(SRC_DIR)/graph.cpp
	$(CC) $(CFLAGS) $(SRC_DIR)/graph.cpp -o $(OBJ_DIR)/graph.o

$(OBJ_DIR)/boruvka.o : $(INCLUDE_DIR)/graph.h $(INCLUDE_DIR)/boruvka.h $(SRC_DIR)/boruvka.cpp
	$(CC) $(CFLAGS) $(SRC_DIR)/boruvka.cpp -o $(OBJ_DIR)/boruvka.o

clean:
	rm -rf OBJ_DIR/*
	rm MST	asdf

test:
	./MST $(input)

all: MST test

# tar:
# 	tar cfv p1.tar Movie.h Movie.cpp Name.h Name.cpp NameList.h \
# 			NameList.cpp  Iterator.cpp Iterator.h


