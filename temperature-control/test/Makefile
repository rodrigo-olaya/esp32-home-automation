# Host-side unit tests. The firmware sources are compiled as C with the C compiler, and the
# CppUTest test files as C++ with g++. Only the final link uses g++. That way the C stays C,
# and a header that only works as C++ (or needs an ESP-IDF include) fails here instead of
# in the firmware build.
CC  ?= cc
CXX ?= g++

CFLAGS   = -std=c11 -g -Wall -Wextra -MMD -MP
CXXFLAGS = -std=c++17 -I/usr/local/include -g -Wall -Wno-deprecated -MMD -MP
LDFLAGS  = -L/usr/local/lib -lCppUTest -lCppUTestExt

# Test files (C++)
CPP_SRCS = \
	main_tdd.cpp \
	temp_conversion_tdd.cpp \
	humidity_conversion_tdd.cpp \
	discovery_tdd.cpp \
	crc_tdd.cpp \
	discovery_step_tdd.cpp \
	topic_tdd.cpp

# Firmware sources under test (C), paths relative to ../main
C_SRCS = \
	data/handle_temperature.c \
	data/handle_humidity.c \
	discovery/discovery.c \
	data/crc.c \
	MQTT/topic.c

OBJDIR = obj
OBJS = $(CPP_SRCS:%.cpp=$(OBJDIR)/%.o) $(C_SRCS:%.c=$(OBJDIR)/%.o)

TARGET = runTests

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: ../main/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

-include $(OBJS:.o=.d)

.PHONY: all test clean
