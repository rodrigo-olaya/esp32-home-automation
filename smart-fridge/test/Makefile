CXX = g++
CXXFLAGS = -std=c++17 -I/usr/local/include -g -Wall -Wno-deprecated
LDFLAGS = -L/usr/local/lib -lCppUTest -lCppUTestExt

SRCS = main_tdd.cpp temp_conversion_tdd.cpp humidity_conversion_tdd.cpp ../main/data/handle_temperature.c ../main/data/handle_humidity.c calculateSteps_tdd.cpp ../main/motors/handlers/calculate_steps.c

TARGET = runTests

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)