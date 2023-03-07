#include <stdio.h>

int main() {
    int count = 0;
    int first = 0;
    int second = 1;
    int placeholder;
    printf("Podaj liczbe wyrazow ciagu Fibonacciego\n");
    scanf("%d", &count);
    if (count <= 0){
        printf("Niepoprawna ilosc wyrazow ciagu");
        return 1;
    }
    for(int i = 0; i < count; i++) {
        printf("%d ", first);
        placeholder = first;
        first = second;
        second += placeholder;
    }
    return 0;
}