#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

using namespace std;

uint8_t* colorMapLoader(char* file, int* colorCount) {
    FILE *ptr;
    ptr = fopen(file, "r");

    if (ptr == NULL) {
        printf("No such file.");
        fclose(ptr);
        return 0;
    }

    int size;
    fscanf(ptr, "%d", &size);
    *colorCount = size;

    uint8_t *arr = new uint8_t[size * 3];
    for (int i = 0; i < size; i++) {
        fscanf(ptr, "%hhu %hhu %hhu", &arr[(i * 3)], &arr[(i * 3) + 1], &arr[(i * 3) + 2]);
    }

    fclose(ptr);

    return arr;
}