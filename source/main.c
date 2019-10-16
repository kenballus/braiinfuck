/*
Copyright  (C) 1993 Urban Müller
Copyright? (C) 2019 Ben Kallus
Braiinfuck!

I, Ben Kallus, permit you to do whatever you want with this software.
I am not responsible for what you do with this software; you are.
There is no warranty at all.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include "bf.h"

static u32 *xfb;
static GXRModeObj *rmode;

const int MAX_INPUT_SIZE = 100000;

void init() {
    VIDEO_Init();
    WPAD_Init();
 
    // Get the video mode (set in Wii system settings)
    rmode = VIDEO_GetPreferredMode(NULL);

    // Allocate memory for the display in the uncached region
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    // Necessary for printf
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth*VI_DISPLAY_PIX_SZ);
 
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}

int main() {
    init();
 
    printf("\x1b[0;0H"); // Start at Row;Col
    printf("Braiinfuck!\nCopyright? (C) 2019 Ben Kallus\n");

    char input[MAX_INPUT_SIZE];
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        input[i] = 0;
    }

    // char prev_input[MAX_INPUT_SIZE];
    // for (int i = 0; i < MAX_INPUT_SIZE; i++) {
    //     prev_input[i] = 0;
    // }

    int input_index = 0; // The index of the location one after the most recently-typed character
    // int prev_input_index = -1;

    while (1) {
        WPAD_ScanPads();

        u16 buttons_down = WPAD_ButtonsDown(0);
        if (buttons_down & WPAD_BUTTON_LEFT) {
            printf(",");
            input[input_index++] = ',';
        }
        else if (buttons_down & WPAD_BUTTON_1) {
            printf("[");
            input[input_index++] = '[';
        }
        else if (buttons_down & WPAD_BUTTON_2) {
            printf("]");
            input[input_index++] = ']';
        }
        else if (buttons_down & WPAD_BUTTON_PLUS) {
            printf("+");
            input[input_index++] = '+';
        }
        else if (buttons_down & WPAD_BUTTON_MINUS) {
            printf("-");
            input[input_index++] = '-';
        }
        else if (buttons_down & WPAD_BUTTON_A) {
            printf(".");
            input[input_index++] = '.';
        }
        else if (buttons_down & WPAD_BUTTON_UP) {
            printf("<");
            input[input_index++] = '<';
        }
        else if (buttons_down & WPAD_BUTTON_DOWN) {
            printf(">");
            input[input_index++] = '>';
        }

        // else if (buttons_down & WPAD_BUTTON_RIGHT) {
        //     char* temp = input;
        //     input = prev_input;
        //     prev_input = temp;
        //     for (int i = 0; i < input_index; i++) {
        //         backspace();
        //     }
        //     int temp2 = input_index;
        //     input_index = prev_input_index;
        //     prev_input_index = temp2;
        // }

        else if (buttons_down & WPAD_BUTTON_B) {
            if (input_index > 0) {
                if (input_index % 80 == 0) {
                    printf("\x1b[1A\x1b[79C \x1b[1A\x1b[79C");
                }
                else {
                    printf("\b \b");
                }
                input[input_index--] = 0;
            }
        }
        else if (buttons_down & WPAD_BUTTON_HOME) {
            input[input_index] = 0; // Just to be SUPER sure it's null-terminated
            interpret(input);
            for (int i = 0; i <= input_index; i++) { // <= because we want the null terminator
                // prev_input[i] = input[i];
                input[i] = 0;
            }
            // prev_input_index = input_index;
            input_index = 0;
        }
    }
 
    return 0;
}