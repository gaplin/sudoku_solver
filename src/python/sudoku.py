from copy import deepcopy
import random

def __valid_square(grid: list, cords: tuple, n: int) -> bool:
    i, ii = cords
    i = i // n * n
    ii = ii // n * n
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

def get_random_solution(grid: list, n: int) -> list:
    def inner_get_solution(grid: list, n: int, nn: int, zeros: list, m: int, idx: int) -> list:
        zero = zeros[idx]
        i, ii = zero
        nums = [x for x in range(1, nn + 1)]
        random.shuffle(nums)
        for k in nums:
            grid[i][ii] = k
            if __valid_col(grid, ii, nn) and __valid_row(grid, i, nn) and __valid_square(grid, zero, n):
                if idx + 1 == m:
                    return grid
                grid_cpy = deepcopy(grid)
                solution = inner_get_solution(grid_cpy, n, nn, zeros, m, idx + 1)
                grid[i][ii] = 0
                if solution != None:
                    return solution
            grid[i][ii] = 0

        return None
    
    zeros = []
    nn = n ** 2
    for i in range(nn):
        for ii in range(nn):
            if grid[i][ii] == 0:
                zeros.append((i, ii))
    nn = n ** 2
    return inner_get_solution(grid, n, n ** 2, zeros, len(zeros), 0)

def count_solutions(grid: list, n: int, limit: int) -> int:
    def inner_count_solutions(grid: list, n: int, nn: int, limit: int, zeros: list, m: int, idx: int):
        if limit <= 0:
            return 0
        zero = zeros[idx]
        result = 0
        i, ii = zero
        for k in range(1, nn + 1):
            grid[i][ii] = k
            if __valid_col(grid, ii, nn) and __valid_row(grid, i, nn) and __valid_square(grid, zero, n):
                if idx + 1 == m:
                    solutions = 1
                else:
                    solutions = inner_count_solutions(grid, n, nn, limit - result, zeros, m, idx + 1)
                grid[i][ii] = 0
                result += solutions
                if result >= limit:
                    return result

        grid[i][ii] = 0
        return result
    
    zeros = []
    nn = n ** 2
    for i in range(nn):
        for ii in range(nn):
            if grid[i][ii] == 0:
                zeros.append((i, ii))
    return inner_count_solutions(grid, n, n ** 2, limit, zeros, len(zeros), 0)

def generate_grid(n: int, filled_entries: int) -> list:
    nn = n ** 2
    N = nn ** 2
    empty_grid = [[0 for _ in range(nn)] for _ in range(nn)]
    idxes = [(i, ii) for i in range(nn) for ii in range(nn)]
    empty_entries = N - filled_entries
    random_solution = get_random_solution(empty_grid, n)
    if empty_entries == 0:
        return (random_solution, random_solution)
    
    while True:
        random.shuffle(idxes)
        grid_cpy = deepcopy(random_solution)
        left = empty_entries
        m = N - 1
        while m + 1 >= left and left > 0:
            i, ii = idxes[m]
            m -= 1
            grid_cpy[i][ii] = 0
            solutions = count_solutions(grid_cpy, n, 2)
            if solutions == 1:
                left -= 1
            else:
                grid_cpy[i][ii] = random_solution[i][ii]

        if left == 0:
            return (grid_cpy, random_solution)
        else:
            random_solution = get_random_solution(empty_grid, n)