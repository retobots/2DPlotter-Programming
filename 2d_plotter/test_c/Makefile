# ==== Makefile ====

# Tên file đầu ra
TARGET = build/output.exe

# Compiler
CXX = g++
CXXFLAGS = -g -Wall -Iinc

# Tìm tất cả .cpp trong src/
SRC = $(wildcard src/*.cpp)

# Tạo file object tương ứng trong build/
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

# === Rule chính ===
all: $(TARGET)

# Link các object file
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile từng file .cpp thành .o
build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf build/*.o build/*.exe

run:
	./$(TARGET)

.PHONY: all clean
