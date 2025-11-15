#include <Wire.h>
#include <PCF8574.h> // Đảm bảo bạn đã cài đặt thư viện PCF8574

// Địa chỉ I2C mặc định của PCF8574. 
// Nếu module của bạn có jumper A0, A1, A2, địa chỉ có thể khác: 0x20 đến 0x27.
// Ví dụ: Nếu A0, A1, A2 đều nối đất, địa chỉ là 0x20.
// Nếu module là PCF8574A, địa chỉ thường từ 0x38 đến 0x3F.
// Bạn nên dùng I2C Scanner để tìm chính xác nếu 0x27 không hoạt động.
PCF8574 pcf8574(0x27); 

void setup() {
  Serial.begin(115200);
  Serial.println("--- ESP32 PCF8574 Test ---");
  
  // Khởi tạo giao tiếp I2C.
  // ESP32 mặc định dùng GPIO 21 (SDA) và GPIO 22 (SCL).
  Wire.begin(); 

  // Bắt đầu giao tiếp với PCF8574
  if (pcf8574.begin()){
    Serial.println("PCF8574 đã được tìm thấy!");
  } else {
    Serial.println("Lỗi: Không tìm thấy PCF8574. Vui lòng kiểm tra địa chỉ và kết nối.");
    while(1); // Dừng chương trình nếu không tìm thấy
  }

  // Cài đặt tất cả các chân P0-P7 là OUTPUT (Đây là bước bắt buộc với một số thư viện PCF8574)
  for (int i = 0; i < 8; i++) {
    pcf8574.pinMode(i, OUTPUT);
  }
}

void loop() {
  Serial.println("Bắt đầu chu trình Test: Bật từng chân P0-P7");
  
  // Vòng lặp bật từng chân một
  for (int pin = 0; pin < 8; pin++) {
    // Tắt tất cả các chân trước khi bật chân hiện tại
    for(int i = 0; i < 8; i++) {
        pcf8574.digitalWrite(i, HIGH); // Lưu ý: PCF8574 là "active-low" theo thiết kế 
                                        // nên HIGH thường có nghĩa là TẮT (LED TẮT), LOW là BẬT (LED SÁNG).
                                        // Chúng ta sẽ dùng LOW để BẬT.
    }
    
    // Bật chân hiện tại (LOW để sáng LED)
    pcf8574.digitalWrite(pin, LOW); 
    Serial.print("Đang BẬT chân: P");
    Serial.println(pin);
    delay(500); // Chờ 0.5 giây
  }

  Serial.println("Kết thúc chu trình. Tắt tất cả (HIGH)");
  // Tắt tất cả các chân (HIGH để tắt LED)
  for (int pin = 0; pin < 8; pin++) {
    pcf8574.digitalWrite(pin, HIGH); 
  }
  
  delay(1000); // Chờ 1 giây trước khi lặp lại
}