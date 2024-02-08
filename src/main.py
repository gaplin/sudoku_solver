import sudoku
import sys

n = 3
nn = n ** 2
filled_entries = 30
if len(sys.argv) > 1:
    filled_entries = int(sys.argv[1])
    
assert 25 <= filled_entries <= nn ** 2

(puzzle, solution) = sudoku.generate_grid(n, filled_entries)
puzzle_file = open('puzzle.txt', 'w')
for row in puzzle:
    puzzle_file.write(' '.join([str(x) if x != 0 else '_' for x in row]) + '\n')
puzzle_file.close()

solution_file = open('solution.txt', 'w')
for row in solution:
    solution_file.write(' '.join([str(x) if x != 0 else '_' for x in row]) + '\n')
solution_file.close()