#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include <string.h>

extern "C" {
    #include "../main/MQTT/topic.h"
}

/*
 * build_topic(out, out_size, location, type) -> bool  (see main/MQTT/topic.h)
 *   TEMPERATURE -> "home/esp32/<location>/temp"
 *   HUMIDITY    -> "home/esp32/<location>/humidity"
 *   returns true when the whole topic fit in out (with its NUL), false when it did not.
 */
TEST_GROUP(TopicGroup)
{
    char topic[64];

    void setup()
    {
        memset(topic, 'X', sizeof(topic));    /* 'X' means "build_topic has not written here" */
        topic[sizeof(topic) - 1] = '\0';
    }
};

// Pins down: a temperature reading goes to <location>/temp.
TEST(TopicGroup, TemperatureTopicEndsInTemp)
{
    CHECK_TRUE(build_topic(topic, sizeof(topic), "bedroom", TEMPERATURE));
    STRCMP_EQUAL("home/esp32/bedroom/temp", topic);
}

// Pins down: a humidity reading goes to <location>/humidity.
TEST(TopicGroup, HumidityTopicEndsInHumidity)
{
    CHECK_TRUE(build_topic(topic, sizeof(topic), "bedroom", HUMIDITY));
    STRCMP_EQUAL("home/esp32/bedroom/humidity", topic);
}

// Pins down: the location is what tells two sensors on the same node apart.
TEST(TopicGroup, TheLocationChangesTheTopic)
{
    CHECK_TRUE(build_topic(topic, sizeof(topic), "fridge", TEMPERATURE));
    STRCMP_EQUAL("home/esp32/fridge/temp", topic);
}

// Pins down: a buffer with exactly room for the topic and its NUL is enough.
TEST(TopicGroup, BufferWithExactlyEnoughRoomIsAccepted)
{
    const char *expected = "home/esp32/bedroom/temp";

    CHECK_TRUE(build_topic(topic, strlen(expected) + 1, "bedroom", TEMPERATURE));
    STRCMP_EQUAL(expected, topic);
}

// Pins down: one byte short (no room for the NUL) is a failure, not a silent cut-off.
TEST(TopicGroup, BufferOneByteTooSmallIsRejected)
{
    const char *expected = "home/esp32/bedroom/temp";

    CHECK_FALSE(build_topic(topic, strlen(expected), "bedroom", TEMPERATURE));
}

// Pins down: a zero-size buffer is rejected and nothing is written to it.
TEST(TopicGroup, ZeroSizeBufferIsRejectedAndNotWritten)
{
    CHECK_FALSE(build_topic(topic, 0, "bedroom", TEMPERATURE));
    LONGS_EQUAL('X', topic[0]);
}

// Pins down: a NULL location is rejected instead of crashing in strlen.
TEST(TopicGroup, NullLocationIsRejected)
{
    CHECK_FALSE(build_topic(topic, sizeof(topic), NULL, TEMPERATURE));
}

// Pins down: an empty location is rejected, because home/esp32//temp is a topic the dashboard
// backend would silently drop.
TEST(TopicGroup, EmptyLocationIsRejected)
{
    CHECK_FALSE(build_topic(topic, sizeof(topic), "", TEMPERATURE));
}
