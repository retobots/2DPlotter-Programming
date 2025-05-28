
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