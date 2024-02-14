#include <stdlib.h>
#include <string.h>
#include "sudoku.h"

typedef struct {
    int first;
    int second;
} pair;

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

char valid_row(int grid[][SUDOKU_NN], int i) {
    unsigned int taken_nums = 0;
    for(int j = 0; j < SUDOKU_NN; ++j) {
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

char valid_col(int grid[][SUDOKU_NN], int j) {
    unsigned int taken_nums = 0;
    for(int i = 0; i < SUDOKU_NN; ++i) {
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

char valid_square(int grid[][SUDOKU_NN], int i, int j) {
    i = i / SUDOKU_N * SUDOKU_N;
    j = j / SUDOKU_N * SUDOKU_N;
    unsigned int taken_nums = 0;
    for(int k = 0; k < SUDOKU_N; ++k) {
        for(int l = 0; l < SUDOKU_N; ++l) {
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

char fill_grid_randomly(int grid[][SUDOKU_NN], pair zeros[], int zeros_count, int idx) {
    pair zero = zeros[idx];
    int nums[SUDOKU_NN];
    for(int i = 0; i < SUDOKU_NN; ++i) {
        nums[i] = i + 1;
    }
    int_random_shuffle(nums, SUDOKU_NN);
    for(int i = 0; i < SUDOKU_NN; ++i) {
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

int* get_random_solution(int grid[][SUDOKU_NN]) {
    int (*grid_cpy)[SUDOKU_NN] = (int(*)[SUDOKU_NN]) malloc(SUDOKU_ALL * sizeof(int));
    memcpy(grid_cpy, grid, sizeof(int) * SUDOKU_ALL);
    pair zeros[SUDOKU_ALL];
    int zeros_count = 0;
    for(int i = 0; i < SUDOKU_NN; ++i) {
        for(int j = 0; j < SUDOKU_NN; ++j) {
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

int count_solutions(int rows[], int cols[], int squares[], int square_idxes[][SUDOKU_NN], pair zeros[], int zeros_count, int idx, int limit) {
    if(limit <= 0) {
        return 0;
    }
    pair zero = zeros[idx];
    int result = 0;
    for(int i = 1; i <= SUDOKU_NN; ++i) {
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

int get_solutions_count(int grid[][SUDOKU_NN], int limit) {
    pair zeros[SUDOKU_ALL];
    int zeros_count = 0;
    for(int i = 0; i < SUDOKU_NN; ++i) {
        for(int j = 0; j < SUDOKU_NN; ++j) {
            if(grid[i][j] == 0) {
                zeros[zeros_count].first = i;
                zeros[zeros_count].second = j;
                ++zeros_count;
            }
        }
    }
    int rows[SUDOKU_NN];
    int cols[SUDOKU_NN];
    int squares[SUDOKU_NN];
    int square_idxes[SUDOKU_NN][SUDOKU_NN];
    memset(rows, 0, SUDOKU_NN * sizeof(int));
    memset(cols, 0, SUDOKU_NN * sizeof(int));
    memset(squares, 0, SUDOKU_NN * sizeof(int));
    for(int i = 0; i < SUDOKU_NN; ++i) {
        for(int j = 0; j < SUDOKU_NN; ++j) {
            int mask = (1 << grid[i][j]);
            rows[i] |= mask;
            cols[j] |= mask;
            int square_idx = i / SUDOKU_N * SUDOKU_N + j / SUDOKU_N;
            square_idxes[i][j] = square_idx;
            squares[square_idx] |= mask;
        }
    }
    int solutions = count_solutions(rows, cols, squares, square_idxes, zeros, zeros_count, 0, limit);
    return solutions;
}

gen_result generate_random_puzzle(int filled_entries) {
    int empty_grid[SUDOKU_NN][SUDOKU_NN];
    pair idxes[SUDOKU_ALL];
    pair idxes_copy[SUDOKU_ALL];
    for(int i = 0; i < SUDOKU_NN; ++i) {
        for(int j = 0; j < SUDOKU_NN; ++j) {
            empty_grid[i][j] = 0;
            idxes[i * SUDOKU_NN + j].first = i;
            idxes[i * SUDOKU_NN + j].second = j;
            idxes_copy[i * SUDOKU_NN + j].first = i;
            idxes_copy[i * SUDOKU_NN + j].second = j;
        }
    }

    int (*solution)[SUDOKU_NN] = (int(*)[SUDOKU_NN]) malloc(SUDOKU_ALL * sizeof(int));
    int (*puzzle)[SUDOKU_NN] = (int(*)[SUDOKU_NN]) malloc(SUDOKU_ALL * sizeof(int));
    memset(solution, 0, SUDOKU_ALL * sizeof(int));
    fill_grid_randomly(solution, idxes_copy, SUDOKU_ALL, 0);
    memcpy(puzzle, solution, SUDOKU_ALL * sizeof(int));

    int empty_entries = SUDOKU_ALL - filled_entries;
    if(empty_entries > 0) {
        while(1) {
            pair_random_shuffle(idxes, SUDOKU_ALL);
            int left_to_take = empty_entries;
            int idx = SUDOKU_ALL - 1;
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
                memset(solution, 0, SUDOKU_ALL * sizeof(int));
                fill_grid_randomly(solution, idxes_copy, SUDOKU_ALL, 0);
                memcpy(puzzle, solution, SUDOKU_ALL * sizeof(int));
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

void reseed_rand(unsigned int seed) {
    srand(seed);
}