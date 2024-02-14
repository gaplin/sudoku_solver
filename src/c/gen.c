#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"

int main(int argc, char* argv[]) {
    reseed_rand(time(NULL));
    int filled_entries = 30;
    if(argc > 1) {
        filled_entries = atoi(argv[1]);
        if(filled_entries < 20 || filled_entries > SUDOKU_ALL) {
            printf("Filled entries value must be in range [20, %d]\n", SUDOKU_ALL);
            return -1;
        }
    }

    gen_result result = generate_random_puzzle(filled_entries);
    printf("puzzle\n");
    for(int i = 0; i < SUDOKU_NN; ++i) {
        for(int j = 0; j < SUDOKU_NN; ++j) {
            printf("%d ", result.puzzle[i][j]);
        }
        printf("\n");
    }
    printf("solution\n");
    for(int i = 0; i < SUDOKU_NN; ++i) {
        for(int j = 0; j < SUDOKU_NN; ++j) {
            printf("%d ", result.solution[i][j]);
        }
        printf("\n");
    }
    free_gen_result(result);
}