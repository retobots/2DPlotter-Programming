#include <Arduino.h>

/*==========  General Setup =========*/
#define ENABLE 8

/*========== Setup X-Stepper =========*/
#define X_DIR 5
#define X_STEP 2

/*========== Setup Z-Stepper ==========*/
#define Z_DIR 7
#define Z_STEP 4

/*=========== Khai báo biến toàn cục ==========*/

uint16_t steps = 100;

/*========== Setup Thông số Motor ==========*/
double r = 16.6;        // Bán kính báng răng xoay đai motor
int steps_number(int x) // Tính số bước
{
  return x / ((1.0 / 200) * 2.0 * 3.14 * r);
}

/*=========== Khai báo class vẽ hình ==========*/

class shape
{
protected:
  virtual void length_draw() = 0;
  virtual void width_draw() = 0;
};

class rectangle : shape
{
private:
  uint16_t Length;
  uint16_t Width;

public:
  void length_draw() override;
  void width_draw() override;

  void setLength(int length)
  {
    Length = length;
  }

  void setWidth(int width)
  {
    Width = width;
  }
};

// Định nghĩa hàm
void rectangle::length_draw()
{
  // Vẽ chiều dài
  for (int i = 0; i < steps_number(rectangle::Length); i++)
  {
    digitalWrite(X_STEP, HIGH);
    delay(10);
    digitalWrite(X_STEP, LOW);
    delay(10);
  }
}

void rectangle::width_draw()
{
  // Vẽ chiều dài
  for (int i = 0; i < steps_number(rectangle::Width); i++)
  {
    digitalWrite(Z_STEP, HIGH);
    delay(10);
    digitalWrite(Z_STEP, LOW);
    delay(10);
  }
}

void start_draw(rectangle &a)
{
  for (int i = 0; i < 2; i++)
  {
    a.length_draw();
    delay(50);
    a.width_draw();
    delay(50);
  }
}

/*========== Setup Vi điều khiển ==========*/
void setup()
{
  // Setup Chân Điều Khiển X
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STEP, OUTPUT);

  // Setup chân điều khiển Z
  pinMode(Z_DIR, OUTPUT);
  pinMode(Z_STEP, OUTPUT);

  // Setup khởi động
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, LOW);

  // Setup thông số ban đầu
  rectangle rec;
  rec.setLength(100);
  rec.setWidth(60);

  start_draw(rec);
}

void loop()
{
}