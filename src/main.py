from copy import deepcopy
from heapq import heappop, heappush
import random
import sys

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

def get_solutions(initial_grid: list, n: int, limit: int, randomize: bool, prohibited: set = set()) -> list:
    missing_entries = []
    result = []
    nn = n ** 2
    for i in range(nn):
        for ii in range(nn):
            if initial_grid[i][ii] == 0:
                missing_entries.append((i, ii))

    def random_or_normal():
        return random.random() if randomize == True else 0
    

    N = len(missing_entries) - 1
    Q = [(N, random_or_normal(), initial_grid)]
    nums = [i for i in range(1, nn + 1)]

    while Q:
        current_idx, _, grid = heappop(Q)
        if current_idx == -1:
            result.append(grid)
            if len(result) == limit:
                break
            continue
        i, ii = missing_entries[current_idx]

        if randomize == True:
            random.shuffle(nums)

        for k in nums:
            if (i, ii, k) in prohibited:
                continue
            grid[i][ii] = k
            square = (i // n * n, ii // n * n)
            if valid_square(grid, square, n) == True and valid_row(grid, i, nn) == True and valid_col(grid, ii, nn) == True:
                heappush(Q, (current_idx - 1, random_or_normal(), deepcopy(grid)))
            
            grid[i][ii] = 0
            
    return result

def generate_grid(n: int, filled_entries: int) -> (list, list):
    nn = n ** 2
    empty_grid = [[0 for _ in range(nn)] for _ in range(nn)]
    idxes = [(i, ii) for i in range(nn) for ii in range(nn)]
    empty_entries = nn ** 2 - filled_entries
    retries = 500
    
    while True:
        random_solution = get_solutions(empty_grid, n, 1, True)[0]
        if empty_entries == 0:
            return (random_solution, random_solution)
        for _ in range(retries):
            empty_idxes = random.sample(idxes, empty_entries)
            grid_cpy = deepcopy(random_solution)
            prohibited = set()
            for i, ii in empty_idxes:
                prohibited.add((i, ii, grid_cpy[i][ii]))
                grid_cpy[i][ii] = 0

            solutions = get_solutions(grid_cpy, n, 1, False, prohibited)
            if solutions == []:
                return (grid_cpy, random_solution)

n = 3
nn = n ** 2
filled_entries = 30
if len(sys.argv) > 1:
    filled_entries = int(sys.argv[1])
    
assert 17 < filled_entries <= nn ** 2

(puzzle, solution) = generate_grid(n, filled_entries)
puzzle_file = open('puzzle.txt', 'w')
for row in puzzle:
    puzzle_file.write(' '.join([str(x) if x != 0 else '_' for x in row]) + '\n')
puzzle_file.close()

solution_file = open('solution.txt', 'w')
for row in solution:
    solution_file.write(' '.join([str(x) if x != 0 else '_' for x in row]) + '\n')
solution_file.close()