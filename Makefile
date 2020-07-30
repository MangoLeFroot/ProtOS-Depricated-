CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -MMD -MP -ggdb

BUILD_DIR	:= build
SRC_DIR		:= src
INCLUDE_DIR	:= include
LIB_DIR		:= lib

PCH_OUT := pch.h.gch
PCH_SRC := pch.h

SRCS := $(shell find $(SRC_DIR) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

LIBRARIES	:= -lrgbmatrix -lpthread
EXECUTABLE	:= prot_os

all: $(BUILD_DIR)/$(EXECUTABLE)

run: clean all
	clear
	./$(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): $(BUILD_DIR)/$(SRC_DIR)/$(PCH_OUT) $(OBJS)
	$(CXX) $(OBJS) -L$(LIB_DIR) -o $@ $(LIBRARIES)

$(BUILD_DIR)/$(SRC_DIR)/$(PCH_OUT): $(SRC_DIR)/$(PCH_SRC)
	mkdir -p $(dir $@)
	cp $(SRC_DIR)/$(PCH_SRC) $(BUILD_DIR)/$(SRC_DIR)/$(PCH_SRC)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -include $(BUILD_DIR)/$(SRC_DIR)/$(PCH_SRC) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	-rm -r $(BUILD_DIR)/*
