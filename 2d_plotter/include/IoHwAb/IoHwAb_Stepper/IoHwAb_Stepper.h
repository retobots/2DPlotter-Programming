#pragma once
#include <Arduino.h>
#include "Pins.h"
#include "Settings.h"

class IoHwAb_Stepper
{
public:
  static IoHwAb_Stepper &getInstance();
  void setup();

  void move(int32_t steps_x, int32_t steps_y);

  void moveTo(float xPos_mm, float yPos_mm);

  void stop();

  void enableOutputs();

  void setRollSteps(float xStepsPerSec, float yStepsPerSec);

  void setRollMM(float xMmPerSec, float yMmPerSec);

  void runSpeedTickX();

  void runSpeedTickY();

  void stopRolling();

  void setSpeed(float speedStepsPerSec);

  void setCurrentPosition(long xSteps, long ySteps);

  bool enqueueSegment(int32_t stepsX,
                      int32_t stepsY,
                      int8_t dirX,
                      int8_t dirY,
                      uint32_t totalTicks);

  bool isQueueEmpty() const;

  bool isQueueFull() const;

private:
  IoHwAb_Stepper();
  IoHwAb_Stepper(const IoHwAb_Stepper &) = delete;
  IoHwAb_Stepper &operator=(const IoHwAb_Stepper &) = delete;

  struct MotionSegment
  {
    uint32_t stepsX;
    uint32_t stepsY;
    int8_t dirX;
    int8_t dirY;
    uint32_t totalTicks;
  };

  // Queue configuration
  static constexpr uint8_t QUEUE_SIZE = 32;
  static constexpr uint32_t TIMER_BASE_FREQ_HZ = 1000000UL;  // 1 MHz after prescaler
  static constexpr uint16_t TIMER_PRESCALER = 80;            // 80 MHz / 80 = 1 MHz
  static constexpr uint32_t STEPPER_TIMER_FREQ_HZ = 20000UL; // 20 kHz tick rate
  static constexpr uint32_t TIMER_TICKS_PER_ISR = TIMER_BASE_FREQ_HZ / STEPPER_TIMER_FREQ_HZ;

  // Direction
  static constexpr bool X_DIR_INVERT = true;  // đổi thành true nếu X đang ngược
  static constexpr bool Y_DIR_INVERT = false; // tương tự cho Y nếu cần

  // Timer-based engine state
  hw_timer_t *_timer = nullptr;

  volatile MotionSegment _queue[QUEUE_SIZE];
  volatile uint8_t _head = 0;
  volatile uint8_t _tail = 0;
  volatile bool _hasCurrent = false;
  volatile MotionSegment _currentSeg{};
  volatile uint32_t _tickCount = 0;
  volatile int32_t _errX = 0;
  volatile int32_t _errY = 0;

  // Software position (steps)
  volatile long _currentXSteps = 0;
  volatile long _currentYSteps = 0;

  // Feed rate for auto moves (steps/s)
  float _currentSpeedStepsPerSec = 1000.0f;

  // Manual rolling state (steps/s)
  float _rollSpeedX = 0.0f;
  float _rollSpeedY = 0.0f;
  uint32_t _lastRollMicrosX = 0;
  uint32_t _lastRollMicrosY = 0;
  bool _rolling = false;

  // Timer ISR plumbing
  static void IRAM_ATTR onTimerISRStatic();
  void IRAM_ATTR onTimerISR();

  bool popSegmentFromISR(MotionSegment &outSeg);

  inline void stepPulseX(int8_t dir);
  inline void stepPulseY(int8_t dir);
};
