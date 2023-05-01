#include "cell.h"

Cell::Cell() {
    type = cell_empty;
    std::vector<Cell> neighbours;
}

Cell::~Cell() {
}

bool Cell::age(args_t *args) {
    if (type == cell_empty)
        return false;

    ++this->years;
    if (this->years >= LIFE_HISTORY_SIZE) {
        kill();
        return false;
    }

    fittness += life_history[years] > 0;

    if (type == cell_sheep && fittness >= args->fitness_treshold_sheep) {
        kill();
        return false;
    }

    if (type == cell_wolf && fittness >= args->fitness_treshold_wolf) {
        kill();
        return false;
    }
    return true;
}

void Cell::end_iteration() {
    type = future_type;
    future_type = type;
}

void Cell::kill() {
    type = cell_empty;
    future_type = cell_empty;
    years = 0;
    fittness = 0;
    hungry = 0;

    memset(life_history, 0, LIFE_HISTORY_SIZE);

    to_print->push_back(this);
}

int Cell::neighbour_wolfs() {
    int sum = 0;
    for (auto n : von_neuman_neighbours) {
        if (n->type == cell_wolf)
            ++sum;
    }
    return sum;
}

int Cell::neighbour_sheeps() {
    int sum = 0;
    for (auto n : von_neuman_neighbours) {
        if (n->type == cell_sheep)
            ++sum;
    }
    return sum;
}

void Cell::movement() {
    std::vector<Cell *> empty;
    int r;

    empty.clear();

    for (auto neighbour : von_neuman_neighbours)
        if (neighbour->future_type == cell_empty)
            empty.push_back(neighbour);

    if (empty.size() <= 0)
        return;

    r = rand() % empty.size();

    move(empty[r]);
}

void Cell::move(Cell *target) {

    target->future_type = this->type;
    target->years = this->years;
    target->fittness = this->fittness;
    target->hungry = this->hungry;

    memcpy(target->life_history, this->life_history, LIFE_HISTORY_SIZE);

    kill();

    to_print->push_back(target);
}

void Cell::reproduce(Cell *target, int mutation_rate) {
    int mutations = 0;
    int r;

    /*protection against infinite loop*/
    if (mutations > LIFE_HISTORY_SIZE)
        ERROR(1, "mutation rate bigger than lifespan (lifespan = %d)", LIFE_HISTORY_SIZE);

    /*cannot reproduce empty cell*/
    if (type == cell_empty)
        return;

    /*cannot reproduce to non emplty cell*/
    if (target->future_type != cell_empty)
        return;

    /*setting parameters*/
    target->future_type = this->type;
    target->years = 0;
    target->hungry = 0;
    memcpy(target->life_history, this->life_history, LIFE_HISTORY_SIZE);

    while (mutations < mutation_rate) {
        /*pick random index to mutation*/
        r = rand() % LIFE_HISTORY_SIZE;

        /*check if was already changed*/
        if (target->life_history[r] != this->life_history[r])
            continue;

        /*mutate*/
        target->life_history[r] = !target->life_history[r];
        ++mutations;
    }

    target->fittness = target->life_history[0] > 0;
}

void Cell::iterate(args_t *args) {
    switch (type) {
    case cell_sheep:
        iterate_sheep(args);
        break;
    case cell_wolf:
        iterate_wolf(args);
        break;
    case cell_cougar:
        iterate_cougar(args);
        break;
    default:
        break;
    }
}

void Cell::add_neighbour_moore(Cell *cell) {
    moore_neighbours.push_back(cell);
}

void Cell::add_neighbour_von_neuman(Cell *cell) {
    von_neuman_neighbours.push_back(cell);
}

void Cell::generate_random_history() {
    for (int i = 0; i < LIFE_HISTORY_SIZE; ++i)
        life_history[i] = rand() % 2;
}