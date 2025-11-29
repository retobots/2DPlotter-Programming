#include "IoHwAb_Stepper.h"
#include <math.h>

// Singleton access
IoHwAb_Stepper &IoHwAb_Stepper::getInstance()
{
  static IoHwAb_Stepper instance;
  return instance;
}

// Private constructor
IoHwAb_Stepper::IoHwAb_Stepper()
{
  // Nothing heavy here; real init is done in setup()
}

// ----------------------------------------------------------------------------
// Setup & basic control
// ----------------------------------------------------------------------------

void IoHwAb_Stepper::setup()
{
  // Configure GPIOs for STEP/DIR
  pinMode(PIN_X_STEP, OUTPUT);
  pinMode(PIN_X_DIR, OUTPUT);
  pinMode(PIN_Y_STEP, OUTPUT);
  pinMode(PIN_Y_DIR, OUTPUT);

  digitalWrite(PIN_X_STEP, LOW);
  digitalWrite(PIN_Y_STEP, LOW);

  // Initialize timer only once
  if (_timer == nullptr)
  {
    _timer = timerBegin(0, TIMER_PRESCALER, true);
    timerAttachInterrupt(_timer, &IoHwAb_Stepper::onTimerISRStatic, true);
    timerAlarmWrite(_timer, TIMER_TICKS_PER_ISR, true);
    timerAlarmEnable(_timer);
  }

  // Reset motion engine state
  noInterrupts();
  _head = 0;
  _tail = 0;
  _hasCurrent = false;
  _tickCount = 0;
  _errX = 0;
  _errY = 0;
  interrupts();

  // Default feed rate (steps/s); can be overridden via setSpeed()
  _currentSpeedStepsPerSec = MAX_SPEED; // MAX_SPEED is in steps/s in original config

  // Ensure drivers are enabled
  enableOutputs();
}

// ----------------------------------------------------------------------------
// Block-style relative / absolute moves (temporary compatibility layer)
// ----------------------------------------------------------------------------

void IoHwAb_Stepper::move(int32_t steps_x, int32_t steps_y)
{
  // No movement requested
  if (steps_x == 0 && steps_y == 0)
    return;

  int8_t dirX = (steps_x >= 0) ? 1 : -1;
  int8_t dirY = (steps_y >= 0) ? 1 : -1;

  uint32_t absX = static_cast<uint32_t>(labs(steps_x));
  uint32_t absY = static_cast<uint32_t>(labs(steps_y));
  uint32_t major = (absX > absY) ? absX : absY;
  if (major == 0)
    return;

  // Compute total ticks based on current feed rate
  float speed = _currentSpeedStepsPerSec;
  if (speed <= 0.0f)
  {
    speed = MAX_SPEED;
  }

  // Time to finish the move: T = majorSteps / speed
  // totalTicks = T * STEPPER_TIMER_FREQ_HZ
  uint32_t totalTicks = static_cast<uint32_t>((static_cast<float>(major) * STEPPER_TIMER_FREQ_HZ) / speed);
  if (totalTicks < major)
  {
    // Safety: ensure we have at least one tick per step
    totalTicks = major;
  }

  // Enqueue a single segment and wait for completion
  // (This keeps the old blocking behaviour. Later, MotionControlService will
  //  use enqueueSegment() directly to become fully non-blocking.)
  while (!enqueueSegment(absX, absY, dirX, dirY, totalTicks))
  {
    // Queue is full – allow ISR to drain it a bit
    delay(1);
  }

  // Wait until the queue and current segment are finished
  for (;;)
  {
    noInterrupts();
    bool busy = _hasCurrent || (_head != _tail);
    interrupts();
    if (!busy)
      break;
    delay(1);
  }

  // Update software position
  _currentXSteps += steps_x;
  _currentYSteps += steps_y;
}

void IoHwAb_Stepper::moveTo(float xPos_mm, float yPos_mm)
{
  // Convert mm to steps using configured steps/mm constants
  long targetX = lroundf(xPos_mm * STEPS_PER_MM_X);
  long targetY = lroundf(yPos_mm * STEPS_PER_MM_Y);

  long dx = targetX - _currentXSteps;
  long dy = targetY - _currentYSteps;

  move(dx, dy);
}

// ----------------------------------------------------------------------------
// Stop / outputs
// ----------------------------------------------------------------------------

void IoHwAb_Stepper::stop()
{
  // Cancel queued motion and current segment
  noInterrupts();
  _head = 0;
  _tail = 0;
  _hasCurrent = false;
  interrupts();

  // Also stop manual rolling
  stopRolling();

  // Optionally: disable outputs here if your driver uses an EN pin
  // (left empty to avoid assumptions about hardware wiring)
}

void IoHwAb_Stepper::enableOutputs()
{
  // If you have an enable pin, drive it here, e.g.:
  //   pinMode(PIN_STEPPER_EN, OUTPUT);
  //   digitalWrite(PIN_STEPPER_EN, LOW); // enable (active-low)
  // For now we only ensure STEP lines are low.
  digitalWrite(PIN_X_STEP, LOW);
  digitalWrite(PIN_Y_STEP, LOW);
}

// ----------------------------------------------------------------------------
// Manual rolling implementation (no AccelStepper)
// ----------------------------------------------------------------------------

void IoHwAb_Stepper::setRollSteps(float xStepsPerSec, float yStepsPerSec)
{
  _rollSpeedX = xStepsPerSec;
  _rollSpeedY = yStepsPerSec;

  if (fabsf(_rollSpeedX) < 0.001f && fabsf(_rollSpeedY) < 0.001f)
  {
    _rolling = false;
  }
  else
  {
    _rolling = true;
    uint32_t now = micros();
    _lastRollMicrosX = now;
    _lastRollMicrosY = now;
  }
}

void IoHwAb_Stepper::setRollMM(float xMmPerSec, float yMmPerSec)
{
  // Convert mm/s to steps/s
  float xStepsPerSec = xMmPerSec * STEPS_PER_MM_X;
  float yStepsPerSec = yMmPerSec * STEPS_PER_MM_Y;
  setRollSteps(xStepsPerSec, yStepsPerSec);
}

void IoHwAb_Stepper::runSpeedTickX()
{
  if (!_rolling || fabsf(_rollSpeedX) < 0.001f)
    return;

  float speed = fabsf(_rollSpeedX);
  if (speed < 1.0f)
    return; // avoid extremely slow speeds → effectively stopped

  uint32_t now = micros();
  uint32_t elapsed = now - _lastRollMicrosX;

  // Interval between two steps in microseconds
  float interval = 1000000.0f / speed;

  while (elapsed >= interval)
  {
    int8_t dir = (_rollSpeedX > 0.0f) ? 1 : -1;
    stepPulseX(dir);
    _currentXSteps += dir;

    elapsed -= static_cast<uint32_t>(interval);
    _lastRollMicrosX += static_cast<uint32_t>(interval);
  }
}

void IoHwAb_Stepper::runSpeedTickY()
{
  if (!_rolling || fabsf(_rollSpeedY) < 0.001f)
    return;

  float speed = fabsf(_rollSpeedY);
  if (speed < 1.0f)
    return;

  uint32_t now = micros();
  uint32_t elapsed = now - _lastRollMicrosY;

  float interval = 1000000.0f / speed;

  while (elapsed >= interval)
  {
    int8_t dir = (_rollSpeedY > 0.0f) ? 1 : -1;
    stepPulseY(dir);
    _currentYSteps += dir;

    elapsed -= static_cast<uint32_t>(interval);
    _lastRollMicrosY += static_cast<uint32_t>(interval);
  }
}

void IoHwAb_Stepper::stopRolling()
{
  _rollSpeedX = 0.0f;
  _rollSpeedY = 0.0f;
  _rolling = false;
}

// ----------------------------------------------------------------------------
// Feed rate and software position
// ----------------------------------------------------------------------------

void IoHwAb_Stepper::setSpeed(float speedStepsPerSec)
{
  // Treat this as steps/second; caller (G-code parser) is responsible for unit conversion.
  if (speedStepsPerSec <= 0.0f)
  {
    _currentSpeedStepsPerSec = MAX_SPEED;
  }
  else
  {
    _currentSpeedStepsPerSec = speedStepsPerSec;
  }
}

void IoHwAb_Stepper::setCurrentPosition(long xSteps, long ySteps)
{
  noInterrupts();
  _currentXSteps = xSteps;
  _currentYSteps = ySteps;
  interrupts();
}

// ----------------------------------------------------------------------------
// Motion queue API
// ----------------------------------------------------------------------------

bool IoHwAb_Stepper::enqueueSegment(int32_t stepsX,
                                    int32_t stepsY,
                                    int8_t dirX,
                                    int8_t dirY,
                                    uint32_t totalTicks)
{
  if (stepsX < 0 || stepsY < 0 || totalTicks == 0)
  {
    return false;
  }

  noInterrupts();
  uint8_t nextTail = static_cast<uint8_t>((_tail + 1) % QUEUE_SIZE);
  if (nextTail == _head)
  {
    // Queue full
    interrupts();
    return false;
  }

  // Ghi trực tiếp vào slot trong queue (tránh dùng operator= với volatile)
  volatile MotionSegment &slot = _queue[_tail];
  slot.stepsX = static_cast<uint32_t>(stepsX);
  slot.stepsY = static_cast<uint32_t>(stepsY);
  slot.dirX = dirX;
  slot.dirY = dirY;
  slot.totalTicks = totalTicks;

  _tail = nextTail;
  interrupts();

  return true;
}

bool IoHwAb_Stepper::isQueueEmpty() const
{
  // head/tail are modified in ISR; reading them without critical section is
  // acceptable here for a simple check (slight race is harmless).
  return (_head == _tail);
}

bool IoHwAb_Stepper::isQueueFull() const
{
  uint8_t nextTail = static_cast<uint8_t>((_tail + 1) % QUEUE_SIZE);
  return (nextTail == _head);
}

// ----------------------------------------------------------------------------
// Timer ISR & helpers
// ----------------------------------------------------------------------------

void IRAM_ATTR IoHwAb_Stepper::onTimerISRStatic()
{
  IoHwAb_Stepper::getInstance().onTimerISR();
}

bool IoHwAb_Stepper::popSegmentFromISR(MotionSegment &outSeg)
{
  if (_head == _tail)
  {
    return false; // empty
  }

  volatile MotionSegment &slot = _queue[_head];

  // Copy từng field từ volatile sang non-volatile
  outSeg.stepsX = slot.stepsX;
  outSeg.stepsY = slot.stepsY;
  outSeg.dirX = slot.dirX;
  outSeg.dirY = slot.dirY;
  outSeg.totalTicks = slot.totalTicks;

  _head = static_cast<uint8_t>((_head + 1) % QUEUE_SIZE);
  return true;
}

void IRAM_ATTR IoHwAb_Stepper::onTimerISR()
{
  // If we do not have an active segment, try to grab one from the queue
  if (!_hasCurrent)
  {
    MotionSegment seg;
    if (!popSegmentFromISR(seg))
    {
      return; // nothing to do
    }

    // Copy từng field (tránh operator= trên volatile hoặc struct)
    _currentSeg.stepsX = seg.stepsX;
    _currentSeg.stepsY = seg.stepsY;
    _currentSeg.dirX = seg.dirX;
    _currentSeg.dirY = seg.dirY;
    _currentSeg.totalTicks = seg.totalTicks;

    _tickCount = 0;
    _errX = 0;
    _errY = 0;
    _hasCurrent = true;
  }

  // Advance one tick of the current segment
  _tickCount++;

  // Digital differential analyzer (integer version)
  _errX += static_cast<int32_t>(_currentSeg.stepsX);
  if (_errX >= static_cast<int32_t>(_currentSeg.totalTicks))
  {
    _errX -= static_cast<int32_t>(_currentSeg.totalTicks);
    stepPulseX(_currentSeg.dirX);
  }

  _errY += static_cast<int32_t>(_currentSeg.stepsY);
  if (_errY >= static_cast<int32_t>(_currentSeg.totalTicks))
  {
    _errY -= static_cast<int32_t>(_currentSeg.totalTicks);
    stepPulseY(_currentSeg.dirY);
  }

  // Check if the segment is finished
  if (_tickCount >= _currentSeg.totalTicks)
  {
    _hasCurrent = false;
  }
}

inline void IoHwAb_Stepper::stepPulseX(int8_t dir)
{
  bool level = (dir > 0);
  if (X_DIR_INVERT)
    level = !level;

  digitalWrite(PIN_X_DIR, level ? HIGH : LOW);
  digitalWrite(PIN_X_STEP, HIGH);
  digitalWrite(PIN_X_STEP, LOW);
}

inline void IoHwAb_Stepper::stepPulseY(int8_t dir)
{
  bool level = (dir > 0);
  if (Y_DIR_INVERT)
    level = !level;

  digitalWrite(PIN_Y_DIR, level ? HIGH : LOW);
  digitalWrite(PIN_Y_STEP, HIGH);
  digitalWrite(PIN_Y_STEP, LOW);
}
