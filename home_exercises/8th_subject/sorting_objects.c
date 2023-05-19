#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEST 1

typedef struct {
    int day; 
    int month; 
    int year;
} Date;

/////////////////////////////////////////////////////////////////
// 8.1 funkcja bsearch2

#define FOOD_MAX  30   // max. liczba artykułów
#define ART_MAX   15   // max. długość nazwy artykułu +1
#define RECORD_MAX 40  // max. długość rekordu (linii) w pliku

typedef struct {
    char art[ART_MAX];
    float price;
    float amount;
    Date valid_date;
} Food;

int calculate_date_difference(Date *first, Date *second) {
    struct tm first_time;
    first_time.tm_sec = 0;
    first_time.tm_min = 0;
    first_time.tm_hour = 0;
    first_time.tm_mday = first->day;
    first_time.tm_mon = first->month - 1;
    first_time.tm_year = first->year - 1900;
    first_time.tm_isdst = -1;
    struct tm second_time;
    second_time.tm_sec = 0;
    second_time.tm_min = 0;
    second_time.tm_hour = 0;
    second_time.tm_mday = second->day;
    second_time.tm_mon = second->month - 1;
    second_time.tm_year = second->year - 1900;
    second_time.tm_isdst = -1;
    return mktime(&first_time) - mktime(&second_time);
}

int same_names(char (*first_word)[15], char (*second_word)[15]) {
    char *first = first_word[0];
    char *second = second_word[0];
    return strcmp(first, second);
}

typedef int (*ComparFp)(const void *, const void *);

int compare_food(const void *first, const void *second) {
    Food first_product = *(Food *)first;
    Food second_product = *(Food *)second;
    int date_difference = calculate_date_difference(&first_product.valid_date, &second_product.valid_date);
    int same_name = same_names(&first_product.art, &second_product.art);
    if (first_product.price == second_product.price && same_name == 0 && date_difference == 0) {
        return 0;
    }
    if (date_difference == 0) {
        return (first_product.price - second_product.price) < 0 ? -1 : 1;
    }
    return date_difference < 0 ? -1 : 1;
}
 
void *bsearch2(const void *key, void *base, size_t nitems, size_t size, ComparFp compar, int *result) {
    if (nitems == 0) {
        *result = 0;
        return base;
    }
    if (nitems == 1) {
        int comparison = (*compar)(key, base);
        switch (comparison) {
            case -1: {
                *result = 0;
                return base;
            }
            case 0: {
                *result = 1;
                return base;
            }
            case 1: {
                *result = 0;
                return base + size;
            }
        }
    }
    char *range_start = base;
    int range_length = nitems;
    void *middle_addr = range_start + (range_length >> 1) * size;
    int comparison = (*compar)(key, middle_addr);
    while (range_length > 1) {
        if (comparison == 0) {
            *result = 1;
            return middle_addr;
        } else if (comparison == 1) {
            range_start = (char *)middle_addr + size;
        }
        range_length >>= 1;
        middle_addr = range_start + (range_length >> 1) * size;
        comparison = (*compar)(key, middle_addr);
    }
    if (comparison == 0) {
        *result = 1;
        return middle_addr;
    }
    *result = 0;
    return middle_addr;
}

void print_art(Food *p, size_t n, char *art) {
    for (int i = 0; i < n; i++) {
        Food article = *(p + i);
        if (strcmp(&article.art[0], art) == 0) printf("%f %f %d %d %d\n", article.price, article.amount, article.valid_date.day, article.valid_date.month, article.valid_date.year);
    }
    return;
}

Food* add_record(Food *tab, size_t tab_size, int *np, ComparFp compar, Food *new) {
    if (*np == 0) {
        *np = *np + 1;
        *tab = *new;
        return tab;
    }
    int result = 0;
    Food *new_address = bsearch2(new, tab, *np, tab_size, compar, &result);
    if (result == 1) {
        new_address->amount = new_address->amount + new->amount;
        return new_address;
    }
    if (*np + 1 > FOOD_MAX) return NULL;
    memmove(new_address + 1, new_address, tab_size * (*np - (new_address - tab)));
    *new_address = *new;
    *np = *np + 1;
    return new_address;
}
  
int read_stream(Food *tab, size_t size, int no, FILE *stream) {
    size = sizeof(Food);
    Food article;
    int scanned = 0;
    int items = 0;
    while (scanned < no) {
        fscanf(stream, "%s %f %f %d %d %d", &(article.art), &(article.price), &(article.amount), &(article.valid_date.day), &(article.valid_date.month), &(article.valid_date.year));
        add_record(tab, size, &items, &compare_food, &article);
        scanned++;
    }
}

int read_stream0(Food *tab, size_t size, int no, FILE *stream) {
}

float value(Food *food_tab, size_t n, Date curr_date, int anticip) {
}

/////////////////////////////////////////////////////////////////
// 8.3 Sukcesja

#define CHILD_MAX  20   // maksymalna liczba przesuwanej w tablicy grupy osób (elementów tablicy)

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
    enum Sex sex:1;
    enum BOOL pretendent:1;   // =no - nie pretenduje (panuje albo nie żyje) ale jest w zbiorze dla spójności relacji.
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
       char *par_name;
       int index;
} Parent;    // strukturę można rozbudować o pole liczby dzieci

const Date primo_date = { 28, 10, 2011 }; // Data zmiany kolejności sukcesji

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
}

void persons_shiftings(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
}

int cleaning(Person *person_tab,int n) {
}

void print_person(const Person *p) {
    printf("%s\n", p->name);
}

void print_persons(const Person *person_tab, int n) {
    for(int i=1; i<=n; ++i, ++person_tab) printf("%2d %12s %s\n", i, person_tab->name, person_tab->parent);
    return;
}

int create_list(Person *person_tab, int n) {
}

////////////////////////////////////////////////////////////////

int main(void)
{
    // Wszyscy potomkowie Jerzego VI (w kolejności przypadkowej):
    Person person_tab[33]={
    {"Charles III",M,no,14,11,1948,"Elizabeth II"},
    {"Anne",F,yes,15,8,1950,"Elizabeth II"},
    {"Andrew",M,yes,19,2,1960,"Elizabeth II"},
    {"Edward",M,yes,10,3,1964,"Elizabeth II"},
    {"David",M,yes,3,11,1961,"Margaret"},
    {"Sarah",F,yes,1,5,1964,"Margaret"},
    {"William",M,yes,21,6,1982,"Charles III"},
    {"Henry",M,yes,15,9,1984,"Charles III"},
    {"Peter",M,yes,15,11,1977,"Anne"},
    {"Zara",F,yes,15,5,1981,"Anne"},
    {"Beatrice",F,yes,8,8,1988,"Andrew"},
    {"Eugenie",F,yes,23,3,1990,"Andrew"},
    {"James",M,yes,17,12,2007,"Edward"},
    {"Louise",F,yes,8,11,2003,"Edward"},
    {"Charles",M,yes,1,7,1999,"David"},
    {"Margarita",F,yes,14,5,2002,"David"},
    {"Samuel",M,yes,28,7,1996,"Sarah"},
    {"Arthur",M,yes,6,5,2019,"Sarah"},
    {"Georg",M,yes,22,7,2013,"William"},
    {"George VI",M,no,14,12,1895,NULL},
    {"Charlotte",F,yes,22,5,2015,"William"},
    {"Louis",M,yes,23,4,2018,"William"},
    {"Archie",M,yes,6,5,2019,"Henry"},
    {"Lilibet",F,yes,4,6,2021,"Henry"},
    {"Savannah",F,yes,29,12,2010,"Peter"},
    {"Isla",F,yes,29,3,2012,"Peter"},
    {"Mia",F,yes,17,1,2014,"Zara"},
    {"Lena",F,yes,18,6,2018,"Zara"},
    {"Elizabeth II",F,no,21,4,1925,"George VI"},
    {"Margaret",F,no,21,8,1930,"George VI"},
    {"Lucas",M,yes,21,3,2021,"Zara"},
    {"Sienna",F,yes,18,9,2021,"Beatrice"},
    {"August",M,yes,9,2,2021,"Eugenie"}
    };
    
    int to_do, no;
    size_t size, n;
        Food food_tab[FOOD_MAX];
        char buff[ART_MAX];
        FILE *file;
    if(TEST) printf("Wpisz nr zadania (1 - 3) ");
    scanf("%d", &to_do);

    switch (to_do) {
    case 1:  // bsearch2
        if (TEST) printf("Wpisz liczbe linii danych: ");
        scanf("%d",&no); getchar();
        if(TEST) file = stdin;
        else {
            scanf("%s",buff);
            file = fopen(buff,"r");
//            file = fopen("foods0.txt","r");
            if(file==NULL) { printf("Error 1\n"); break; }
        }
        if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n",no);
        n = read_stream(food_tab,FOOD_MAX,no,file);
        if (TEST) printf("Wpisz nazwe artykulu: ");
        scanf("%s",buff);
        print_art(food_tab,n,buff);
        break;
    case 2: // proste sortowanie struktur
        if (TEST) printf("Wpisz liczbe linii danych: ");
        scanf("%d",&no); getchar();
        if(TEST) file = stdin;
        else {
            scanf("%s",buff);
            file = fopen(buff,"r");
//            file = fopen("foods0.txt","r");
            if(file==NULL) { printf("Error 1\n"); break; }
        }
        if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n",no);
        n = read_stream0(food_tab,FOOD_MAX,no,file);
        Date curr_date;
        int anticip;
        if (TEST) printf("Wpisz date odniesienia dd mm yyyy: ");
        scanf("%d %d %d",&curr_date.day,&curr_date.month,&curr_date.year);
        if (TEST) printf("Wpisz roznice dni: ");
        scanf("%d",&anticip);
        printf("%.2f\n",value(food_tab,n,curr_date,anticip));
        break;
    case 3: // sukcesja
        if(TEST==1) printf("Wpisz pozycję w kolejce (liczona od 1): ");
        scanf("%d",&no);
    	int no_persons = sizeof(person_tab) / sizeof(Person);
        no_persons = create_list(person_tab,no_persons);
        if(TEST) print_persons(person_tab,no_persons);
        else print_person(person_tab+no-1);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
    }
    printf("natural end");
    return 0;
}

