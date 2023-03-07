#include <stdio.h>

int main() {
    float number = 1;
    float accuracy = 1;
    float output = 1;
    printf("Podaj liczbe\n");
    scanf("%f", &number);
    printf("Podaj dokladnosc\n");
    scanf("%f", &accuracy);
    output = number;
    while (1 == 1) {
        float next = (output + (number / output)) / 2;
        if (output - next < accuracy) {
            break;
        }
        output = next;
    }
    printf("%3.7f \n", output);
    return 0;
}