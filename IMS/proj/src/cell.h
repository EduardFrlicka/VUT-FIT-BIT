#pragma once
#include "args.h"
#include "error.h"

#include <stdlib.h>
#include <vector>

#define LIFE_HISTORY_SIZE 32

void generate_random_life_history(char *target);

enum cell_types {
    cell_empty,
    cell_wolf,
    cell_sheep,
    cell_cougar,
};

class Cell {
  public:
    Cell();
    ~Cell();

    /*
      GENERATION
    */

    void add_neighbour_moore(Cell *);
    void add_neighbour_von_neuman(Cell *);

    bool generate_wolf(args_t *);
    bool generate_sheep(args_t *);
    bool generate_cougar(args_t *);

    /*
      ITERATION
    */

    void iterate(args_t *);
    void iterate_wolf(args_t *);
    void iterate_sheep(args_t *);
    void iterate_cougar(args_t *);

    bool age(args_t *);
    void kill();

    void end_iteration();

    enum cell_types type;
    enum cell_types future_type;
    std::vector<Cell *> *to_print;

    int x;
    int y;

  private:
    int fittness;
    int years;
    char life_history[LIFE_HISTORY_SIZE];

    int hungry;

    std::vector<Cell *> von_neuman_neighbours;
    std::vector<Cell *> moore_neighbours;

    void reproduction_wolf(args_t *);
    void reproduction_sheep(args_t *);
    void reproduction_cougar(args_t *);

    bool canibal_wolf();

    bool eating_wolf();
    bool eating_cougar();
    void move(Cell *);
    void movement();
    void reproduce(Cell *, int);
    void clear();

    void smart_movement();
    int neighbour_wolfs();
    int neighbour_sheeps();

    void generate_random_history();
};
