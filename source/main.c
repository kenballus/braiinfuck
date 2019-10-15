#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include "bf.h"

static u32 *xfb;
static GXRModeObj *rmode;

const uint MAX_INPUT_SIZE = 100000;

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
    printf("BraiinFuck\nCopyright (c) 2019 Ben Kallus\n");

    char input[MAX_INPUT_SIZE];
    uint i;
    for (i = 0; i < MAX_INPUT_SIZE; i++) {
        input[i] = 0;
    }
    uint input_index = 0; // The index of the location one after the most recently-typed character

    while (1) {
        WPAD_ScanPads();

        u16 buttons_down = WPAD_ButtonsDown(0);
        if (buttons_down & WPAD_BUTTON_A) {
            printf("[");
            input[input_index++] = '[';
        }
        else if (buttons_down & WPAD_BUTTON_B) {
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
        else if (buttons_down & WPAD_BUTTON_2) {
            printf(".");
            input[input_index++] = '.';
        }
        // else if (buttons_down & WPAD_BUTTON_1) {
        //     printf(",");
        //     input[input_index++] = ',';
        // }
        else if (buttons_down & WPAD_BUTTON_UP) {
            printf("<");
            input[input_index++] = '<';
        }
        else if (buttons_down & WPAD_BUTTON_DOWN) {
            printf(">");
            input[input_index++] = '>';
        }
        else if (buttons_down & WPAD_BUTTON_LEFT) {
            if (input_index > 0) {
                if (input_index % 80 == 0) { // Backspacing a line (DOESN'T WORK)
                    printf("\x1b[%i;79H", (input_index / 80) + 1);
                }
                printf("\b \b");
                input[input_index--] = 0;
            }
        }

        else if (buttons_down & WPAD_BUTTON_HOME) {
            printf("\n");
            input[input_index] = 0; // Just to be SUPER sure it's null-terminated
            interpret(input);
            for (i = 0; i < input_index; i++) {
                input[i] = 0;
            }
            input_index = 0;
        }

    }
 
    return 0;
}