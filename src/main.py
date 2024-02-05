from copy import deepcopy
from heapq import heappop, heappush
from math import isqrt
from random import random

def to_key(grid: list) -> tuple:
    return tuple([tuple(x) for x in grid])

def valid_square(grid: list, top_left: tuple, n: int) -> bool:
    i, ii = top_left
    nums = set()
    for y in range(i, i + n):
        for x in range(ii, ii + n):
            if grid[y][x] != 0 and grid[y][x] in nums:
                return False
            nums.add(grid[y][x])
            
    return True

def valid_row(grid: list, i: int, nn: int) -> bool:
    nums = set()
    for ii in range(nn):
        if grid[i][ii] != 0 and grid[i][ii] in nums:
            return False
        nums.add(grid[i][ii])

    return True

def valid_col(grid: list, ii: int, nn: int) -> bool:
    nums = set()
    for i in range(nn):
        if grid[i][ii] != 0 and grid[i][ii] in nums:
            return False
        nums.add(grid[i][ii])

    return True

def valid_state(grid: list, n: int) -> bool:
    nn = n ** 2
    for i in range(0, nn, n):
        for ii in range(0, nn, n):
            if valid_square(grid, (i, ii), n) == False:
                return False
            
    for i in range(nn):
        if valid_row(grid, i, nn) == False or valid_col(grid, i, nn) == False:
            return False
        
    return True


def get_solution(initial_grid: list, n: int) -> list:
    missing_entries = []
    nn = n ** 2
    for i in range(nn):
        for ii in range(nn):
            if initial_grid[i][ii] == 0:
                missing_entries.append((i, ii))

    N = len(missing_entries) - 1
    visited = {to_key(initial_grid)}
    Q = [(N, random(), initial_grid)]

    while Q != []:
        current_idx, _, grid = heappop(Q)
        if current_idx == -1:
            return grid
        i, ii = missing_entries[current_idx]

        for k in range(1, nn + 1):
            grid[i][ii] = k
            square = (i // n * n, ii // n * n)
            if valid_square(grid, square, n) == True and valid_row(grid, i, nn) == True and valid_col(grid, ii, nn) == True:
                key = to_key(grid)
                if key not in visited:
                    heappush(Q, (current_idx - 1, random(), deepcopy(grid)))
                    visited.add(key)
    return None

input = open('input.txt').read().splitlines()
n = isqrt(len(input))
grid = []
for line in input:
    grid.append([int(x) for x in line.split(' ')])

if valid_state(grid, n) == False:
    raise Exception('Invalid initial state')

solution = get_solution(grid, n)
if solution is None:
    print('Solution not found')
    exit()
    
for row in solution:
    print(row)