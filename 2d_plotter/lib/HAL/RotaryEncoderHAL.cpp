
/*======================================================== [INCLUDE LIBRARY] ===================================================*/
#include "RotaryEncoderHAL.hpp"

/*=====================================================  [FUNCTION DEFINITIONS] ================================================*/

/*========================================================== ( Hàm Setup ) =====================================================*/

void setupRotaryEncoder()
{
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    Serial.begin(9600);
    lastCLK = digitalRead(CLK);
}

/*===================================================== ( Hàm Đọc Điều Khiển ) =================================================*/

void readEncoder()
{
    int currentCLK = digitalRead(CLK);

    if (currentCLK != lastCLK && currentCLK == LOW)
    { // Sườn xuống
        int dtState = digitalRead(DT);

        if (dtState != currentCLK)
        {
            upFlag = true;
        }
        else
        {
            downFlag = true;
        }
    }

    lastCLK = currentCLK;
}

/*======================================================= ( Hàm Scroll Lên ) ====================================================*/

bool scrollUp()
{
    if (upFlag)
    {
        upFlag = false;
        return true;
    }
    return false;
}

/*====================================================== ( Hàm Scroll Xuống ) ===================================================*/

bool scrollDown()
{
    if (downFlag)
    {
        downFlag = false;
        return true;
    }
    return false;
}