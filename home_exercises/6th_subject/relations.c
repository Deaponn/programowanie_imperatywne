#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair*, int);

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair*, int);

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair*, int);

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair*, int);

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair*, int);

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair*, int);

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair*, int);

// A total order relation is a partial order relation that is connected
int is_total_order(pair*, int);

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair*, int);

int find_max_elements(pair*, int, int*);
int find_min_elements(pair*, int, int*);
int get_domain(pair*, int, int*);

// Case 3:

int composition (pair*, int, pair*, int, pair*);

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
}

int insert_int (int *tab, int n, int new_element) {
}

int bubble_sort(int n, int *tab) {
    int counter = 0;
    int is_sorted = 0;
    for (int i = 0; i < n; i++) {
        is_sorted = 1;
        for (int j = 0; j < (n - i - 1); j++) {
            if (*(tab + j) > *(tab + j + 1)) {
                is_sorted = 0;
                int placeholder = *(tab + j);
                *(tab + j) = *(tab + j + 1);
                *(tab + j + 1) = placeholder;
            }
        }
        if (is_sorted == 1) {
            return counter;
        }
        counter++;
    }
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
    if (n == 0) {
        *tab = new_pair;
        return 1;
    }
    for (int idx = 0; idx < n; idx++) {
        // if ((*(tab + idx)).first > new_pair.first) break;
        if ((*(tab + idx)).first == new_pair.first && (*(tab + idx)).second == new_pair.second) return n;
    }
    *(tab + n) = new_pair;
    // int last_smaller = 0;
    // while (last_smaller < n && (*(tab + last_smaller)).first < new_pair.first) last_smaller++;
    // for (int idx = n; idx > last_smaller; idx--) {
    //     *(tab + idx) = *(tab + idx - 1);
    // }
    // // while (last_smaller > 0 && (*(tab + last_smaller)).first == new_pair.first && (*(tab + last_smaller)).second > new_pair.second) {
    // //     *(tab + last_smaller) = *(tab + last_smaller - 1);
    // // }
    // *(tab + last_smaller - 1) = new_pair;
    return n + 1;
}

int find(pair *tab, int n, int first, int second) {
    for (int idx = 0; idx < n; idx++) {
        if ((*(tab + idx)).first == first && (*(tab + idx)).second == second) return idx;
    }
    return -1;
}

int find_related(pair *tab, int n, int item, int start) {
    for (int idx = start; idx < n; idx++) {
        int first = (*(tab + idx)).first;
        int second = (*(tab + idx)).second;
        if (first == item) return idx;
    }
    return -1;
}

int is_reflexive(pair *tab, int n) {
    for (int idx = 0; idx < n; idx++) {
        int first = (*(tab + idx)).first;
        int result = find(tab, n, first, first);
        if (result == -1) return 0;
    }
    return 1;
};

int is_irreflexive(pair *tab, int n) {
    for (int idx = 0; idx < n; idx++) {
        if ((*(tab + idx)).first == (*(tab + idx)).second) return 0;
    }
    return 1;
};

int is_symmetric(pair *tab, int n) {
    for (int idx = 0; idx < n; idx++) {
        int first = (*(tab + idx)).first;
        int second = (*(tab + idx)).second;
        if (first != second) {
            int result = find(tab, n, second, first);
            if (result == -1) return 0;
        }
    }
    return 1;
};

int is_antisymmetric(pair *tab, int n) {
    for (int idx = 0; idx < n; idx++) {
        int first = (*(tab + idx)).first;
        int second = (*(tab + idx)).second;
        if (first != second) {
            int result = find(tab, n, second, first);
            if (result != -1) return 0;
        }
    }
    return 1;
};

int is_asymmetric(pair *tab, int n) {
    for (int idx = 0; idx < n; idx++) {
        int first = (*(tab + idx)).first;
        int second = (*(tab + idx)).second;
        int result = find(tab, n, second, first);
        if (result != -1) return 0;
    }
    return 1;
};

int is_transitive(pair *tab, int n) {
    for (int i = 0; i < n; i++) {
        int x_first = (*(tab + i)).first;
        int x_second = (*(tab + i)).second;
        for (int j = 0; j < n; j++) {
            int y_first = (*(tab + j)).first;
            int y_second = (*(tab + j)).second;
            if (x_second == y_first) {
                int result = find(tab, n, x_first, y_second);
                if (result == -1) return 0;
            }
        }
    }
    return 1;
};

int is_partial_order(pair *tab, int n) {
    int reflexive = is_reflexive(tab, n);
    int antisymmetric = is_antisymmetric(tab, n);
    int transitive = is_transitive(tab, n);
    return (reflexive == 1 && antisymmetric == 1 && transitive == 1) ? 1 : 0;
};

int is_total_order(pair *tab, int n) {
    int partial_order = is_partial_order(tab, n);
    int connected = is_connected(tab, n);
    return (partial_order == 1 && connected == 1) ? 1 : 0;
};

int is_connected(pair *tab, int n) {
    for (int idx = 0; idx < n; idx++) {
        int first = (*(tab + idx)).first;
        int second = (*(tab + idx)).second;
        int connected_first = 0;
        int connected_second = 0;
        for (int jdx = 0; jdx < n; jdx++) {
            int current_first = (*(tab + jdx)).first;
            int current_second = (*(tab + jdx)).second;
            if ((first == current_first || first == current_second) && current_first != current_second) connected_first = 1;
            if ((second == current_first || second == current_second) && current_first != current_second) connected_second = 1;
        }
        if (connected_first == 0 || connected_second == 0) return 0;
    }
    return 1;
};

int find_max_elements(pair *tab, int n, int *elem) {
    int last_max_element = 0;
    for (int idx = 0; idx < n; idx++) {
        int item = (*(tab + idx)).second;
        int related = 0;
        for (int current_idx = 0; current_idx < n; current_idx++) {
            int current_first = (*(tab + current_idx)).first;
            int current_second = (*(tab + current_idx)).second;
            if (current_first == item && current_first != current_second) {
                related = 1;
                break;    
            };
        }
        if(related == 0) {
            int present = 0;
            for (int current_idx = 0; current_idx < last_max_element; current_idx++) {
                if ((*(elem + current_idx)) == item) {
                    present = 1;
                    break;    
                }
            }
            if (present == 0) {
                *(elem + last_max_element) = item;
                last_max_element++;
            }
        }
    }
    bubble_sort(last_max_element, elem);
    return last_max_element;
};

int find_min_elements(pair *tab, int n, int *elem) {int last_max_element = 0;
    for (int idx = 0; idx < n; idx++) {
        int item = (*(tab + idx)).first;
        int related = 0;
        for (int current_idx = 0; current_idx < n; current_idx++) {
            int current_first = (*(tab + current_idx)).first;
            int current_second = (*(tab + current_idx)).second;
            if (current_second == item && current_first != current_second) {
                related = 1;
                break;    
            };
        }
        if(related == 0) {
            int present = 0;
            for (int current_idx = 0; current_idx < last_max_element; current_idx++) {
                if ((*(elem + current_idx)) == item) {
                    present = 1;
                    break;    
                }
            }
            if (present == 0) {
                *(elem + last_max_element) = item;
                last_max_element++;
            }
        }
    }
    bubble_sort(last_max_element, elem);
    return last_max_element;
};

int get_domain(pair *tab, int n, int *elem) {
    int last_unique_element = 0;
    for (int idx = 0; idx < n; idx++) {
        int first = (*(tab + idx)).first;
        int second = (*(tab + idx)).second;
        int first_present = 0;
        int second_present = 0;
        int current_idx = 0;
        while (current_idx < last_unique_element && (first_present == 0 || second_present == 0)) {
            if ((*(elem + current_idx)) == first) first_present = 1;
            if ((*(elem + current_idx)) == second) second_present = 1;
            current_idx++;
        }
        if(first_present == 0) {
            *(elem + last_unique_element) = first;
            last_unique_element++;
        }
        if(second_present == 0 && first != second) {
            *(elem + last_unique_element) = second;
            last_unique_element++;
        }
    }
    bubble_sort(last_unique_element, elem);
    return last_unique_element;
};

int composition(pair *tab1, int n1, pair *tab2, int n2, pair *tab3) {
    int n3_len = 0;
    for (int idx = 0; idx < n1; idx++) {
        int first = (*(tab1 + idx)).first;
        int second = (*(tab1 + idx)).second;
        int paired_idx = find_related(tab2, n2, second, 0);
        while (paired_idx != -1) {
            int third = (*(tab2 + paired_idx)).second;
            pair new_pair;
            new_pair.first = first;
            new_pair.second = third;
            n3_len = add_relation(tab3, n3_len, new_pair);
            paired_idx = find_related(tab2, n2, second, paired_idx + 1);
        }
    }
    return n3_len;
};

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int size;
    int length = 0;
    scanf("%d", &size);
    for (int i = 0; i < size; i++) {
        int first, second;
        pair my_pair;
        scanf("%d %d", &first, &second);
        my_pair.first = first;
        my_pair.second = second;
        length = add_relation(relation, length, my_pair);
    }
    return length;
}

void print_int_array(int *array, int n) {
    printf("%d\n", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", *(array + i));
    }
    printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
