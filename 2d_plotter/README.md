
```
2d_plotter
├─ include
│  ├─ Application
│  │  ├─ AppController.h # Quản lý lifecycle toàn hệ thống (init + run)
│  │  ├─ AutoModeController.h # Điều phối chế độ Auto: đọc & vẽ G-code
│  │  └─ ManualModeController.h # Điều phối chế độ Manual: điều khiển tay bằng PS4
│  ├─ Config
│  │  ├─ Limitations.h # Cấu hình giới hạn toạ độ X/Y, bước, vùng an toàn
│  │  ├─ Pins.h # Mapping GPIO: step, dir, PS4, LCD, buzzer, v.v.
│  │  └─ Settings.h # Các hằng số cấu hình hệ thống: tốc độ, delay, độ phân giải
│  ├─ HAL
│  │  ├─ BuzzerHAL.h # Điều khiển buzzer theo trạng thái hệ thống
│  │  ├─ LcdHAL.h # Giao tiếp LCD I2C để hiển thị trạng thái, menu
│  │  ├─ PS4ControllerHAL.h # Giao tiếp tay cầm PS4 qua Bluetooth (dùng trong Manual Mode)
│  │  ├─ RotaryEncoderHAL.h # Đọc input từ Rotary Encoder (dùng trong Menu điều hướng)
│  │  ├─ RTCHAL.h # Giao tiếp module RTC DS1307 (hiển thị thời gian)
│  │  ├─ SDCardHAL.h # Đọc file G-code từ thẻ nhớ SD
│  │  ├─ ServoHAL.h # Điều khiển servo để nhấc/hạ bút
│  │  └─ StepperHAL.h # Gửi tín hiệu step/dir cho motor trục X/Y
│  ├─ README 
│  └─ Services
│     ├─ FeedbackService.h # Hiển thị trạng thái hệ thống qua LCD + buzzer
│     ├─ GcodeParserService.h # Phân tích lệnh G0/G1/M3/M5 trong G-code
│     ├─ MotionControlService.h # Điều phối chuyển động trục X/Y theo yêu cầu
│     ├─ PenService.h # Logic nhấc/hạ bút theo lệnh hoặc nút bấm
│     └─ UIMenuService.h # Quản lý giao diện menu điều hướng (chọn mode, file, v.v.)
├─ lib
│  ├─ PS4_Controller_Host-main
│  │  └─ PS4_Controller_Host-main
│  │     ├─ component.mk
│  │     ├─ examples
│  │     │  ├─ PS4Data
│  │     │  │  └─ PS4Data.ino
│  │     │  ├─ PS4ReceiveData
│  │     │  │  └─ PS4ReceiveData.ino
│  │     │  ├─ PS4SendData
│  │     │  │  └─ PS4SendData.ino
│  │     │  └─ PS4ViewIncomingBits
│  │     │     └─ PS4ViewIncomingBits.ino
│  │     ├─ keywords.txt
│  │     ├─ library.json
│  │     ├─ library.properties
│  │     ├─ LICENSE
│  │     ├─ README.md
│  │     └─ src
│  │        ├─ osi
│  │        │  └─ allocator.h
│  │        ├─ ps4.c
│  │        ├─ ps4.h
│  │        ├─ PS4Controller.cpp
│  │        ├─ PS4Controller.h
│  │        ├─ ps4_int.h
│  │        ├─ ps4_l2cap.c
│  │        ├─ ps4_parser.c
│  │        ├─ ps4_spp.c
│  │        └─ stack
│  │           ├─ btm_api.h
│  │           ├─ bt_types.h
│  │           ├─ gap_api.h
│  │           ├─ hcidefs.h
│  │           ├─ l2cdefs.h
│  │           └─ l2c_api.h
│  └─ README
├─ platformio.ini
├─ rules
│  ├─ Cấu trúc Code.cpp
│  ├─ Quy tắc hoạt động.md
│  └─ Quy tắc đặt tên file.md
├─ src
│  ├─ Application
│  │  ├─ AppController.cpp
│  │  ├─ AutoModeController.cpp
│  │  └─ ManualModeController.cpp
│  ├─ HAL
│  │  ├─ BuzzerHAL.cpp
│  │  ├─ LcdHAL.cpp
│  │  ├─ PS4ControllerHAL.cpp
│  │  ├─ RotaryEncoderHAL.cpp
│  │  ├─ RTCHAL.cpp
│  │  ├─ SDCardHAL.cpp
│  │  ├─ ServoHAL.cpp
│  │  └─ StepperHAL.cpp
│  ├─ main.cpp
│  └─ Services
│     ├─ FeedbackService.cpp
│     ├─ GcodeParserService.cpp
│     ├─ MotionControlService.cpp
│     ├─ PenService.cpp
│     └─ UIMenuService.cpp
└─ test
   └─ README

```
```
2d_plotter
├─ include
│  ├─ Application
│  │  ├─ AppController.h
│  │  ├─ AutoModeController.h
│  │  └─ ManualModeController.h
│  ├─ Config
│  │  ├─ Limits.h
│  │  ├─ Pins.h
│  │  └─ Settings.h
│  ├─ HAL
│  │  ├─ BuzzerHAL.h
│  │  ├─ LcdHAL.h
│  │  ├─ PS4ControllerHAL.h
│  │  ├─ RotaryEncoderHAL.h
│  │  ├─ RTCHAL.h
│  │  ├─ SDCardHAL.h
│  │  ├─ ServoHAL.h
│  │  └─ StepperHAL.h
│  ├─ README
│  └─ Services
│     ├─ FeedbackService.h
│     ├─ GcodeParserService.h
│     ├─ MotionControlService.h
│     ├─ PenService.h
│     └─ UIMenuService.h
├─ lib
│  └─ README
├─ platformio.ini
├─ README.md
├─ rules
│  ├─ Cấu trúc Code.cpp
│  ├─ Quy tắc hoạt động.md
│  └─ Quy tắc đặt tên file.md
├─ src
│  ├─ Application
│  │  ├─ AppController.cpp
│  │  ├─ AutoModeController.cpp
│  │  └─ ManualModeController.cpp
│  ├─ HAL
│  │  ├─ BuzzerHAL.cpp
│  │  ├─ LcdHAL.cpp
│  │  ├─ PS4ControllerHAL.cpp
│  │  ├─ RotaryEncoderHAL.cpp
│  │  ├─ RTCHAL.cpp
│  │  ├─ SDCardHAL.cpp
│  │  ├─ ServoHAL.cpp
│  │  └─ StepperHAL.cpp
│  ├─ main.cpp
│  └─ Services
│     ├─ FeedbackService.cpp
│     ├─ GcodeParserService.cpp
│     ├─ MotionControlService.cpp
│     ├─ PenService.cpp
│     └─ UIMenuService.cpp
└─ test
   └─ README

```