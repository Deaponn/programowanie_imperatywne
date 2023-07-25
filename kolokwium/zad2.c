#include <stdlib.h>
#include <stdio.h>

// a)
enum Drive_type {combustion, electric, d_ndef};

enum Fuel_type {gasoline, diesel, gas, hydrogen, f_d_ndef};

enum Battery_type {Li_Ion, Na_Ion, NiMH, b_d_ndef};

typedef struct {
    char emission[15];
    enum Fuel_type fuel_t;
} Comb_data;

typedef struct {
    float capacity;
    enum Battery_type bat_t;
} El_data;

typedef struct {
    int d;
    int m;
    int y;
} Date;

union Drv_union {
    Comb_data cs;
    El_data es;
};

typedef struct {
    char owner[30];
    Date valid_date;
    enum Drive_type d_type;
    union Drv_union drv_union;
} Vehicle;

// f)
int cmp_date(const Date *first, const Date *second) {
    if (first->y < second->y) return 1;
    if (first->y > second->y) return -1;
    if (first->m < second->m) return 1;
    if (first->m > second->m) return -1;
    if (first->d < second->d) return 1;
    if (first->d > second->d) return -1;
    return 0;
}

// b)
int delayed(const Vehicle *tv, int size, const Date *base_date, int **pdelayed_owners) {
    int *delayed_owners = malloc(sizeof(int) * size);
    int delayed_owners_count = 0;
    for (int idx = 0; idx < size; idx++) {
        if (cmp_date(&tv->valid_date, base_date) > 0) {
            *(delayed_owners + delayed_owners_count) = idx;
            delayed_owners_count++;
        }
    }
    if (delayed_owners_count == 0) *pdelayed_owners = 0;
    else *pdelayed_owners = delayed_owners;
    return delayed_owners_count;
}

// c)
void print_owners(const Vehicle *tv, int *t_ind, size_t size) {
    for (int idx = 0; idx < size; idx++) {
        int pointed_idx = t_ind[idx];
        printf("%s\n", (tv[pointed_idx]).owner);
    }
}

// d)
Vehicle *create_vehicle(void) {
    Vehicle *new_vehicle = malloc(sizeof(Vehicle));
    new_vehicle->owner[0] = '\0';
    new_vehicle->valid_date.d = 1;
    new_vehicle->valid_date.m = 1;
    new_vehicle->valid_date.y = 1970;
    new_vehicle->d_type = d_ndef;
}

// e)
void print_drive_data(Vehicle *v) {

}

// g)
// funkcji cmp_date nie trzeba modyfikować
// qsort(Vtab, SIZE, sizeof(Vehicle), cmp_date);

int main() {
    return 0;
}