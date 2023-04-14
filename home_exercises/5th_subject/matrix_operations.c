#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 1    // 1 - dla testow,  0 - dla oceny automatycznej

void read_vec(double x[], size_t n) {
  for(size_t i=0; i<n; ++i)  scanf("%lf",x++);
}

void print_vec(const double x[], size_t n) {
  for(size_t i=0; i<n; ++i) printf("%.4f ",x[i]);
  printf("\n");
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

double absolute(double number) {
    if (number >= 0.0) return number;
    return -number;
}

int find_abs_max_element(double (**A)[SIZE], size_t n, int col_idx, int start_index, int (*indexes)[]) {
    // printf("the entire indexes ");
    // for(int i = 0; i < n; i++) printf("%d ", (*indexes)[i]);
    // printf("\n");
    int max_idx = start_index;
    int val_idx = (*indexes)[max_idx];
    double max_value = absolute((*A)[val_idx][col_idx]);
    for (int idx = start_index; idx < n; idx++) {
        int current_idx = (*indexes)[idx];
        double current_value = absolute((*A)[current_idx][col_idx]);
        if (current_value > max_value) {
            max_value = current_value;
            max_idx = idx;
        }
    }
    return max_idx;
}


// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.

double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps) {
    int indexes[n];
    double sol[n];
    int switches = 0;
    double determinant = 1;
    for (int i = 0; i < n; i++) {
        indexes[i] = i;
        sol[i] = b[i];    
    };

    for (int index = 0; index < n; index++) {
        int max_idx = find_abs_max_element(&A, n, index, index, &indexes);
        if (index != max_idx) {
            switches++;
            int placeholder = indexes[index];
            indexes[index] = indexes[max_idx];
            indexes[max_idx] = placeholder;
        }
        int next_row = indexes[index];
        double main = A[next_row][index];
        if (absolute(main) < eps) {
            return 0;   
        }
        determinant *= main;
        for (int row_idx = index + 1; row_idx < n; row_idx++) {
            int working_row_idx = indexes[row_idx];
            double multiplier = A[working_row_idx][index] / main;
            sol[working_row_idx] = sol[working_row_idx] - sol[next_row] * multiplier;
            for (int item = index; item < n; item++) {
                double new = A[working_row_idx][item] - A[next_row][item] * multiplier;
                A[working_row_idx][item] = new;
            }
        }
    }

    if (determinant != 0) {
        for (int index = n - 1; index >= 0; index--) {
            int working_row_idx = indexes[index];
            sol[working_row_idx] = sol[working_row_idx] / A[working_row_idx][index];
            for (int j = index - 1; j >= 0; j--) {
                int next_row = indexes[j];
                sol[next_row] -= sol[working_row_idx] * A[next_row][index];
            }
        }
    }

    for (int i = 0; i < n; i++) x[i] = sol[indexes[i]];

    return switches % 2 == 0 ? determinant : -determinant;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps) {
    int switches = 0;
    int indexes[n];
    double determinant = 1;
    double inversed[n][n];

    for (int i = 0; i < n; i++) {
        indexes[i] = i;
        for (int j = 0; j < n; j++) {
            inversed[i][j] = i == j ? 1 : 0;
        }
    }

    for (int index = 0; index < n; index++) {
        int max_idx = find_abs_max_element(&A, n, index, index, &indexes);
        if (index != max_idx) {
            switches++;
            int placeholder = indexes[index];
            indexes[index] = indexes[max_idx];
            indexes[max_idx] = placeholder;
        }
        int next_row = indexes[index];
        double main = A[next_row][index];
        if (absolute(main) < eps) {
            return 0;   
        }
        determinant *= main;
        for (int row_idx = index + 1; row_idx < n; row_idx++) {
            int working_row_idx = indexes[row_idx];
            double multiplier = A[working_row_idx][index] / main;
            for (int item = 0; item < n; item++) {
                double new = A[working_row_idx][item] - A[next_row][item] * multiplier;
                double reversed = inversed[working_row_idx][item] - inversed[next_row][item] * multiplier;
                A[working_row_idx][item] = new;
                inversed[working_row_idx][item] = reversed;
            }
        }
    }

    for (int index = 0; index < n; index++) {
        int diagonal = indexes[index];
        double main = A[diagonal][index];
        for (int item = 0; item < n; item++) {
            double new = A[diagonal][item] / main;
            double reversed = inversed[diagonal][item] / main;
            A[diagonal][item] = new;
            inversed[diagonal][item] = reversed;
        }
    }

    for (int index = n - 1; index >= 0; index--) {
        int main_row = indexes[index];
        for (int row_idx = 0; row_idx < index; row_idx++) {
            int working_row_idx = indexes[row_idx];
            double main = A[working_row_idx][index];
            for (int item = 0; item < n; item++) {
                double new = A[working_row_idx][item] - A[main_row][item] * main;
                double reversed = inversed[working_row_idx][item] - inversed[main_row][item] * main;
                A[working_row_idx][item] = new;
                inversed[working_row_idx][item] = reversed;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        int row = indexes[i];
        for (int j = 0; j < n; j++) {
            B[i][j] = inversed[row][j];
        }
    }

    return switches % 2 == 0 ? determinant : -determinant;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;
	int to_do;
	size_t m,n,p,q;
	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
	switch (to_do) {
    case 1:
      if(TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
      scanf("%d", &n);
      if(TEST) printf("Wpisz macierz A (wierszami): ");
      read_mat(A,n,n);
      if(TEST) printf("Wpisz wektor b: ");
      read_vec(b,n);
      det = gauss(A, b, x, n, eps);
      printf("%.4f\n",det);
      if(det) print_vec(x,n);
      break;
    case 2:
      if(TEST) printf("Wpisz rozmiar macierzy n = ");
      scanf("%d",&n);
      if(TEST) printf("Wpisz elementy macierzy (wierszami): ");
      read_mat(A,n,n);
      det = matrix_inv(A,B,n,eps);
      printf("%.4f\n",det);
      if(det) print_mat(B,n,n);
      break;
    default:
      printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
