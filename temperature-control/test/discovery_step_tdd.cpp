#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

/*
 * The general process: what gets called when. One call to discovery_step() is one pass of the
 * discovery task. On the board a task calls it every few seconds. Here the sensors, the clock and
 * the task start/stop are all fakes, so the whole loop runs on the Mac.
 *
 * Added to discovery.h:
 *
 *   typedef void (*sensor_action_fn_t)(sensor_flag_t sensor);
 *   typedef struct { sensor_action_fn_t start; sensor_action_fn_t stop; } discovery_actions_t;
 *   typedef struct { uint8_t running; } discovery_state_t;     // mask of sensors that have a task
 *
 *   void discovery_step(discovery_state_t *state,
 *                       const discovery_probes_t *probes,
 *                       discovery_health_t *health,
 *                       uint32_t now_ms,
 *                       const discovery_actions_t *actions);
 *
 * One pass does, in this order:
 *   1. Running sensors that have gone quiet (discovery_recently_seen) get stop(sensor) called,
 *      and are cleared from state->running.
 *   2. Sensors that have NO task are probed, and each one that answers gets start(sensor) called,
 *      is added to state->running, and has its health.last_seen_ms set to now_ms so it gets a
 *      grace period to produce its first reading.
 *   A sensor that already has a task is never probed (that would disturb a read in progress).
 *
 * Every step below only uses discovery_scan, discovery_newly_found, discovery_lost and
 * discovery_recently_seen, which already have their own tests.
 */

extern "C" {
    #include "../main/discovery/discovery.h"
}

/* ---- fakes ---- */
static bool fake_sht31_present;
static bool fake_ds18b20_present;
static int sht31_probe_calls;
static int ds18b20_probe_calls;

static bool fake_probe_sht31(void)
{
    sht31_probe_calls++;
    return fake_sht31_present;
}

static bool fake_probe_ds18b20(void)
{
    ds18b20_probe_calls++;
    return fake_ds18b20_present;
}

static sensor_flag_t started[8];
static int started_count;
static sensor_flag_t stopped[8];
static int stopped_count;

static void fake_start(sensor_flag_t sensor)
{
    started[started_count++] = sensor;
}

static void fake_stop(sensor_flag_t sensor)
{
    stopped[stopped_count++] = sensor;
}

TEST_GROUP(DiscoveryStepGroup)
{
    discovery_state_t state;
    discovery_probes_t probes;
    discovery_actions_t actions;
    discovery_health_t health;
    uint32_t now;

    void setup()
    {
        fake_sht31_present = false;
        fake_ds18b20_present = false;
        sht31_probe_calls = 0;
        ds18b20_probe_calls = 0;
        started_count = 0;
        stopped_count = 0;

        state.running = 0;
        probes.sht31 = fake_probe_sht31;
        probes.ds18b20 = fake_probe_ds18b20;
        actions.start = fake_start;
        actions.stop = fake_stop;

        now = 100000;
        health.sht31.interval_ms = 10000;
        health.ds18b20.interval_ms = 6000;
        health.sht31.last_seen_ms = now;
        health.ds18b20.last_seen_ms = now;
    }

    /* One pass of the discovery task at the current fake time. */
    void step()
    {
        discovery_step(&state, &probes, &health, now, &actions);
    }

    bool was_started(sensor_flag_t sensor)
    {
        for (int i = 0; i < started_count; i++) {
            if (started[i] == sensor) return true;
        }
        return false;
    }
};

/* ---- boot and plug-in ---- */

// Pins down: a node with nothing attached starts nothing, stops nothing, and keeps going.
TEST(DiscoveryStepGroup, NothingAttachedStartsNothingAndStopsNothing)
{
    step();

    LONGS_EQUAL(0, started_count);
    LONGS_EQUAL(0, stopped_count);
    LONGS_EQUAL(0, state.running);
}

// Pins down: the first pass (boot) starts a task for every sensor that answers.
TEST(DiscoveryStepGroup, BothSensorsPresentAtBootEachGetATask)
{
    fake_sht31_present = true;
    fake_ds18b20_present = true;

    step();

    LONGS_EQUAL(2, started_count);
    CHECK_TRUE(was_started(SENSOR_SHT31));
    CHECK_TRUE(was_started(SENSOR_DS18B20));
    LONGS_EQUAL(SENSOR_SHT31 | SENSOR_DS18B20, state.running);
}

// Pins down: a sensor plugged in after boot gets its task on a later pass, and nothing else changes.
TEST(DiscoveryStepGroup, SensorPluggedInLaterGetsItsTaskOnALaterStep)
{
    step();                               /* boot: nothing attached */
    fake_sht31_present = true;            /* someone plugs the SHT31 in */
    now += 5000;

    step();

    LONGS_EQUAL(1, started_count);
    LONGS_EQUAL(SENSOR_SHT31, started[0]);
    LONGS_EQUAL(0, stopped_count);
    LONGS_EQUAL(SENSOR_SHT31, state.running);
}

// Pins down: sensors that already have a healthy task are left alone.
TEST(DiscoveryStepGroup, HealthySensorsThatAlreadyHaveTasksAreNotStartedAgain)
{
    state.running = SENSOR_SHT31 | SENSOR_DS18B20;
    fake_sht31_present = true;
    fake_ds18b20_present = true;

    step();

    LONGS_EQUAL(0, started_count);
    LONGS_EQUAL(0, stopped_count);
    LONGS_EQUAL(SENSOR_SHT31 | SENSOR_DS18B20, state.running);
}

/* ---- what is probed ---- */

// Pins down: a running sensor is never probed (a 1-Wire reset would corrupt its read in progress);
// only the sensor without a task is asked.
TEST(DiscoveryStepGroup, ARunningSensorIsNeverProbed)
{
    state.running = SENSOR_SHT31;

    step();

    LONGS_EQUAL(0, sht31_probe_calls);
    LONGS_EQUAL(1, ds18b20_probe_calls);
}

/* ---- going silent and coming back ---- */

// Pins down: a running sensor that stopped reporting has its task stopped, and the other keeps running.
TEST(DiscoveryStepGroup, SilentSensorHasItsTaskStopped)
{
    state.running = SENSOR_SHT31 | SENSOR_DS18B20;
    health.sht31.last_seen_ms = now - 40000;    /* 4 intervals quiet */

    step();

    LONGS_EQUAL(1, stopped_count);
    LONGS_EQUAL(SENSOR_SHT31, stopped[0]);
    LONGS_EQUAL(SENSOR_DS18B20, state.running);
}

// Pins down: after its task was stopped, the sensor gets a fresh task when it answers again.
TEST(DiscoveryStepGroup, StoppedSensorGetsAFreshTaskWhenItComesBack)
{
    state.running = SENSOR_SHT31 | SENSOR_DS18B20;
    health.sht31.last_seen_ms = now - 40000;
    step();                               /* SHT31 goes silent, its task is stopped */

    fake_sht31_present = true;            /* it recovers */
    now += 5000;
    step();

    LONGS_EQUAL(1, started_count);
    LONGS_EQUAL(SENSOR_SHT31, started[0]);
    LONGS_EQUAL(SENSOR_SHT31 | SENSOR_DS18B20, state.running);
}

// Pins down: a task that was just started is given time to produce its first reading
// instead of being judged stale from an old timestamp.
TEST(DiscoveryStepGroup, NewlyStartedSensorIsNotStoppedBeforeItHasHadTimeToReport)
{
    health.sht31.last_seen_ms = 0;        /* never reported anything */
    fake_sht31_present = true;
    step();                               /* task started now */

    now += 1000;                          /* one second later, still no reading yet */
    step();

    LONGS_EQUAL(0, stopped_count);
    LONGS_EQUAL(SENSOR_SHT31, state.running);
}

// Pins down: a task that hung but whose sensor is still electrically present.
// The SHT31 task has gone silent for 4 intervals, but the probe would still answer.
// Design choice: is the task stopped and restarted in the SAME pass (fast recovery), or only
// stopped now and started on the NEXT pass?
TEST(DiscoveryStepGroup, SilentSensorThatStillAnswersTheProbe)
{
    state.running = SENSOR_SHT31 | SENSOR_DS18B20;
    health.sht31.last_seen_ms = now - 40000;
    fake_sht31_present = true;

    step();

    // TODO(you): after this one pass, how many times was the SHT31 stopped, how many times started,
    // and is it in state.running? Decide the behaviour, then assert it.
    FAIL("TODO(you): design choice not made yet");
}
