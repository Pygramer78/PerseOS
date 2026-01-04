#pragma once
#include <stdint.h>
#include <stdbool.h>
// Just a file that manages ascii related stuff
/*
I believe that this file does not require documentation for now
*/
// Normal table (no shift)
static const char normal[128] = {
    0,   0, '1','2','3','4','5','6','7','8',  // 0x00-0x09
    '9','0','-','=',0, 0, 'q','w','e','r',    // 0x0A-0x13
    't','y','u','i','o','p','[',']','\n',0,   // 0x14-0x1D
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\', // 0x1E-0x2B
    'z','x','y','c','v','b','n','m',',','.','/',0, // 0x2C-0x38
    ' ','\b','\t',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// shift table
static const char shifted[128] = {
    0,   0, '!','@','#','$','%','^','&','*',
    '(',')','_','+',0,0,'Q','W','E','R',
    'T','Y','U','I','O','P','{','}','\n',0,
    'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
    'Z','X','Y','C','V','B','N','M','<','>','?',0,
    ' ','\b','\t',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
char scancode_to_ascii(uint8_t sc, bool shift, bool capslock);