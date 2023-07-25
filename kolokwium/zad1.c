#include <stdlib.h>
#include <stdio.h>

typedef struct _matrix {
    int rows;
    int cols;
    double **data;
} Matrix;

int create_matrix(Matrix *pmatrix, int rows, int cols) {
    if (rows < 1 || cols < 1) return 0;
    pmatrix->rows = rows;
    pmatrix->cols = cols;
    pmatrix->data = malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        double *row = calloc(cols, sizeof(double));
        pmatrix->data[i] = row;
    }
    return 1;
}

void free_matrix(Matrix *pmatrix) {
    if (pmatrix->data == 0) return;
    for (int row = 0; row < pmatrix->rows; row++) {
        free(pmatrix->data[row]);
    }
    free(pmatrix->data);
}

void copy_matrix(Matrix *ptrg, const Matrix *psrc) {
    create_matrix(ptrg, psrc->rows, psrc->cols);
    for (int row = 0; row < psrc->rows; row++) {
        for (int col = 0; col < psrc->cols; col++) {
            ptrg->data[row][col] = psrc->data[row][col];
        }
    }
}

void print_matrix(const Matrix *pm) {
    printf("{");
    for (int row = 0; row < pm->rows; row++) {
        printf("{");
        for (int col = 0; col < pm->cols - 1; col++) printf("%f,", pm->data[row][col]);
        printf("%f", pm->data[pm->rows - 1][pm->cols - 1]); // ostatni element bez przecinka
        printf("}");
    }
}

int swap_rows(Matrix *m, int r1, int r2) {
    if (r1 >= m->rows || r2 >= m->rows) return 0;
    double *placeholder = m->data[r1];
    m->data[r1] = m->data[r2];
    m->data[r2] = placeholder;
    return 1;
}

size_t glob_columns;

int compar(const void *a, const void *b) {
    double sum_a = 0;
    double sum_b = 0;
    double *pa = (double *)a;
    double *pb = (double *)b;
    for (int idx = 0; idx < glob_columns; idx++) {
        sum_a += pa[idx];
        sum_b += pb[idx];
    }
    // jesli porownujemy srednie arytmetyczne:
    // sum_b / glob_columns > sum_a / glob_columns
    // mozemy pominac dzielenie przez glob_columns, poniewaz jest to liczba zawsze wieksza od 0:
    // sum_b / glob_columns > sum_a / glob_columns      / * glob_columns
    // sum_b > sum_a
    if (sum_a == sum_b) return 0;
    return sum_b - sum_a > 0 ? 1 : -1;
}

int main() {
    Matrix *sample_matrix;
    create_matrix(sample_matrix, 10, 10);
    qsort(sample_matrix->data, sample_matrix->rows, sizeof(double*), compar);
    return 0;
}