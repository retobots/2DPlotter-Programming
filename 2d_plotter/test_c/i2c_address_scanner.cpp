#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Serial.println("\n--- I2C Scanner ESP32 ---");
  
  // Khởi tạo giao tiếp I2C. 
  // ESP32 mặc định dùng GPIO 21 (SDA) và GPIO 22 (SCL).
  Wire.begin(); 
}

void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Đang quét I2C...");

  nDevices = 0;
  // Các địa chỉ I2C hợp lệ nằm trong khoảng 0x01 đến 0x7F (127)
  for(address = 1; address < 127; address++ ) {
    
    // Wire.beginTransmission(address) bắt đầu truyền dữ liệu tới địa chỉ
    Wire.beginTransmission(address);
    
    // Wire.endTransmission() thực hiện truyền dữ liệu. 
    // Nếu thiết bị tồn tại và phản hồi, nó sẽ trả về 0 (Wire::endTransmission(false) mặc định).
    error = Wire.endTransmission(); 

    if (error == 0) {
      Serial.print("Thiết bị I2C được tìm thấy tại địa chỉ 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX); // In địa chỉ ở định dạng Hex
      Serial.println("  <-- PCF8574 có thể ở đây!");

      nDevices++;
    }
    // Nếu error == 4: Lỗi không xác định khác (ví dụ: kết nối lỏng)
    else if (error == 4) {
      // Serial.print("Lỗi không xác định tại địa chỉ 0x");
      // if (address<16) 
      //   Serial.print("0");
      // Serial.println(address,HEX);
    }    
  }

  if (nDevices == 0) {
    Serial.println("Không tìm thấy thiết bị I2C nào.");
    Serial.println("Vui lòng kiểm tra lại kết nối SDA/SCL và nguồn.");
  } else {
    Serial.println("Hoàn tất.");
  }
  
  // Chờ 5 giây trước khi quét lại
  delay(5000); 
}