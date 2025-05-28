#include "Services/UIMenuService.h"

UIMenuService::UIMenuService() {}

UIMenuService& UIMenuService::getInstance() {
  static UIMenuService instance;
  return instance;
}

void UIMenuService::setup() {
  // Hiển thị menu ban đầu
}

ModeType UIMenuService::selectMode() {
  // Lắng nghe điều hướng menu và trả về chế độ
  return ModeType::NONE; // placeholder
}
