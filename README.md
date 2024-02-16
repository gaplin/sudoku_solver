# Sudoku solver

Python and C solution for solving and generating sudoku puzzles. </br>

### Python usage
- Solver </br>
    To solve the puzzle you need to run `solve.py` script. </br>
    Input is taken from stdin. Should be of the form: 9 rows with 9 numbers separated by spaces where. For missing entries any character can be used. </br>
    Example input:
    ```
    _ _ 9 _ _ _ _ 7 _
    _ _ 3 _ _ 2 4 _ _
    _ _ _ _ _ _ 2 5 _
    7 _ _ 6 _ _ _ _ _
    _ _ 4 3 _ 8 _ _ _
    _ _ _ _ 5 _ 6 _ _
    4 _ 8 _ _ 3 _ _ _
    _ _ _ _ 8 _ _ 9 3
    _ 1 _ _ _ 9 5 _ _
    ```
    Output is written to stdout. </br>
    Example usage (assuming that input is in the file puzzle.txt):
    ```console
    foo@bar:src/python$ python3 solve.py < puzzle.txt
    6 2 9 8 4 5 3 7 1
    1 5 3 7 6 2 4 8 9
    8 4 7 9 3 1 2 5 6
    7 3 1 6 9 4 8 2 5
    5 6 4 3 2 8 9 1 7
    9 8 2 1 5 7 6 3 4
    4 9 8 5 1 3 7 6 2
    2 7 5 4 8 6 1 9 3
    3 1 6 2 7 9 5 4 8
    ```
- Generator </br>
    To generate random puzzle you need to run `gen.py` script. </br>
    It takes as an argument number of clues. Set to 30 by default. </br>
    For number of clues lower than 25 generation speed decreases significantly. </br>
    Generation produces two files: puzzle.txt with puzzle and solution.txt with solution. </br>
    Produced puzzle always has **unique** solution. </br>
    Example usage:
    ```console
    foo@bar:src/python$ python3 gen.py
    ```
    Sample output:
    - puzzle.txt
        ```
        3 _ _ _ _ 6 8 _ 2
        _ 4 _ _ _ _ _ 9 _
        8 _ _ _ _ _ _ _ 1
        _ _ _ _ _ _ _ _ _
        _ _ _ 9 _ _ 7 _ 3
        _ 5 2 1 _ _ _ _ 6
        9 2 _ _ _ _ 5 _ 7
        _ 8 _ _ 3 _ _ 2 _
        _ _ _ 5 2 _ _ 4 _
        ```
    - solution.txt
        ```
        3 1 5 4 9 6 8 7 2
        2 4 7 3 8 1 6 9 5
        8 9 6 2 7 5 4 3 1
        1 3 9 7 6 8 2 5 4
        4 6 8 9 5 2 7 1 3
        7 5 2 1 4 3 9 8 6
        9 2 3 8 1 4 5 6 7
        5 8 4 6 3 7 1 2 9
        6 7 1 5 2 9 3 4 8
        ```