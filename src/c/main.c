#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 3
#define NN 9
#define ALL 81

typedef struct {
    int first;
    int second;
} pair;

typedef struct {
    int (*puzzle)[NN];
    int (*solution)[NN];
} gen_result;

void int_random_shuffle(int arr[], int n) {
    for(int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

void pair_random_shuffle(pair arr[], int n) {
    for(int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        pair tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

char valid_row(int grid[][NN], int i) {
    unsigned int taken_nums = 0;
    for(int j = 0; j < NN; ++j) {
        if(grid[i][j] != 0) {
            unsigned int mask = (1 << grid[i][j]);
            if((taken_nums & mask) != 0) {
                return 0;
            }
            taken_nums |= mask;
        }
    }
    return 1;
}

char valid_col(int grid[][NN], int j) {
    unsigned int taken_nums = 0;
    for(int i = 0; i < NN; ++i) {
        if(grid[i][j] != 0) {
            unsigned int mask = (1 << grid[i][j]);
            if((taken_nums & mask) != 0) {
                return 0;
            }
            taken_nums |= mask;
        }
    }
    return 1;
}

char valid_square(int grid[][NN], int i, int j) {
    i = i / N * N;
    j = j / N * N;
    unsigned int taken_nums = 0;
    for(int k = 0; k < N; ++k) {
        for(int l = 0; l < N; ++l) {
            if(grid[i + k][j + l] != 0) {
                unsigned int mask = (1 << grid[i + k][j + l]);
                if((taken_nums & mask) != 0) {
                    return 0;
                }
                taken_nums |= mask;
            }
        }
    }
    return 1;
}

char fill_grid_randomly(int grid[][NN], pair zeros[], int zeros_count, int idx) {
    pair zero = zeros[idx];
    int nums[NN];
    for(int i = 0; i < NN; ++i) {
        nums[i] = i + 1;
    }
    int_random_shuffle(nums, NN);
    for(int i = 0; i < NN; ++i) {
        grid[zero.first][zero.second] = nums[i];
        if(valid_col(grid, zero.second) && valid_row(grid, zero.first) && valid_square(grid, zero.first, zero.second)) {
            if(idx + 1 == zeros_count) {
                return 1;
            }
            char solution_found = fill_grid_randomly(grid, zeros, zeros_count, idx + 1);
            if(solution_found) {
                return 1;
            }
        }
    }
    grid[zero.first][zero.second] = 0; 
    return 0;
}

int* get_random_solution(int grid[][NN]) {
    int (*grid_cpy)[NN] = (int(*)[NN]) malloc(ALL * sizeof(int));
    memcpy(grid_cpy, grid, sizeof(int) * ALL);
    pair zeros[ALL];
    int zeros_count = 0;
    for(int i = 0; i < NN; ++i) {
        for(int j = 0; j < NN; ++j) {
            if(grid[i][j] == 0) {
                zeros[zeros_count].first = i;
                zeros[zeros_count].second = j;
                ++zeros_count;
            }
        }
    }
    char solution_found = fill_grid_randomly(grid_cpy, zeros, zeros_count, 0);
    if(solution_found) {
        return (int*)grid_cpy;
    }
    free(grid_cpy);
    return NULL;
}

int count_solutions(int grid[][NN], pair zeros[], int zeros_count, int idx, int limit) {
    if(limit <= 0) {
        return 0;
    }
    pair zero = zeros[idx];
    int result = 0;
    for(int i = 1; i <= NN; ++i) {
        grid[zero.first][zero.second] = i;
        if(valid_col(grid, zero.second) && valid_row(grid, zero.first) && valid_square(grid, zero.first, zero.second)) {
            int solutions_found = 0;
            if(idx + 1 == zeros_count) {
                solutions_found = 1;
            }
            else {
                solutions_found = count_solutions(grid, zeros, zeros_count, idx + 1, limit - result);
            }
            result += solutions_found;
            if(result >= limit) {
                break;
            }
        }
    }
    grid[zero.first][zero.second] = 0;
    return result;
}

int get_solutions_count(int grid[][NN], int limit) {
    pair zeros[ALL];
    int zeros_count = 0;
    for(int i = 0; i < NN; ++i) {
        for(int j = 0; j < NN; ++j) {
            if(grid[i][j] == 0) {
                zeros[zeros_count].first = i;
                zeros[zeros_count].second = j;
                ++zeros_count;
            }
        }
    }
    int solutions = count_solutions(grid, zeros, zeros_count, 0, limit);
    return solutions;
}

gen_result generate_grid(int empty_entries) {
    int empty_grid[NN][NN];
    pair idxes[ALL];
    for(int i = 0; i < NN; ++i) {
        for(int j = 0; j < NN; ++j) {
            empty_grid[i][j] = 0;
            idxes[i * NN + j].first = i;
            idxes[i * NN + j].second = j;
        }
    }

    while(1) {
        int (*random_solution)[NN] = (int(*)[NN]) get_random_solution(empty_grid);
        if(empty_entries <= 0) {
            gen_result result;
            result.puzzle = random_solution;
            result.solution = random_solution;
            return result;
        }
        pair_random_shuffle(idxes, ALL);
        int (*grid_cpy)[NN] = (int(*)[NN]) malloc(ALL * sizeof(int));
        memcpy(grid_cpy, random_solution, ALL * sizeof(int));
        int left_to_take = empty_entries;
        int idx = ALL - 1;
        while(idx + 1 >= left_to_take && left_to_take > 0) {
            int prev_value = random_solution[idxes[idx].first][idxes[idx].second];
            random_solution[idxes[idx].first][idxes[idx].second] = 0;
            int solutions = get_solutions_count(random_solution, 2);
            if(solutions == 1) {
                --left_to_take;
            } else {
                random_solution[idxes[idx].first][idxes[idx].second] = prev_value;
            }
            --idx;
        }
        if(left_to_take == 0) {
            gen_result result;
            result.puzzle = random_solution;
            result.solution = grid_cpy;
            return result;
        }
        free(grid_cpy);
        free(random_solution);
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int filled_entries = 30;
    if(argc > 1) {
        filled_entries = atoi(argv[1]);
        if(filled_entries < 20 || filled_entries > ALL) {
            printf("Filled entries value must be in range [20, %d]\n", ALL);
            return -1;
        }
    }

    gen_result result = generate_grid(ALL - filled_entries);
    printf("puzzle\n");
    for(int i = 0; i < NN; ++i) {
        for(int j = 0; j < NN; ++j) {
            printf("%d ", result.puzzle[i][j]);
        }
        printf("\n");
    }
    printf("solution\n");
    for(int i = 0; i < NN; ++i) {
        for(int j = 0; j < NN; ++j) {
            printf("%d ", result.solution[i][j]);
        }
        printf("\n");
    }
    free(result.puzzle);
    free(result.solution);
}