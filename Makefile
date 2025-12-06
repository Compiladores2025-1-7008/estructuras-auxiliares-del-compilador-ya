CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./src -I./external/googletest/googletest/include -I./external/googletest/googletest
LDFLAGS = -pthread

SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/%.o)

GTEST_DIR = external/googletest/googletest
GTEST_SRCS = $(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc
GTEST_OBJS = $(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/gtest_main.o

TARGET_TEST = runTests

.PHONY: all test clean setup_gtest

all: test

# -------------------------
# Clone GoogleTest if needed
# -------------------------
setup_gtest:
	@if [ ! -d "external/googletest" ]; then \
		echo "Clonando GoogleTest..."; \
		mkdir -p external; \
		cd external; \
		git clone https://github.com/google/googletest.git; \
	fi
	@mkdir -p $(BUILD_DIR)

# -------------------------
# Build GoogleTest Objects
# -------------------------
$(BUILD_DIR)/gtest-all.o: setup_gtest
	$(CXX) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc -o $@

$(BUILD_DIR)/gtest_main.o: setup_gtest
	$(CXX) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc -o $@

# -------------------------
# Build Tests
# -------------------------
test: $(TARGET_TEST)
	./$(TARGET_TEST)

$(TARGET_TEST): $(GTEST_OBJS) $(OBJS) $(TEST_OBJS)
	$(CXX) -o $@ $(OBJS) $(TEST_OBJS) $(GTEST_OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# -------------------------
# Clean
# -------------------------
clean:
	rm -rf $(BUILD_DIR) $(TARGET_TEST)
