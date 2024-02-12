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

void free_gen_result(gen_result value) {
    free(value.puzzle);
    if(value.puzzle != value.solution) {
        free(value.solution);
    }
}

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

int count_solutions(int rows[], int cols[], int squares[], int square_idxes[][NN], pair zeros[], int zeros_count, int idx, int limit) {
    if(limit <= 0) {
        return 0;
    }
    pair zero = zeros[idx];
    int result = 0;
    for(int i = 1; i <= NN; ++i) {
        int num_mask = (1 << i);
        int square_idx = square_idxes[zero.first][zero.second];
        if((rows[zero.first] & num_mask) == 0 && (cols[zero.second] & num_mask) == 0 && (squares[square_idx] & num_mask) == 0) {
            int solutions_found = 0;
            if(idx + 1 == zeros_count) {
                solutions_found = 1;
            }
            else {
                rows[zero.first] |= num_mask;
                cols[zero.second] |= num_mask;
                squares[square_idx] |= num_mask;
                solutions_found = count_solutions(rows, cols, squares, square_idxes, zeros, zeros_count, idx + 1, limit - result);
                rows[zero.first] ^= num_mask;
                cols[zero.second] ^= num_mask;
                squares[square_idx] ^= num_mask;
            }
            result += solutions_found;
            if(result >= limit) {
                return result;
            }
        }
    }
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
    int rows[NN];
    int cols[NN];
    int squares[NN];
    int square_idxes[NN][NN];
    memset(rows, 0, NN * sizeof(int));
    memset(cols, 0, NN * sizeof(int));
    memset(squares, 0, NN * sizeof(int));
    for(int i = 0; i < NN; ++i) {
        for(int j = 0; j < NN; ++j) {
            int mask = (1 << grid[i][j]);
            rows[i] |= mask;
            cols[j] |= mask;
            int square_idx = i / N * N + j / N;
            square_idxes[i][j] = square_idx;
            squares[square_idx] |= mask;
        }
    }
    int solutions = count_solutions(rows, cols, squares, square_idxes, zeros, zeros_count, 0, limit);
    return solutions;
}

gen_result generate_grid(int empty_entries) {
    int empty_grid[NN][NN];
    pair idxes[ALL];
    pair idxes_copy[ALL];
    for(int i = 0; i < NN; ++i) {
        for(int j = 0; j < NN; ++j) {
            empty_grid[i][j] = 0;
            idxes[i * NN + j].first = i;
            idxes[i * NN + j].second = j;
            idxes_copy[i * NN + j].first = i;
            idxes_copy[i * NN + j].second = j;
        }
    }

    int (*solution)[NN] = (int(*)[NN]) malloc(ALL * sizeof(int));
    int (*puzzle)[NN] = (int(*)[NN]) malloc(ALL * sizeof(int));
    memset(solution, 0, ALL * sizeof(int));
    fill_grid_randomly(solution, idxes_copy, ALL, 0);
    memcpy(puzzle, solution, ALL * sizeof(int));
    if(empty_entries > 0) {
        while(1) {
            pair_random_shuffle(idxes, ALL);
            int left_to_take = empty_entries;
            int idx = ALL - 1;
            while(idx + 1 >= left_to_take && left_to_take > 0) {
                int prev_value = puzzle[idxes[idx].first][idxes[idx].second];
                puzzle[idxes[idx].first][idxes[idx].second] = 0;
                int solutions = get_solutions_count(puzzle, 2);
                if(solutions == 1) {
                    --left_to_take;
                } else {
                    puzzle[idxes[idx].first][idxes[idx].second] = prev_value;
                }
                --idx;
            }
            if(left_to_take > 0) {
                memset(solution, 0, ALL * sizeof(int));
                fill_grid_randomly(solution, idxes_copy, ALL, 0);
                memcpy(puzzle, solution, ALL * sizeof(int));
            } else {
                break;
            }
        }
    }

    gen_result result;
    result.puzzle = puzzle;
    result.solution = solution;
    return result;
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
    free_gen_result(result);
}