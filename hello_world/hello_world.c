//
// Created by Deaponn on 01.03.2023.
//

#include <stdio.h>

char * prompt() {
    return "Basic setup done";
}

int main() {
    // printf() displays the string inside quotation
    printf("Hello, World!\n");
    printf(prompt());
    return 0;
}