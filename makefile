# Output binary names
RELEASE_BIN=$(OBJ_DIR)/release.out
DEBUG_BIN=$(OBJ_DIR)/debug.out
TESTER_BIN=$(OBJ_DIR)/test.out

# Directories
SRC_DIR=src
TESTER_DIR=test
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
DEBUG_O_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/debug_%.o,$(CPP_FILES))
RELEASE_O_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/release_%.o,$(CPP_FILES))
DEBUG_DEP_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/debug_%.d,$(CPP_FILES))
RELEASE_DEP_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/release_%.d,$(CPP_FILES))

# Remove main from src to avoid conflicts in test build
SRC_WITHOUT_MAIN=$(filter-out $(SRC_DIR)/main.cpp, $(CPP_FILES))

# Test binary sources and objects, compiling testers intentionally in release mode for speed
TESTER_CPP_FILES=$(wildcard $(TESTER_DIR)/*.cpp)
TESTER_O_FILES=$(patsubst $(TESTER_DIR)/%.cpp,$(OBJ_DIR)/release_%.o,$(TESTER_CPP_FILES))
TESTER_SRC_O_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/debug_%.o,$(SRC_WITHOUT_MAIN))
TESTER_DEP_FILES=$(patsubst $(TESTER_DIR)/%.cpp,$(OBJ_DIR)/release_%.d,$(TESTER_CPP_FILES))

# Check for empty source lists
ifneq ($(CPP_FILES),)
    HAVE_SOURCE=1
endif

ifneq ($(TESTER_CPP_FILES),)
    HAVE_TESTER_SOURCE=1
endif

# Debug Build
all: $(DEBUG_BIN)

# Build the debug binary
$(DEBUG_BIN): $(DEBUG_O_FILES)
	$(CXX) $(LD_FLAGS) -o $@ $^

# Compile source object files (Debug Mode)
$(OBJ_DIR)/debug_%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(C_FLAGS_DEV) -o $@ $<

# Include dependencies
ifneq ($(HAVE_SOURCE),)
    -include $(DEP_FILES)
endif

run: all
	@./$(DEBUG_BIN)

# Release Build
release: $(RELEASE_BIN)

# Build the release binary
$(RELEASE_BIN): $(RELEASE_O_FILES)
	$(CXX) $(LD_FLAGS) -o $@ $^

# Compile source object files (Release Mode)
$(OBJ_DIR)/release_%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(C_FLAGS_RELEASE) -o $@ $<

# Include dependencies
ifneq ($(HAVE_SOURCE),)
    -include $(DEP_FILES)
endif

run_release: release
	@./$(RELEASE_BIN)

# Test Build
test: $(TESTER_BIN)

$(TESTER_BIN): $(TESTER_O_FILES) $(TESTER_SRC_O_FILES)
	$(CXX) $(LD_FLAGS) -o $@ $^

# Compile all source files needed for test with debug flags
$(OBJ_DIR)/debug_%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(C_FLAGS_DEV) -o $@ $<

# Compile tester files with release flags
$(OBJ_DIR)/release_%.o: $(TESTER_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(C_FLAGS_DEV) -o $@ $<

# Include test dependencies
ifneq ($(HAVE_TESTER_SOURCE),)
    -include $(TESTER_DEP_FILES)
endif

# Run test binary after building
dev: test
	@./$(TESTER_BIN)

# Clean generated files
clean:
	$(RM) -rf $(TESTER_BIN) $(OBJ_DIR)

# Ensure these targets aren't mistaken for files
.PHONY: test dev clean run all release run_release
