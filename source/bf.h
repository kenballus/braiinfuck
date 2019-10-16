#ifndef BF
#define BF

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

const int ARRAY_SIZE = 30000;

void interpret(char ifile[]) {
    int* array = (int*)malloc(sizeof(int) * ARRAY_SIZE); // The tape
    int index = 0; // Where we are on the tape

    int i;
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = 0;
    }

    int char_index; // Where we are in the input
    for (char_index = 0; ifile[char_index] != 0; char_index++) {
        char chr = ifile[char_index]; // The current input character

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
                printf("\nERROR: Indexed off the right end of the tape\n");
                return;
            }
            break;

        // Decrement pointer
        case '<':
            index--;
            if (index < 0) {
                printf("\nERROR: Indexed off the left end of the tape\n");
                return;
            }
            break;

        // Output
        case '.':
            putchar(array[index]);
            break;

        // Input
        case ',':
            array[index] = getchar();
            break;

        // Begin loop
        case '[':
            if (array[index] == 0) { // Skip to the corresponding ']'
                int counter = 1;
                while (counter) {
                    char_index++;
                    if (ifile[char_index] == 0) {
                        printf("\nERROR: \"[\" without corresponding \"]\"\n");
                        return;
                    }
                    if (ifile[char_index] == ']') {
                        counter--;
                    }
                    else if (ifile[char_index] == '[') {
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
                    char_index--;
                    if (char_index < 0) {
                        printf("\nERROR: \"]\" without corresponding \"[\"\n");
                        return;
                    }
                    if (ifile[char_index] == ']') {
                        counter++;
                    }
                    else if (ifile[char_index] == '[') {
                        counter--;
                    }
                }
            }
            break;
        }
    }
    int start, end, mode;

    if (index <= 10) {
        start = 0;
        end = index + 11;
        mode = 0; // beginning
    }
    else if (index < ARRAY_SIZE - 10) {
        start = index - 10;
        end = index + 11;
        mode = 1; // middle
    }
    else {
        start = index - 10;
        end = ARRAY_SIZE;
        mode = 2; // end
    }

    printf("\n=> ");

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
}

#endif