#include "../lib/config.hpp"
#include "../lib/motor_control.hpp"
#include "../lib/pen_control.hpp"
#include "../lib/input_process.hpp"
#include "../lib/motion_control.hpp"
#include "../lib/manual_control.hpp"
#include "../lib/display_manager.hpp"
#include "../lib/iot_manager.hpp"

int currentMode = MODE_AUTO;

void setup()
{
  Serial.begin(115200);
  setupMotors();
  setupPen();
  setupManualControl();
  setupDisplay();
  setupIoT();
  homePosition();
  penUp();
  Serial.println("Plotter Ready!");
}

void loop()
{
}
