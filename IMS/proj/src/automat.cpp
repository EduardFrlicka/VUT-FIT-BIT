#include "automat.h"

#define FOR_CELL for (int x = 0, y = 0; y < args->y; ++x, y += (x == args->x), x = (x >= args->x) ? 0 : x)

CA::CA(args_t *args) {
    this->args = args;
    cells.resize(args->x, std::vector<Cell>(args->y));

    FOR_CELL {
        Cell *cell = &(cells[x][y]);

        add_moore_neighbours(cell, x, y);
        add_von_neuman_neighbours(cell, x, y);

        cell->x = x;
        cell->y = y;
        cell->to_print = &to_print;

        to_print.push_back(cell);
    }
}

CA::~CA() {
}

void CA::set_seed(int seed) {
    srand(seed);
}

int CA::iterate() {
    FOR_CELL
    cells[x][y].iterate_wolf(args);

    FOR_CELL
    cells[x][y].iterate_cougar(args);

    FOR_CELL
    cells[x][y].iterate_sheep(args);

    FOR_CELL
    cells[x][y].end_iteration();

    return 0;
}

bool CA::in_bounds(int x, int y) {
    return (x >= 0 && x < args->x) && (y >= 0 && y < args->y);
}

void CA::add_von_neuman_neighbours(Cell *cell, int x, int y) {
    int nx, ny;
    int von_neuman[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (auto vec : von_neuman) {
        nx = x + vec[0];
        ny = y + vec[1];
        if (!in_bounds(nx, ny))
            continue;
        cell->add_neighbour_von_neuman(&(cells[nx][ny]));
    }
}

void CA::add_moore_neighbours(Cell *cell, int x, int y) {
    int nx, ny;
    int moore[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    for (auto vec : moore) {
        nx = x + vec[0];
        ny = y + vec[1];
        if (!in_bounds(nx, ny))
            continue;
        cell->add_neighbour_moore(&(cells[nx][ny]));
    }
}

void CA::generate() {
    int sheep_count = 0;
    int wolf_count = 0;
    int cougar_count = 0;

    if (args->initial_wolf_count + args->initial_sheep_count + args->initial_cougar_count >= (args->x * args->y / 4 * 3))
        ERROR(1, "Initial count of wolfes and sheeps and cougars too high for plot size %d x %d", args->x, args->y);

    while (wolf_count < args->initial_wolf_count) {
        int rx = rand() % args->x;
        int ry = rand() % args->y;

        if (cells[rx][ry].type == cell_empty) {
            if (cells[rx][ry].generate_wolf(args))
                ++wolf_count;
        }
    }

    while (sheep_count < args->initial_sheep_count) {
        int rx = rand() % args->x;
        int ry = rand() % args->y;

        if (cells[rx][ry].type == cell_empty) {
            if (cells[rx][ry].generate_sheep(args))
                ++sheep_count;
        }
    }

    while (cougar_count < args->initial_cougar_count) {
        int rx = rand() % args->x;
        int ry = rand() % args->y;

        if (cells[rx][ry].type == cell_empty) {
            if (cells[rx][ry].generate_cougar(args))
                ++cougar_count;
        }
    }
}

void CA::print_cell(Cell *cell) {
    int x;
    int y;
    x = cell->x * 2 + 1;
    y = cell->y + 2;

    if (args->tty)
        printf("\033[%d;%dH", y, x);

    if (args->colors) {
        char *color;
        switch (cell->type) {
        case cell_sheep:
            color = (char *)"\033[47m";
            break;

        case cell_wolf:
            color = (char *)"\033[40m";
            break;

        case cell_empty:
            color = (char *)"\033[48;5;22m";
            break;

        case cell_cougar:
            color = (char *)"\033[45m";
            break;

        default:
            color = (char *)"\033[41m";
            break;
        }
        printf("%s  " ASCII_RESET, color);
    } else {
        char c = ' ';
        switch (cell->type) {
        case cell_sheep:
            c = '.';
            break;

        case cell_empty:
            c = ' ';
            break;

        case cell_wolf:
            c = '#';
            break;

        case cell_cougar:
            c = 'O';
            break;

        default:
            c = 'E';
            break;
        }
        printf(" %c", c);
    }
}

void CA::print_cell(int x, int y) {
    print_cell(&(cells[x][y]));
}

int CA::wolf_count() {
    int sum = 0;

    FOR_CELL
    if (cells[x][y].type == cell_wolf)
        ++sum;

    return sum;
}

int CA::sheep_count() {
    int sum = 0;

    FOR_CELL
    if (cells[x][y].type == cell_sheep)
        ++sum;

    return sum;
}

int CA::cougar_count() {
    int sum = 0;

    FOR_CELL
    if (cells[x][y].type == cell_cougar)
        ++sum;

    return sum;
}

int CA::empty_count() {
    int sum = 0;

    FOR_CELL
    if (cells[x][y].type == cell_empty)
        ++sum;

    return sum;
}

void CA::print_matrix() {
    for (auto cell : to_print) {
        print_cell(cell);
    }
    fflush(stdout);
    to_print.clear();

    // uint delim_size;
    // char *delim;

    // if (args->colors)
    //     delim_size = args->x * 2;
    // else
    //     delim_size = args->x * 2 + 1;

    // delim = (char *)calloc(delim_size + 1, 1);
    // memset(delim, '=', delim_size);

    // printf("%s\n", delim);
    // for (int y = 0; y < args->y; ++y) {
    //     for (int x = 0; x < args->x; ++x) {
    //         print_cell(x, y);
    //     }
    //     printf("\n");
    // }
    // printf("%s\n", delim);
}
