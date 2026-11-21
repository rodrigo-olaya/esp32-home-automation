#ifndef TORQUE_TEST_H
#define TORQUE_TEST_H

/**
 * @brief Bench test for pushing a switch with the motor, driven from the serial monitor.
 *        Keys jog or press the motor; after each move the encoder reports how far the shaft
 *        really turned, so a stall (commanded steps that did not happen) shows up in the log.
 *        Only built into the test firmware: idf.py -B test-build -D MOTOR_SWITCH_TEST=1 flash monitor
 *        Never returns.
 */
void torque_test_console(void);

#endif // TORQUE_TEST_H
