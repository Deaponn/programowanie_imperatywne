#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR  -1
#define FILE_OPEN_ERROR          -2
#define LIST_ERROR               -3
#define TEST 0               // 1 dla testowania, 0 dla automatycznej oceny

typedef struct tagListElement {
    struct tagListElement *next;
    void                  *data;
} ListElement;

typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*,const void*);

typedef struct tagList {
    ListElement *head;
    ListElement *tail;
    int         size;
    ConstDataFp dumpData;
    DataFp      freeData;
    CompareDataFp compareData;
    DataFp      modifyData;
} List;

////////////////////////////////////////////////

// Funkcje pomocnicze
void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	printf("malloc error\n");
	exit(MEMORY_ALLOCATION_ERROR);
}
char *safe_strdup(char *str) {
	char *ptr = strdup(str);
	if (ptr) return ptr;
	printf("safe_strdup error\n");
	exit(MEMORY_ALLOCATION_ERROR);
}

////////////////////////////////////////////////////////////////
// Funkcje uniwersalne (ogolnego przeznaczenia) obsługi listy

// Inicjuje listę
void init_List(List *list, ConstDataFp dump, DataFp free, CompareDataFp cmp, DataFp modify) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->dumpData = dump;
    list->freeData = free;
    list->compareData = cmp;
    list->modifyData = modify;
}

// Wypisuje dane elemntów listy
void dumpList(const List* list) {
    if(list->dumpData == NULL) return;
    ListElement *i;
    for(i=list->head; i!=0; i=i->next) list->dumpData(i->data);
}

// Zwalnia pamięć elementów listy i danych
void freeList(List *list) {
    ListElement *current = 0;
    current = list->head;
    while(current) {
        ListElement *todelete = current;
        current = current->next;
        if(list->freeData) list->freeData(todelete->data); // zwolnienie pamięci struktury data
        free(todelete);              // zwolnienie pamięci elementu
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Uniwersalne funkcje obsługi elementów listy

// Dodaje element na początku listy
void pushFront(List *list, void *data) {
    ListElement* new_head = safe_malloc(sizeof(ListElement));
    new_head->data = data;
    if (list->head == NULL) {
        list->head = new_head;
        list->tail = new_head;
        new_head->next = NULL;
    } else {
        new_head->next = list->head;
        list->head = new_head;
    }
}

// Dodaje element na końcu listy
void pushBack(List *list, void *data) {
    if (list->tail == NULL) {
        pushFront(list, data);
        return;
    }
    ListElement* new_tail = safe_malloc(sizeof(ListElement));
    new_tail->next = NULL;
    new_tail->data = data;
    list->tail->next = new_tail;
    list->tail = new_tail;
}

// Usuwa pierwszy element listy
void popFront(List *list) {
    if (list->head == NULL) return;
    ListElement *old_head = list->head;
    list->head = old_head->next;
    free(old_head);
}

// Odwraca kolejność elementów listy
void reverse(List *list) {
    if (list->head == NULL) return;
    ListElement *prev_element = NULL;
    ListElement *curr_element = list->head;
    ListElement *next_element;
    list->head = list->tail;
    list->tail = curr_element;
    while (curr_element != NULL) {
        next_element = curr_element->next;
        curr_element->next = prev_element;
        prev_element = curr_element;
        curr_element = next_element;
    }
}

// Funkcje pomocnicze dla list uporzadkowanych 
// (porządek wyznacza funkcja wskazana polem compareData)


int compare_int(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}


// Zwraca element w posortowanej liście, a jeżeli nie ma, to element poprzedzający (nie ma, to NULL)
ListElement* findInsertionPoint(const List *list, ListElement *element) {
    if (list->head == NULL || list->compareData(list->head->data, element->data) < 0) return NULL;
    ListElement *shadow = list->head;
    ListElement *head = shadow->next;
    while (head != NULL) {
        int comparison = list->compareData(head->data, element->data);
        if (comparison == 0) return head;
        if (comparison < 0) return shadow;
        shadow = head;
        head = head->next;
    }
    return list->tail;
}


// Drugi parametr przekazuje do funkcji adres przydzielonej dla "nowych" danych pamięci.
// Jeżeli takie dane są już w elemencie listy, to dane tego elementu są modyfikowane
// funkcją wskazaną w polu modifyData, a pamięć "nowych" danych jest zwalniana.
// Jeżeli w liście takiego elementu nie ma, to nowy element jest tworzony
// i dopisany do listy zachowując uporządkowanie listy.
void insertInOrder(List *list, void *a) {
    ListElement *new_element = safe_malloc(sizeof(ListElement));
    new_element->data = a;
    ListElement *insertion_point = findInsertionPoint(list, new_element);
    if (insertion_point == NULL) {
        pushFront(list, a);
        free(new_element);
        return;
    }
    if (list->compareData(insertion_point->data, new_element->data) == 0) {
        if (list->modifyData != NULL) list->modifyData(insertion_point->data);
        return;
    }
    if (insertion_point->next == NULL) list->tail = new_element;
    new_element->next = insertion_point->next;
    insertion_point->next = new_element;
}


/////////////////// Funkcje do zadania 11.1.2

// Dodaje element na końcu listy bez korzystania z pola tail
void pushBack_v0(List *list, void *data) {
    ListElement *head = list->head;
    if (head == NULL) {
        pushFront(list, data);
        return;
    }
    while (head->next != NULL) head = head->next;
    ListElement *new_element = safe_malloc(sizeof(ListElement));
    new_element->data = data;
    new_element->next = NULL;
    head->next = new_element;
}


// Funkcje warstwy obsługi danych wskazywanych w polach struktury List


//////  Dla zadania 11.1.1 i 11.1.2
// 1. Dana jest liczbą typu int

typedef int DataInt;

void dump_int(const void *d) {
    printf("%d ", *(int *)d);
}

void free_int(void *d) {
    if (d) free(d);
}

int cmp_int(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}

// Przydziela pamięć i zapisuje w niej daną o wartości v
void *create_data_int(int v) {
    int *number = safe_malloc(sizeof(int));
    *number = v;
    return number;
}

//////  Dla zadania 11.1.3 i 11.1.4
// 2. Dana jest zapisana w  strukturze typu DataWord

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

int is_letter(char *letter) {
    int letter_code = (int)(*letter);
    if (letter_code < 65 || letter_code > 122 || (letter_code < 97 && letter_code > 90)) return 0;
    return 1;
}

void dump_word (const void *d) {
    DataWord word = *(DataWord *)d;
    printf("%s ", word.word);
}

void dump_word_lower (const void *d) {
    DataWord word = *(DataWord *)d;
    for (char *i = word.word; *i != '\0'; i++) *i = tolower(*i);
    printf("%s ", word.word);
}

void free_word(void *d) {
    DataWord *word_d = (DataWord *)d;
    if (word_d && word_d->word) free(word_d->word);
    if (word_d) free(word_d);
}

int cmp_word_alphabet(const void *a, const void *b) {
    char *first = (*(DataWord*)a).word;
    char *second = (*(DataWord*)b).word;
    while (tolower(*first) == tolower(*second) && *first != '\0' && *second != '\0') {
        first++;
        second++;
    }
    if (tolower(*first) == tolower(*second)) return 0; // both \0
    if (*first == '\0') return 1;
    if (*second == '\0') return -1;
    if ((int)tolower(*first) < (int)tolower(*second)) return 1;
    return -1;
}

int cmp_word_counter(const void *a, const void *b) {
    DataWord first = *(DataWord *)a;
    int second = *(int *)b;
    return second - first.counter;
}

void modify_word(void *a) {
    ((DataWord *)a)->counter++;
}

// Wypisz dane elementów spełniających warunek równości sprawdzany funkcją 
// wskazywaną w polu compareData nagłówka listy
void dumpList_word_if(List *plist, int n) {
    for (ListElement *i = plist->head; i != NULL; i = i->next) {
        if (plist->compareData(i->data, &n) == 0) plist->dumpData(i->data);
    }
}

// Przydziela pamięć dla łańcucha string i struktury typu DataWord.
// Do przydzielonej pamięci wpisuje odpowiednie dane.
// Zwraca adres struktury.
void *create_data_word(char *string, int counter) {
    DataWord *new_element = safe_malloc(sizeof(DataWord));
    char *copied_word = safe_strdup(string);
    new_element->word = copied_word;
    new_element->counter = counter;
    return new_element;
}

//////////////////////////////////////////////////

/////////////////// Funkcje pomocnicze dla zadan 11.1.3 i 11.1.4

// Wyrazy ze strumienia wejściowego zapisuje w elementach listy
// w kolejności określonej ostatnim parametrem funkcji (cmp)
// - adresem funkcji porównującej typu CompareDataFp. Adres zerowy oznacza, 
// że kolejność elementów listy jest zgodna z kolejnością 
void stream_to_list(List *list, FILE *stream, CompareDataFp cmp) {
    DataWord *data;
    char *p, buf[BUFFER_SIZE];
    char delimits[] = " \r\t\n.,?!-;";
    if(cmp) list->compareData = cmp;
    while(fgets(buf,BUFFER_SIZE,stream)) {
        p = strtok(buf, delimits);
        while(p) {
            data = create_data_word(p, 1); 
            if(cmp) insertInOrder(list,data);
            else    pushBack(list, data);
            p = strtok(NULL,delimits);
        }
    }
}

void list_test(List *plist, int n) {
	char op[2];
	int v;
	ListElement element, *pelement;
	DataInt data;
	for (int i = 0; i < n; ++i) {
        if(TEST) printf("Wpisz kod polecenia \n");
		scanf("%s", op);
		switch (op[0]) {
			case 'f': // dodaj na początku listy
                if(TEST) printf("Wpisz wartosc \n");
				scanf("%d", &v);
				pushFront(plist,create_data_int(v));
				break;
			case 'b': // dodaj na końcu listy
                if(TEST) printf("Wpisz wartosc \n");
				scanf("%d", &v);
				pushBack(plist,create_data_int(v));
				break;
			case 'd': // usuń pierwszy element
			    popFront(plist);
				break;
            case 'r': // odwróć kolejność
                reverse(plist);
                break;
			case 'i': // dodaj wg porządku rosnącego (int) lub alfabetycznego (char),
			          // jeżeli element już istnieje, to akcja zależna od typu danych
                if(TEST) printf("Wpisz wartosc \n");
				scanf("%d",&v);
				insertInOrder(plist,create_data_int(v));
                break;
			default:
				printf("No such operation: %s\n", op);
				break;
		}
	}
}

int main(void) {
    int to_do, n, size;
    char buf[257];
    clock_t t_start, t_end;
    double t_total;
    FILE *stream;
    List list;
    ListElement *head;

	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
	switch (to_do) {
case 1:
	    if(TEST) printf("Wpisz liczbe polecen ");
        scanf("%d",&n);
        head = NULL;
        init_List(&list, dump_int, free_int, cmp_int, NULL);
        list_test(&list, n);
        dumpList(&list);
        freeList(&list);
        break;
case 2:
        if(TEST) printf("Wpisz liczbe elementow ");
        scanf("%d",&size);
        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushBack_v0(&list,create_data_int(i));
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Back bez tail.  Czas = %g s.\n",size,t_total);
        freeList(&list);

        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushFront(&list,create_data_int(i));
        reverse(&list);
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Front + revers. Czas = %g s.\n",size,t_total);
        freeList(&list);

        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushBack(&list,create_data_int(i));
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Back z tail.    Czas = %g s.\n",size,t_total);
        freeList(&list);
        break;
case 3:      // wypisz wszystkie słowa wczytane do elementów listy
        init_List(&list,dump_word,free_word,NULL,NULL);
        if(TEST) {
            stream = stdin;
            printf("Wpisz linie tekstu\n");
            stream_to_list(&list, stream, NULL);
            rewind(stream);
        }
        else {
            stream = fopen("Tekst_1.txt","r");
            if(stream == NULL) exit(FILE_OPEN_ERROR);
            stream_to_list(&list, stream, NULL);
            fclose(stream);
        }
        dumpList(&list);
        freeList(&list);
        break;
case 4:      // wypisz w porządku alfabetycznym słowa o zadanej krotności
        if(TEST) printf("Wpisz krotnosc slowa ");
        scanf("%d",&n);
        init_List(&list,dump_word_lower,free_word,NULL,modify_word);
        if(TEST) {
            stream = stdin;
            printf("Wpisz linie tekstu\n");
            stream_to_list(&list, stream, cmp_word_alphabet);
            rewind(stream);
        }
        else {
            stream = fopen("Tekst_C.txt","r");
            if(stream == NULL) exit(FILE_OPEN_ERROR);
            stream_to_list(&list, stream, cmp_word_alphabet);
            fclose(stream);
        }
        list.compareData = cmp_word_counter;
        dumpList_word_if(&list,n); printf("\n");
        freeList(&list);
        break;
default:
		printf("NOTHING TO DO FOR %d\n", to_do);
	}
    return 0;
}
