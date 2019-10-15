#ifndef STACK
#define STACK

#include <stdlib.h>
#include <stdio.h>
#include "bf.h"

struct stack
{
    uint  capacity;
    uint  top;
    int* items;
};

struct stack* new_stack(uint start_capacity) {
    if (start_capacity < 1) exit(EXIT_FAILURE);
    struct stack *baby = (struct stack*)malloc(sizeof(struct stack));

    baby->capacity = start_capacity;
    baby->top = -1;
    baby->items = (int*)malloc(sizeof(int) * start_capacity);

    return baby;
}

void destroy_stack(struct stack* s) {
    free(s->items);
    free(s);
}

uint size(struct stack* s) {
    return s->top + 1;
}

uint empty(struct stack* s) {
    return s->top == -1;
}

uint full(struct stack* s) {
    return s->top == s->capacity - 1;
}

void grow(struct stack* s) {
    int* new_items = (int*)malloc(sizeof(int) * s->capacity * 2);
    uint i;
    for (i = 0; i < s->capacity; i++) {
        new_items[i] = s->items[i];
    }

    free(s->items);
    s->capacity *= 2;
    s->items = new_items;
}

int peek(struct stack* s) {
    if (!empty(s)) return s->items[s->top];
    else exit(EXIT_FAILURE);
}

int pop(struct stack* s) {
    if (empty(s)) exit(EXIT_FAILURE);
    else return s->items[s->top--];
}

void push(struct stack* s, int i) {
    if (full(s)) grow(s);
    s->items[++s->top] = i;
}

#endif