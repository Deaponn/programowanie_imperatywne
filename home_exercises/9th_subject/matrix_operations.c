#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    return *(A + row * cols + col);
}

void set(int cols, int row, int col, int *A, int value) {
    *(A + row * cols + col) = value;
}

int dot_product(const int *A, const int *B, int row, int col, int colsA, int colsB) {
    int product = 0;
    for(int i = 0; i < colsA; i++){
        product += get(colsA, row, i, A) * get(colsB, i, col, B);
    }
    return product;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    for (int target_row = 0; target_row < rowsA; target_row++) {
        for (int target_column = 0; target_column < colsB; target_column++) {
            set(colsB, target_row, target_column, AB, dot_product(A, B, target_row, target_column, colsA, colsB));
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    int item;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            scanf("%d", &item);
            set(cols, row, col, t, item);
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    int item;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            item = get(cols, row, col, t);
            printf("%d ", item);
        }
        printf("\n");
    }
}

int read_char_lines(char *array[]) {
    char *string;
    int length;
    int line = 0;
    while (1) {
        string = malloc(BUF_SIZE);
        fgets(string, BUF_SIZE, stdin);
        length = strlen(string);
        if (length == 1 || feof(stdin) || ferror(stdin)) break;
        array[line] = string;
        *(string + length - 1) = '\0'; // remove unnecessary \n from input
        line++;
    }
}

void write_char_line(char *array[], int n) {
    printf("%s", array[n]);
}

void delete_lines(char *array[]) {
}

int read_int_lines_cont(int *ptr_array[]) {
    char input[TAB_SIZE];
    int length;
    int *numbers = ptr_array[0];
    int line = 0;
    while (1) {
        fgets(input, TAB_SIZE, stdin);
        length = strlen(input);
        if (length == 1 || feof(stdin) || ferror(stdin)) break;
        char *start = input;
        ptr_array[line] = numbers;
        while (*start != '\n') {
            long number = strtol(start, &start, 10);
            *numbers = number;
            numbers++;
        }
        line++;
    }
    ptr_array[line] = numbers;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int *start = ptr_array[n];
    int *end = ptr_array[n + 1];
    while (start != end) {
        printf("%d ", *start);
        start++;
    }
}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    char input[TAB_SIZE];
    int length;
    int line = 0;
    while (1) {
        fgets(input, TAB_SIZE, stdin);
        length = strlen(input);
        if (length == 1 || feof(stdin) || ferror(stdin)) break;
        long sum = 0;
        int *numbers = malloc(TAB_SIZE * sizeof(int));
        line_type data;
        data.len = 0;
        data.values = numbers;
        char *start = input;
        while (*start != '\n') {
            long number = strtol(start, &start, 10);
            *numbers = number;
            numbers++;
            sum += number;
            data.len = data.len + 1;
            data.average = (float)sum / data.len;
        }
        lines_array[line] = data;
        line++;
    }
    return line;
}

void write_int_line(line_type lines_array[], int n) {
    line_type selected = lines_array[n];
    int *numbers = selected.values;
    for (int idx = 0; idx < selected.len; idx++) {
        printf("%d ", *numbers);
        numbers++;
    }
    printf("\n%.2f\n", selected.average);
}

void delete_int_lines(line_type array[], int line_count) {
}

int cmp (const void *a, const void *b) {
    line_type first = *(line_type *)a;
    line_type second = *(line_type *)b;
    return first.average - second.average;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array, line_count, sizeof(line_type), &cmp);
}

typedef struct {
	int row, column, value;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    int row, column, value;
    for (int index = 0; index < n_triplets; index++) {
        scanf("%d %d %d\n", &row, &column, &value);
        triplet current ;
        current.row = row;
        current.column = column;
        current.value = value;
        *(triplet_array + index) = current;
    }
    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet first = *(triplet *)t1;
    triplet second = *(triplet *)t2;
    return first.row == second.row ? first.column - second.column : first.row - second.row;
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array, n_triplets, sizeof(triplet), &cmp_triplets);
    for (int index = 0; index < n_triplets; index++) {
        triplet current = *(triplet_array + index);
        int row_items = 0;
        *(V + index) = current.value;
        *(C + index) = current.column;
        *(R + current.row + 1) = *(R + current.row + 1) + 1;
    }
    for (int index = 0; index <= rows; index++) {
        *(R + index + 1) = *(R + index) + *(R + index + 1);
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for (int i = 0; i <= rows - 1; i++) {
        *(y + i) = 0;
        for (int j = *(R + i); j <= *(R + i + 1) - 1; j++) {
            *(y + i) = *(y + i) + (*(V + j)) * (*(x + *(C + j)));
        }
    }
}

void read_vector(int *v, int n) {
    char *input = malloc(TAB_SIZE);
    fgets(input, TAB_SIZE, stdin);
    while (*input != '\n') {
        long number = strtol(input, &input, 10);
        *v = number;
        v++;
    }
}

void write_vector(int *v, int n) {
    for (int idx = 0; idx < n; idx++) {
        printf("%d ", *(v + idx));
    }
    printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE];
    int R[TAB_SIZE] = {0};
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
