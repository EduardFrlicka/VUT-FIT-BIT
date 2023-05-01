#pragma once

#include "args.h"
#include "cell.h"
#include "error.h"

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

class CA {
  public:
    CA(args_t *args);
    ~CA();
    void generate();
    int iterate();
    void set_seed(int seed);
    void print_matrix();
    int wolf_count();
    int sheep_count();
    int cougar_count();
    int empty_count();

    std::vector<std::vector<Cell>> cells;

    std::vector<Cell *> to_print;

  private:
    // void change_cell(Cell *cell, cell_types new_type);
    void print_cell(int x, int y);
    void print_cell(Cell *cell);
    void remove_cell(std::vector<Cell *> &, Cell *);
    bool in_bounds(int, int);
    void add_von_neuman_neighbours(Cell *, int, int);
    void add_moore_neighbours(Cell *, int, int);
    args_t *args;
};