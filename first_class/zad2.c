#include <stdio.h>

int main() {
    int first = 1;
    int second = 1;
    printf("Podaj pierwsza liczbe\n");
    scanf("%d", &first);
    printf("Podaj druga liczbe\n");
    scanf("%d", &second);
    if (first < second) {
        int placeholder = first;
        first = second;
        second = placeholder;
    }
    while (second != 0) {
        if (first < second) {
            int placeholder = first;
            first = second;
            second = placeholder;
        }
        first -= second;
    }
    printf("NWD to: %d", first);
    return 0;
}