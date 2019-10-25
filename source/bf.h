#ifndef BF
#define BF

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <wiiuse/wpad.h>

const int ARRAY_SIZE = 30000;
const int ARRAY_PRINTED = 16;

void interpret(char ifile[]) {
    u8* array = (u8*)malloc(sizeof(u8) * ARRAY_SIZE); // The tape
    int index = 0; // Where we are on the tape

    char* output_string = (char*)malloc(sizeof(char) * ARRAY_SIZE);
    int output_index = 0;

    int i;
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = 0;
        output_string[i] = 0;
    }

    int input_index; // Where we are in the input
    for (input_index = 0; ifile[input_index] != 0; input_index++) {
        char chr = ifile[input_index]; // The current input character

        switch (chr) {
        // Increment value
        case '+':
            array[index]++;
            break;

        // Decrement value
        case '-':
            array[index]--;
            break;

        // Increment pointer
        case '>':
            index++;
            if (index >= ARRAY_SIZE) {
                printf("ERROR: Indexed off the right end of the tape\n");
                return;
            }
            break;

        // Decrement pointer
        case '<':
            index--;
            if (index < 0) {
                printf("ERROR: Indexed off the left end of the tape\n");
                return;
            }
            break;

        // Output
        case '.':
            output_string[output_index++] = array[index];
            break;

        // Input
        case ',':
            // array[index] = getchar(); // For keyboard
            printf("Select a character:  ");
            char input_char = ' ';
            while (1) {
                WPAD_ScanPads();
                u16 buttons_down = WPAD_ButtonsDown(0);
                if (buttons_down & WPAD_BUTTON_RIGHT) {
                    input_char++;
                    if (input_char > '~') input_char = ' ';
                    printf("\b%c", input_char);
                }
                else if (buttons_down & WPAD_BUTTON_LEFT) {
                    input_char--;
                    if (input_char < ' ') input_char = '~';
                    printf("\b%c", input_char);
                }
                else if (buttons_down & WPAD_BUTTON_HOME) {
                    printf("\n");
                    array[index] = input_char;
                    break;
                }
            }
            break;

        // Begin loop
        case '[':
            if (array[index] == 0) { // Skip to the corresponding ']'
                int counter = 1;
                while (counter) {
                    input_index++;
                    if (ifile[input_index] == 0) {
                        printf("ERROR: \"[\" without corresponding \"]\"\n");
                        return;
                    }
                    if (ifile[input_index] == ']') {
                        counter--;
                    }
                    else if (ifile[input_index] == '[') {
                        counter++;
                    }
                }
            }
            break;

        // End loop
        case ']':
            if (array[index] != 0) { // Rewind to the corresponding '['
                int counter = 1;
                while (counter) {
                    input_index--;
                    if (input_index < 0) {
                        printf("ERROR: \"]\" without corresponding \"[\"\n");
                        return;
                    }
                    if (ifile[input_index] == ']') {
                        counter++;
                    }
                    else if (ifile[input_index] == '[') {
                        counter--;
                    }
                }
            }
            break;
        }
    }

    int start, end, mode;

    // Set the range to print
    if (index <= ARRAY_PRINTED) {
        start = 0;
        end = index + ARRAY_PRINTED + 1;
        mode = 0; // beginning
    }
    else if (index < ARRAY_SIZE - ARRAY_PRINTED) {
        start = index - ARRAY_PRINTED;
        end = index + ARRAY_PRINTED + 1;
        mode = 1; // middle
    }
    else {
        start = index - ARRAY_PRINTED;
        end = ARRAY_SIZE;
        mode = 2; // end
    }

    if (output_index != 0) {
        output_string[output_index] = 0; // Null-terminate
        printf("%s\n", output_string);
    }

    printf("=> ");

    if (mode != 0) {
        printf("... ");
    }

    for (i = start; i < end; i++) {
        if (i == index) {
            printf("[%i] ", array[i]);
        }
        else {
            printf("%i ", array[i]);
        }
    }

    if (mode != 2) {
        printf("...");
    }

    printf("\n");
    free(array);
    free(output_string);
}

#endif