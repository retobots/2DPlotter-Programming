#pragma once

/**
 * @file    IoHwAb_Button.h
 * @author  Do Duc Nghia
 * @brief   File này chứa hàm đọc nút nhấn, setup cho nút nhấn
 * @version 1.0
 * @date    2025-06-19
 *
 * @copyright Copyright (c) 2025
 *
 */
/*================================================ [ INCLUDE LIBRARY ] ==================================================*/
#include <Arduino.h>

/*================================================= [ DECLARATION ] ==================================================*/

/****************************************************************************************
 * @class   IoHwAb_Button
 * @brief   Class này khai báo, setup nút nhấn và đọc tín hiệu nút nhấn
 ****************************************************************************************
 */
class IoHwAb_Button
{
private:
    /**
     * @brief Constructor khởi tạo các thông số cho nút nhấn khi tạo đối tượng
     *
     */
    IoHwAb_Button();

public:
    /**
     * @brief   Tạo đối tượng nút nhấn
     *
     * @return  IoHwAb_Button&
     */
    static IoHwAb_Button &getInstance();

    /**
     * @brief setup nút nhấn
     *
     */
    void setup();

    /**
     * @brief   Đọc tín hiệu nút nhấn
     *
     * @return  true
     * @return  false
     */
    bool isButtonPressed();

    /**
     * @brief   đọc tín hiệu nút nhấn, nếu giữ quá 3s thì sẽ hiểu là hold
     *
     * @param   isButtonPressed đọc tín hiệu nhấn nút
     * @return  true
     * @return  false
     */
    bool isButtonHeld3s(bool (*isButtonPressed)());
};