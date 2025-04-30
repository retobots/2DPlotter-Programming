#ifndef DISPLAY_MANAGER_HPP
#define DISPLAY_MANAGER_HPP

#include "config.hpp"

void setupDisplay();
void updateDisplayStatus(float x, float y, bool penIsDown, int mode);
void showError(const char *error);
void showProgress(int percent);

#endif