#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 1     // 1 - dla testow,  0 - dla oceny automatycznej

// 5.1.1

#define STRLEN_MAX  SIZE
#define KW_NO   32    // liczba zapisanych lancuchow (slow kluczowych)

// n lancuchów wskazywanych w tablicy wskaznikow ptab kopiuje do tablicy tablic t2D   
void n_str_copy(char t2D[][STRLEN_MAX], char *ptab[], size_t n) {
    int word_len = 0;
    for (int i = 0; i < n; i++) {
        int current_len = strlen(*ptab + word_len + i);
        for (int j = 0; j < current_len; j++) {
            t2D[i][j] = (*ptab + word_len + i)[j];
        }
        t2D[i][current_len] = '\0';
        word_len += current_len;
        // printf("%s", t2D[i]);
    }
}

int compar(const void *p1, const void *p2) {
}

// returns 1 if word1 is after word2 alphabetically
// returns -1 if word2 is after word1 alphabetically
// returns 0 if word1 and word2 are equal
int word_compare(const char *word1, const char *word2) {
    int word1len = strlen(word1);
    int word2len = strlen(word2);
    int smaller = word1len;
    if (smaller > word2len) smaller = word2len;
    for (int i = 0; i < smaller; i++) {
        if ((*word1 + i) > (*word2 + i)) return 1;
        if ((*word2 + i) > (*word1 + i)) return -1;
    }
    if (word1len == word2len) return 0;
    if (word1len > word2len) return 1;
    return -1;
}

// sortuje alfabetycznie n lancuchow wskazywanych w tablicy wskaznikow t  
void ptab_sort(char *ptab[], size_t n) {
    int is_sorted = 0;
    for (int i = 0; i < n; i++) {
        int total_words_len = 0;
        is_sorted = 1;
        for (int j = 0; j < (n - i - 1); j++) {
            char* word1 = *(ptab + j);
            char* word2 = *(ptab + j + 1);
            if (word_compare(word1, word2) >= 0) {
                is_sorted = 0;
                char* placeholder = *(ptab + j);
                *(ptab + j) = *(ptab + j + 1);
                *(ptab + j + 1) = placeholder;
            }
        }
        if (is_sorted) {
            return;
        }
    }
}

// Porzadek odwrotny do alfabetycznego lancuchow zapisanych w tablicy t2D zapisuje w tablicy indices
void t2D_sort(const char t2D[][STRLEN_MAX], size_t indices[], size_t n) {
    for (int i = 0; i < n; i++) indices[i] = i;

    int is_sorted = 0;
    for (int i = 0; i < n; i++) {
        is_sorted = 1;
        for (int j = 0; j < (n - i - 1); j++) {
            const char* word1 = t2D[indices[j]];
            const char* word2 = t2D[indices[j + 1]];
            if (word_compare(word1, word2) <= 0) {
                is_sorted = 0;
                int placeholder = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = placeholder;
            }
        }
        if (is_sorted) {
            return;
        }
    }
}

// W wierszach tablicy t2D sa zapisane lancuchy znakowe w dowolnej kolejnosci.
// Tablica indeksow wierszy indices wyznacza porzadek tych lancuchow.
// Funkcja wypisuje w osobnych liniach łańcuchy wskazane przez n poczatkowych elementów tablicy indices.
void print_t2D_ind(const char (*ptr)[STRLEN_MAX], const size_t *pindices, size_t n) {
    for (int i = 0; i < n; i++) {
        int string_start = *(pindices + i);
        int character = 0;
        while ((*ptr + string_start * STRLEN_MAX)[character] != '\0') {
            printf("%c", (*ptr + string_start * STRLEN_MAX)[character]);
            character++;
        }
        printf("\n");
    }
}

// Funkcja wypisuje w osobnych liniach n łańcuchów wskazywanych przez elementy tablicy ptab.
void print_ptab(char *ptab[], size_t n) {
    for (int i = 0; i < n; i++) {
        printf("%s\n", *(ptab + i));
    }
}

// 5.1.2
// A mxp, B pxn

void mat_product(const double A[][SIZE], const double B[][SIZE], double AB[][SIZE], size_t m, size_t p, size_t n) {
    for (int y = 0; y < m; y++) {
        for (int x = 0; x < n; x++) {
            int dot_product = 0;
            for (int q = 0; q < p; q++) {
                dot_product += A[y][q] * B[q][x];
            }
            AB[y][x] = dot_product;
        }
    }
}


// 5.1.3 Triangulatyzacja macierzy i obliczanie wyznacznika - wersja uproszczona
// (bez zamiany wierszy).
// Jezeli element glowny a[i][i] = 0, to triangularyzacja nie jest dokonczona,
// a wyznacznik = NAN
// Zalozenie: funkcja gauss moze zmienic wartosci elementow tablicy A

double gauss_simplified(double A[][SIZE], size_t n) {
    double determinant = 1;
    for (int row = 0; row < n; row++) {
        double main = A[row][row];
        determinant *= main;
        for (int next = row + 1; next < n; next++) {
            double multiplier = A[next][row] / main;
            for (int item = row; item < n; item++) {
                double new = A[next][item] - A[row][item] * multiplier;
                A[next][item] = new;
            }
        }
    }
    return determinant;
}

void read_mat(double A[][SIZE], size_t m, size_t n) {
  for(size_t i=0; i<m; ++i) {
    for(size_t j=0; j<n; ++j)  scanf("%lf",&A[i][j]);
  }
}

void print_mat(const double A[][SIZE], size_t m, size_t n) {
  for(size_t i=0; i<m; ++i) {
    for(size_t j=0; j<n; ++j)  printf("%.4f ",A[i][j]);
    printf("\n");
  }
}

int main(void) {
    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE];

	int to_do;
	size_t m,n,p,q;

	char *keywords_ptab[] = {"do", "struct", "typedef", "for", "union",
    "int", "void", "long", "register", "auto", "return",
    "double", "else", "sizeof", "enum", "const", "continue",
    "default", "short", "extern", "static", "float",
    "goto", "switch", "if", "unsigned", "volatile",
    "while", "signed", "break", "char", "case"};
    char keywords_t2D[KW_NO][STRLEN_MAX];
    size_t indices[KW_NO];

	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
	switch (to_do) {
    case 1:
      n_str_copy(keywords_t2D, keywords_ptab, KW_NO);
      ptab_sort(keywords_ptab, KW_NO);
      t2D_sort(keywords_t2D, indices, KW_NO);
      if(TEST) printf("Wpisz liczbe lancuchow n ");
      scanf("%d", &n);   
      print_ptab(keywords_ptab, n);
      print_t2D_ind(keywords_t2D, indices, n);
      break;
    case 2:
      if(TEST) printf("Wpisz liczbe wierszy i liczbe kolumn mac.1: ");
      scanf("%d %d", &m, &n);
      if(TEST) printf("Wpisz mac.1 (wierszami): ");
      read_mat(A,m,n);
      if(TEST) printf("Wpisz liczbe wierszy i liczbe kolumn mac.2: ");
      scanf("%d %d", &p, &q);
      if(TEST) printf("Wpisz mac.2 (wierszami): ");
      read_mat(B,p,q);
      if(n == p) mat_product(A,B,C,m,n,q);
      print_mat(C,m,q);
      break;
    case 3:
      if(TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
      scanf("%d", &n);
      if(TEST) printf("Wpisz macierz A (wierszami): ");
      read_mat(A,n,n);
      printf("%.4f\n", gauss_simplified(A, n));
      print_mat(A,n,n);
      break;
    default:
      printf("NOTHING TO DO FOR %d\n", to_do);
  }
  return 0;
}

