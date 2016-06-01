#.SUFFIXES	: .c .cpp .o
SRC_DIR=src
INC_DIR=include
OBJ_DIR=obj
LIB_DIR=lib
TEST_DIR=test

#Lib
CONFIG_LIB=Config

#make
all	: libdir config testpg

libdir	: 
	mkdir -p $(LIB_DIR)
	mkdir -p $(OBJ_DIR)
	
config	:
	g++ -c $(SRC_DIR)/ConfigManager.cpp -I$(INC_DIR)
	mv ConfigManager.o $(OBJ_DIR)
	ar rv $(LIB_DIR)/lib$(CONFIG_LIB).a $(OBJ_DIR)/ConfigManager.o

testpg	:
	g++ -o $(TEST_DIR)/ConfigTest $(TEST_DIR)/ConfigTest.cpp -I$(INC_DIR) -L$(LIB_DIR) -l$(CONFIG_LIB)

#.PHONY : clean	
clean	: 
	rm -rf $(OBJ_DIR)/*
	rm -rf $(LIB_DIR)/*