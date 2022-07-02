# Homework 01 - Game of life
#
# Your task is to implement part of the cell automata called
# Game of life. The automata is a 2D simulation where each cell
# on the grid is either dead or alive.
#
# State of each cell is updated in every iteration based state of neighbouring cells.
# Cell neighbours are cells that are horizontally, vertically, or diagonally adjacent.
#
# Rules for update are as follows:
#
# 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
# 2. Any live cell with two or three live neighbours lives on to the next generation.
# 3. Any live cell with more than three live neighbours dies, as if by overpopulation.
# 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
#
#
# Our implementation will use coordinate system will use grid coordinates starting from (0, 0) - upper left corner.
# The first coordinate is row and second is column.
#
# Do not use wrap around (toroid) when reaching edge of the board.
#
# For more details about Game of Life, see Wikipedia - https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life


def living_around(cell, alive):
    x, y = cell
    count = 0

    for i in range(-1, 2):
        for j in range(-1, 2):
            if (x + i, y + j) in alive:
                count += 1

    if cell in alive:
        count -= 1  # do not count self

    return count


def update(alive, size, iter_n):
    size_x, size_y = size
    empty = [(x, y) for x in range(size_x) for y in range(size_y)]

    for _ in range(iter_n):
        born = set()
        die = set()

        for cell in empty:
            if cell not in alive and living_around(cell, alive) == 3:
                born.add(cell)

        for cell in empty:
            if cell in alive and not living_around(cell, alive) == 2 and not living_around(cell, alive) == 3:
                die.add(cell)

        alive = alive.difference(die)
        alive = alive.union(born)

    return alive


def draw(alive, size):
    """
    alive - set of cell coordinates marked as alive, can be empty
    size - size of simulation grid as tuple - (

    output - string showing the board state with alive cells marked with X
    """

    size_x, size_y = size
    outline = '+' + '-' * size_y + '+'

    board_drawing = outline

    for x in range(size_x):
        tmp = ''
        for y in range(size_y):
            tmp = tmp + ('X' if (x, y) in alive else ' ')
        
        board_drawing = board_drawing + '\n|' + tmp + '|'

    board_drawing = board_drawing + '\n' + outline

    return board_drawing
