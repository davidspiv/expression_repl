# Output binary name
MAIN_BIN=$(OBJ_DIR)/main.out
TEST_BIN=$(OBJ_DIR)/test.out

# Directories
SRC_DIR=src
TEST_DIR=test
HEADER_DIR=include
OBJ_DIR=build

# Compiler and flags
CXX=g++
OPT=-O0
DEP_FLAGS=-MP -MD
C_FLAGS_DEV=-c -g -Wall -std=c++17 -fpermissive -I$(HEADER_DIR) $(OPT) $(DEP_FLAGS)
C_FLAGS_RELEASE=-c -std=c++17 -O3 -Wall -DNDEBUG -I$(HEADER_DIR) $(OPT) $(DEP_FLAGS)
LD_FLAGS=

# Primary binary sources and objects
CPP_FILES=$(wildcard $(SRC_DIR)/*.cpp)
O_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/release_%.o,$(CPP_FILES))
DEP_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(CPP_FILES))

# Test binary sources and objects
TEST_CPP_FILES=$(wildcard $(TEST_DIR)/*.cpp)
TEST_O_FILES=$(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/release_%.o,$(TEST_CPP_FILES))
SRC_WITHOUT_MAIN=$(filter-out $(SRC_DIR)/main.cpp, $(CPP_FILES))
TEST_SRC_O_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/debug_%.o,$(SRC_WITHOUT_MAIN))
TEST_DEP_FILES=$(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/debug_%.d,$(TEST_CPP_FILES))

# Ensure we don't run with empty source lists
ifneq ($(CPP_FILES),)
    HAVE_SOURCE=1
endif

ifneq ($(TEST_CPP_FILES),)
    HAVE_TEST_SOURCE=1
endif

# Default build target
all: $(MAIN_BIN)

# Build the primary binary
$(MAIN_BIN): $(O_FILES)
	$(CXX) $(LD_FLAGS) -o $@ $^

# Compile source object files (Release Mode)
$(OBJ_DIR)/release_%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(C_FLAGS_RELEASE) -o $@ $<

# Include dependencies
ifneq ($(HAVE_SOURCE),)
    -include $(DEP_FILES)
endif

run: all
	@./$(MAIN_BIN)

# Build the test binary (compiles both src and test files with debug flags)
test: $(TEST_BIN)

$(TEST_BIN): $(TEST_O_FILES) $(TEST_SRC_O_FILES)
	$(CXX) $(LD_FLAGS) -o $@ $^

# Compile all source files needed for test with debug flags
$(OBJ_DIR)/debug_%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(C_FLAGS_DEV) -o $@ $<

# Compile test files with debug flags
$(OBJ_DIR)/release_%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(C_FLAGS_RELEASE) -o $@ $<

# Include test dependencies
ifneq ($(HAVE_TEST_SOURCE),)
    -include $(TEST_DEP_FILES)
endif

# Run test binary after building
dev: test
	@./$(TEST_BIN)

# Clean generated files
clean:
	$(RM) -rf $(TEST_BIN) $(OBJ_DIR)

# Ensure these targets aren't mistaken for files
.PHONY: test dev clean
