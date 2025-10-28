CXX = g++
CXXFLAGS = -std=c++17 -I/usr/local/include -g -Wall
LDFLAGS = -L/usr/local/lib -lCppUTest -lCppUTestExt

SRCS = temp_conversion_tdd.cpp

TARGET = runTests

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
