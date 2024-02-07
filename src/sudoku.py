from copy import deepcopy
from heapq import heappop, heappush
import random

def __valid_square(grid: list, top_left: tuple, n: int) -> bool:
    i, ii = top_left
    nums = set()
    for y in range(i, i + n):
        for x in range(ii, ii + n):
            if grid[y][x] != 0 and grid[y][x] in nums:
                return False
            nums.add(grid[y][x])
            
    return True

def __valid_row(grid: list, i: int, nn: int) -> bool:
    nums = set()
    for ii in range(nn):
        if grid[i][ii] != 0 and grid[i][ii] in nums:
            return False
        nums.add(grid[i][ii])

    return True

def __valid_col(grid: list, ii: int, nn: int) -> bool:
    nums = set()
    for i in range(nn):
        if grid[i][ii] != 0 and grid[i][ii] in nums:
            return False
        nums.add(grid[i][ii])

    return True

def get_solutions(initial_grid: list, n: int, limit: int, randomize: bool) -> list:
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
            grid[i][ii] = k
            square = (i // n * n, ii // n * n)
            if __valid_square(grid, square, n) == True and __valid_row(grid, i, nn) == True and __valid_col(grid, ii, nn) == True:
                heappush(Q, (current_idx - 1, random_or_normal(), deepcopy(grid)))
            
            grid[i][ii] = 0
    
    return result

def generate_grid(n: int, filled_entries: int) -> list:
    nn = n ** 2
    empty_grid = [[0 for _ in range(nn)] for _ in range(nn)]
    idxes = [(i, ii) for i in range(nn) for ii in range(nn)]
    empty_entries = nn ** 2 - filled_entries
    random_solution = get_solutions(empty_grid, n, 1, True)[0]
    if empty_entries == 0:
        return (random_solution, random_solution)
    
    while True:
        empty_idxes = random.sample(idxes, empty_entries)
        grid_cpy = deepcopy(random_solution)
        for i, ii in empty_idxes:
            grid_cpy[i][ii] = 0

            solutions = get_solutions(grid_cpy, n, 2, True)
            if len(solutions) != 1:
                break
        else:
            return (grid_cpy, random_solution)