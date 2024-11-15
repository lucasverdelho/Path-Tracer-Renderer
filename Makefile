CXX      := g++ 
CXXFLAGS := -std=c++17 -O3 -mavx2 -ftree-vectorize -march=native
LDFLAGS  := 
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
SHELL    := /bin/bash

TARGET   := VI-RT

INCLUDE  := -IVI-RT/Camera/ -IVI-RT/Image -IVI-RT/Light -IVI-RT/Primitive -IVI-RT/Primitive/BRDF -IVI-RT/Primitive/Geometry -IVI-RT/Rays -IVI-RT/Renderer -IVI-RT/Scene -IVI-RT/Shader -IVI-RT/utils -IVI-RT/Scene/tinyobjloader/

SRC      :=                      \
   $(wildcard VI-RT/*.cpp) \
   $(wildcard VI-RT/Camera/*.cpp)         \
   $(wildcard VI-RT/Image/*.cpp)         \
   $(wildcard VI-RT/Primitive/*.cpp)         \
   $(wildcard VI-RT/Primitive/BRDF/*.cpp)         \
   $(wildcard VI-RT/Primitive/Geometry/*.cpp)         \
   $(wildcard VI-RT/Renderer/*.cpp)         \
   $(wildcard VI-RT/Scene/*.cpp)         \
   $(wildcard VI-RT/Shader/*.cpp)         \

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES \
		 := $(OBJECTS:.o=.d)

NUM_THREADS := 30 # Define the number of threads

all:    build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean runseq runpar

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	-@rm -rvf $(BUILD)/*
	-@rm -rvf MyImage.ppm

run:
	./$(APP_DIR)/$(TARGET)

