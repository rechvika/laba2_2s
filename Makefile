CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
TEST_CXXFLAGS = -Wall -Wextra -std=c++17 -g -DTESTING_MODE
INCLUDES = -Iinclude
LDFLAGS = -L/usr/local/lib -lncursesw -lftxui-component -lftxui-dom -lftxui-screen
GTEST_LDFLAGS = -lgtest -lgtest_main -pthread

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/app

TESTS_DIR = tests
TESTS_BUILD_DIR = $(BUILD_DIR)/tests
TEST_TARGET = $(TESTS_BUILD_DIR)/run_tests

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

TEST_SOURCES = $(wildcard $(TESTS_DIR)/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTS_DIR)/%.cpp=$(TESTS_BUILD_DIR)/%.o)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TESTS_BUILD_DIR)/%.o: $(TESTS_DIR)/%.cpp
	@mkdir -p $(TESTS_BUILD_DIR)
	$(CXX) $(TEST_CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_TARGET): $(TEST_OBJECTS)
	@mkdir -p $(TESTS_BUILD_DIR)
	$(CXX) $(TEST_OBJECTS) -o $(TEST_TARGET) $(GTEST_LDFLAGS)

tests: $(TEST_TARGET)
	@echo "================== ЗАПУСК ТЕСТОВ =================="
	@./$(TEST_TARGET)
	@echo "================== ТЕСТЫ ЗАВЕРШЕНЫ =================="

run: $(TARGET)
	@xterm -fa 'Monospace' -fs 14 -geometry 150x50 -title "Sequence Manager" -e bash -c "$(TARGET); echo 'Нажмите Enter'; read"

clean:
	rm -rf $(BUILD_DIR)

.PHONY: run clean tests