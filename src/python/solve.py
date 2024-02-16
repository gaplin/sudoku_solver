import sudoku
import math
import re

input = open(0).read().splitlines()

puzzle = []
for line in input:
    cells = line.split(' ')
    row = [int(x) if re.match(r'^-?\d+$', x) != None else 0 for x in cells]
    puzzle.append(row)

NN = len(puzzle)
N = math.isqrt(len(puzzle))
if N * N != NN:
    print('Invalid input: grid size does not have integer square root')
    exit()

for idx, row in enumerate(puzzle):
    if len(row) != NN:
        print('Invalid input: size of row with idx {} does not match columns count'.format(idx))
        exit()

for row in puzzle:
    for value in row:
        if value < 0 or value > NN:
            print('Invalid input: incorrect value {}'.format(value))
            exit()

solution = sudoku.get_random_solution(puzzle, N)
if solution == None:
    print("Solution not found")
    exit()

for row in solution:    
    print(' '.join([str(x) for x in row]))