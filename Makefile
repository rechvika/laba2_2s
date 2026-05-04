CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
INCLUDES = -Iinclude
LDFLAGS = -L/usr/local/lib -lncursesw -lftxui-component -lftxui-dom -lftxui-screen

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/app

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	@xterm -fa 'Monospace' -fs 14 -geometry 150x50 -title "Sequence Manager" -e bash -c "$(TARGET); echo 'Нажмите Enter'; read"

rebuild: clean $(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: run rebuild clean