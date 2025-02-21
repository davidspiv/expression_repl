#https://github.com/gwu-cs-os/evening_os_hour/blob/master/f19/10.2-makefiles/03_featureful_makefile/Makefile

# Output binary name
EXE_NAME=$(OBJ_DIR)/main.out
TEST_EXE_NAME=$(OBJ_DIR)/test.out

# Directories
SRC_DIR=src
HEADER_DIR=include
OBJ_DIR=build
TEST_SRC_DIR=test

# Compiler and flags
CC=g++
OPT=-O0
DEP_FLAGS=-MP -MD
C_FLAGS=-c -g -Wall -std=c++17 -fpermissive -I$(HEADER_DIR) $(OPT) $(DEP_FLAGS)
LD_FLAGS=

# Primary binary sources and objects
CPP_FILES=$(wildcard $(SRC_DIR)/*.cpp)
O_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_FILES))
DEP_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(CPP_FILES))

# Exclude main.cpp from source files when building the test binary
SRC_WITHOUT_MAIN=$(filter-out $(SRC_DIR)/main.cpp, $(CPP_FILES))
SRC_O_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_WITHOUT_MAIN))

# Test binary sources and objects
TEST_CPP_FILES=$(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_O_FILES=$(patsubst $(TEST_SRC_DIR)/%.cpp,$(OBJ_DIR)/test_%.o,$(TEST_CPP_FILES))
TEST_DEP_FILES=$(patsubst $(TEST_SRC_DIR)/%.cpp,$(OBJ_DIR)/test_%.d,$(TEST_CPP_FILES))

# Ensure we don't run with empty source lists
ifneq ($(CPP_FILES),)
    HAVE_SOURCE=1
endif

ifneq ($(TEST_CPP_FILES),)
    HAVE_TEST_SOURCE=1
endif

# Default build target
all: $(EXE_NAME)

# Build the primary binary
$(EXE_NAME): $(O_FILES)
	$(CXX) $(LD_FLAGS) -o $@ $^

# Build the test binary (linking test and src object files, excluding main.cpp)
test: $(TEST_EXE_NAME)

$(TEST_EXE_NAME): $(TEST_O_FILES) $(SRC_O_FILES)  # Link with source object files (excluding main)
	$(CXX) $(LD_FLAGS) -o $@ $^

# Compile source object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(C_FLAGS) -o $@ $<

# Compile test object files
$(OBJ_DIR)/test_%.o: $(TEST_SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(C_FLAGS) -o $@ $<

# Include dependencies
ifneq ($(HAVE_SOURCE),)
    -include $(DEP_FILES)
endif

ifneq ($(HAVE_TEST_SOURCE),)
    -include $(TEST_DEP_FILES)
endif

# Run make test and execute the test binary
dev: test
	@./$(TEST_EXE_NAME)

# Clean generated files
clean:
	$(RM) -rf $(BIN) $(TEST_BIN) $(OBJ_DIR)

# Include dependency files if available
ifneq ($(HAVE_SOURCE),)
    -include $(DEP_FILES)
endif

ifneq ($(HAVE_TEST_SOURCE),)
    -include $(TEST_DEP_FILES)
endif

# Ensure these targets aren't mistaken for files
.PHONY: all clean test
