CXX ?= g++
CXXFLAGS ?= -std=c++17 -O0 -g --coverage
INCLUDES = -I test_support
SOURCES = tests.cpp ../snake0/Snake.cpp ../snake0/Food.cpp test_support/platform_stubs.cpp
TARGET = tests

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET)

test: $(TARGET)
	./$(TARGET)

coverage: test
	gcov -b -o . tests-Snake.gcda tests-Food.gcda

clean:
	rm -f $(TARGET) *.gcda *.gcno *.gcov *.gcov.json.gz
