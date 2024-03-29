#include <stdio.h>
#include <math.h>

#define TEST    0        // 1 dla testowania, 0 dla oceny automatycznej
#define true    1
#define false   0

// Deklaracje (prototypy) funkcji:

double stack_push(double x);
double stack_pop(void);
int stack_state(void);

double queue_push(int how_many);
int queue_pop(int how_many);
int queue_state(void);
void queue_print(void);

double cbuff_push(int cli_nr);
int cbuff_pop(void);
int cbuff_state(void);
void cbuff_print(void);

///////////////////////

int main(void) {
    int to_do, i, n, ans;
    int nr_klienta, kod_operacji;
    double x,f_ans;
    if(TEST) printf("Nr testu = ");
    scanf("%d",&to_do);
    switch(to_do) {
case 1:
        if(TEST) printf("sekwencja operacji (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan stosu i konczy sekwencje operacji
        do {
            scanf("%lf",&x);
            if(x > 0) {
                f_ans = stack_push(x);
                if(f_ans == INFINITY) printf("%f ",INFINITY);
            }
            else {
                if(x < 0) printf("%.2f ",stack_pop());
                else      printf("\n%d\n",stack_state());
            }
        } while(x != 0.0);
        break;
case 2:
//  kolejka kolejnych liczb naturlnych > 0 (naplywajacych grupowo) - implementacja w tablicy z przesunieciami
        if(TEST) printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan kolejki i konczy symulacje kolejki
        do {
            scanf("%d",&n);
            if(n > 0) {
                f_ans = queue_push(n);
                if(f_ans== INFINITY) printf("%f ",INFINITY);
            }
            else {
                if(n < 0) {
                    ans = queue_pop(-n);
                    if(ans < 0) printf("-1 ");
                }
                else  {
                    printf("\n%d\n",queue_state());
                    queue_print();
                }
            }
        } while(n != 0);
        break;
case 3:
//  kolejka kolejnych liczb naturalnych > 0 (naplywajacych pojedynczo - implementacja w buforze cyklicznym
        if(TEST) printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan kolejki i konczy symulacje kolejki
        nr_klienta = 0;
        do {
            scanf("%d",&kod_operacji);
            if(kod_operacji > 0) {
                f_ans = cbuff_push(++nr_klienta);
                if(f_ans == INFINITY) printf("%f ",INFINITY);
            }
            else {
                if(kod_operacji < 0) {
                    printf("%d ",cbuff_pop());
//                    ans = cbuff_pop();
//                    if(ans < 0) printf("%f ",NAN);
//                    else        printf("%d ",ans);
                }
                else  {
                    printf("\n%d\n",cbuff_state());
                    cbuff_print();
                }
            }
        } while(kod_operacji != 0);
        break;
default:
        if(TEST) printf("NOTHING TO DO!\n");
    }
    return 0;
}

///////////////////////////////////////////////////////

// Definicje funkcji i zmiennych o zasięgu plikowym (file scope):

// Stos

#define STACK_SIZE  10

double stack[STACK_SIZE];
int top=0;   // jawne zerowanie nie jest konieczne

double stack_push(double x) {
    if (top >= STACK_SIZE) {
        return INFINITY;
    }
    stack[top] = x;
    top++;
    return 0;
}

double stack_pop(void) {
    if (top <= 0) {
        return NAN;
    }
    top--;
    return stack[top];
}

int stack_state(void) {
    return top;
}

//Kolejka z przesuwaniem w tablicy

#define QUEUE_SIZE  10

int queue[QUEUE_SIZE];
int in=0, curr_nr=0;  // 1. klient dostanie nr = 1

double queue_push(int how_many) {
    int overflow = false;
    for (int i = 0; i < how_many; i++) {
        curr_nr++;
        if (in < QUEUE_SIZE) {
            queue[in] = curr_nr;
            in++;
        } else overflow = true;
    }
    if (overflow) return INFINITY;
    return 0;
}

void shift_left(int array[], int length, int count) {
    for (int i = 0; i < length - count; i++) {
        array[i] = array[count + i];
    }
}

int queue_pop(int how_many) {
    if (in < how_many) {
        in = 0;
        return -1;
    }
    shift_left(queue, QUEUE_SIZE, how_many);
    in = in - how_many;
    return in;
}

int queue_state(void) {
    return in;
}

void queue_print(void) {
    for(int i = 0; i < in; i++) {
        printf("%d ", queue[i]);
    }
}


//Kolejka w buforze cyklicznym

#define CBUFF_SIZE  10

int cbuff[CBUFF_SIZE];
int out=0, len=0;

double cbuff_push(int cli_nr) {
    if (len >= CBUFF_SIZE) {
        return INFINITY;
    }
    if (out + len < CBUFF_SIZE) cbuff[out + len] = cli_nr;
    else cbuff[out + len - CBUFF_SIZE] = cli_nr;
    len++;
    return 0;
}

int cbuff_pop(void) {
    if (len <= 0) {
        return -1;
    }
    out++;
    len--;
    return cbuff[out - 1];
}

int cbuff_state(void) {
    return len;
}

void cbuff_print(void) {
    for (int i = 0; i < len; i++) {
        if (i + out < CBUFF_SIZE) printf("%d ", cbuff[i + out]);
        else printf("%d ", cbuff[i + out - CBUFF_SIZE]);
    }
}