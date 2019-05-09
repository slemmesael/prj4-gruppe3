# -------------------------------------- #
# Main Makefile
#
# @author	PRJ4 - Gruppe 3
# @date		Forår 2019
# -------------------------------------- #

# -------------------------------------- #
# Args
# -------------------------------------- #
# Validate TARGET
ifeq ($(TARGET),$(filter $(TARGET),host rpi))
TARGET=host
endif

# Making for host
# > make TARGET=host
ifeq ($(TARGET),host)
CXX				=g++
AR				=ar
OS				=OS_LINUX
endif

# Making for RPI
# > make TARGET=rpi
ifeq ($(TARGET),rpi)
CXX				=arm-rpizw-g++
AR				=arm-poky-linux-gnueabi-ar
OS				=OS_LINUX
endif

# Making for release (default)
# > make
CXXFLAGS		=-O2 -I. -Isrc -std=c++1z
MODE			=release

# Making for debugging
# > make DEBUG=1
ifdef DEBUG
CXXFLAGS		=-ggdb3 -I. -Isrc -std=c++1z
MODE			=debug
endif

# -------------------------------------- #
# Variables
# -------------------------------------- #
# Executables
EXE				=main

# Paths
SRC_PATH		=src
BUILD_DIR		=build
BIN_DIR			=bin
BUILD_PATH		=$(BUILD_DIR)/$(TARGET)/$(MODE)
BIN_PATH		=$(BIN_DIR)/$(TARGET)/$(MODE)


JSON_PATH		=lib/json
JSONLIB_PATH 		=$(JSON_PATH)/lib/$(TARGET)/$(MODE)

# Files
SRC				=$(shell find $(SRC_PATH) -name '*.cpp' | sort -k 1nr | cut -f2-)
DEPS			=$(addprefix ${BUILD_PATH}/, $(notdir ${SRC:%.cpp=%.d}))
OBJS			=$(addprefix ${BUILD_PATH}/, $(notdir ${SRC:%.cpp=%.o}))

# Compiler flags
CXXFLAGS		+= -Wall -Iinc -D_REENTRANT -D$(OS) -I$(JSON_PATH)/inc
LDFLAGS			=

#add JsonBox
#LDFLAGS			+= -L$(JSONLIB_PATH) -lJsonBox -lpthread -lrt

#add json
LDFLAGS			+= -L$(JSONLIB_PATH) -lpthread -lrt 
#-ljson

VPATH			=$(SRC_PATH) $(SRC_PATH)/Web

# -------------------------------------- #
# Misc
# -------------------------------------- #

# Prevents potential invalid targets
.PHONY: all clean run

# Default target
all: info ${BIN_PATH}/${EXE}

# Display compilation info
info:
	@ echo "Dependencies: ${DEPS}"
	@ echo "Compiling for '${TARGET}' in '${MODE}' mode..."

# -------------------------------------- #
# Folders
# -------------------------------------- #
${BIN_PATH}:
	@ echo "Creating directory '${BIN_PATH}'"
	@ mkdir -p $(BIN_PATH)

${BUILD_PATH}:
	@ echo "Creating directory '${BUILD_PATH}'"
	@ mkdir -p $(BUILD_PATH)

# -------------------------------------- #
# Dependencies
# -------------------------------------- #
# Create dependencies with the proper prerequisites.
${BUILD_PATH}/%.d: $(BUILD_PATH) %.cpp
	@ echo "Generating dependency file '$@' using '$^'"
	@ $(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $(filter-out $<, $^) > $@

# -------------------------------------- #
# Objects
# -------------------------------------- #
# Compile all object files from the imported dependencies
${BUILD_PATH}/%.o: %.cpp
	@ echo "Compiling object file '$@' using '$<'"
	@ ${CXX} -c $< -o $@ ${CXXFLAGS}

# -------------------------------------- #
# Libs
# -------------------------------------- #
# Target for building missing libraries

${JSONLIB}:
	@ echo "Building json for '${TARGET}' in '${DEBUG}' mode."
	@ (cd $(JSON_PATH) && make TARGET=$(TARGET) DEBUG=$(DEBUG))


# -------------------------------------- #
# Makefile runnables
# -------------------------------------- #
# Linking main executable
${BIN_PATH}/${EXE}: $(BIN_PATH) $(BUILD_PATH) $(DEPS) $(OBJS) $(JSONLIB)
	@ echo "Linking '${BIN_PATH}/${EXE}'"
	@ ${CXX} -o $@ $(OBJS) $(LDFLAGS)

# Clean all object files and the main target
clean:
	@ echo "Cleaning up..."
	@ rm -rf $(BUILD_PATH)
	@ rm -rf $(BIN_PATH)

# Runs the main target executable
run:
	@ ./${BIN_PATH}/${EXE}

# Include all dependencies
ifeq (,$(filter $(MAKECMDGOALS),clean run))
-include $(DEPS)
endif
