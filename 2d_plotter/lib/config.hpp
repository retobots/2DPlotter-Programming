#ifndef CONFIG_HPP
#define CONFIG_HPP

// Thông số cơ bản
#define STEPS_PER_MM_X 100.0
#define STEPS_PER_MM_Y 100.0

#define X_MIN 0
#define X_MAX 200
#define Y_MIN 0
#define Y_MAX 200

#define DEFAULT_SPEED_X 600
#define DEFAULT_SPEED_Y 600

// Servo Control
#define PEN_SERVO_PIN 10
#define PEN_UP_ANGLE 115
#define PEN_DOWN_ANGLE 83

// Các chế độ hoạt động
#define MODE_AUTO 0
#define MODE_MANUAL 1

// Kết nối mạng (cho IoT)
#define WIFI_SSID "your_SSID"
#define WIFI_PASSWORD "your_password"
#define MQTT_SERVER "192.168.1.100"
#define MQTT_PORT 1883

#endif