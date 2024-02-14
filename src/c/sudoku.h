#ifndef SUDOKU_H
#define SUDOKU_H

// Size of single square.
// For classis 9x9 sudoku square size is 3
#define SUDOKU_N 3
// Count of available numbers
#define SUDOKU_NN 9
// Count of all numbers in the grid
#define SUDOKU_ALL 81

// Result of random puzzle generation.
// Contains both puzzle and solution
typedef struct {
    int (*puzzle)[SUDOKU_NN];
    int (*solution)[SUDOKU_NN];
} gen_result;

// Frees puzzle and solution
// Freeing both iff pointers are different
void free_gen_result(gen_result value);

// Reseeds RNG.
// Suggested seed: time(NULL)
void reseed_rand(unsigned int seed);

// Returns random solution or NULL if not found. Using rand from stdlib.
// Use reseed_rand to use different seed.
int* get_random_solution(int grid[][SUDOKU_NN]);

// Returns count of solutions up to limit.
int get_solutions_count(int grid[][SUDOKU_NN], int limit);

// Generates random 9x9 sudoku puzzle with given filled entries amount
// It's getting slow for numbers lower than 22
// Use reseed_rand to use different seed.
gen_result generate_random_puzzle(int filled_entries);

#endif // SUDOKU_H