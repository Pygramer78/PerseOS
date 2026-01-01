/*
VGA module. The closest thing you will ever have of stdio.h
Remember you can always chose to install stdio.h
*/
#pragma once
#include <stdint.h>
static volatile uint16_t* const VGA = (uint16_t*)0xB8000;
void print(const char* text);
void clear(void);