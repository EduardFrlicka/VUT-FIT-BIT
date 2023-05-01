#include "cell.h"

bool Cell::generate_cougar(args_t *args) {
    generate_random_history();

    years = rand() % args->reproduction_rate_cougar;
    type = cell_cougar;
    future_type = cell_cougar;
    fittness = 0;

    for (int i = 0; i < years; ++i)
        fittness += life_history[i];

    if (fittness >= args->fitness_treshold_sheep) {
        kill();
        return false;
    }

    return true;
}

bool Cell::eating_cougar() {
    int max_fittness = 0;
    std::vector<Cell *> max_fittness_sheeps;
    for (auto neighbour : von_neuman_neighbours) {
        if (neighbour->type != cell_sheep)
            continue;
        if (neighbour->fittness < max_fittness)
            continue;

        if (neighbour->fittness == max_fittness) {
            max_fittness_sheeps.push_back(neighbour);
            continue;
        }

        max_fittness = neighbour->fittness;
        max_fittness_sheeps.clear();
        max_fittness_sheeps.push_back(neighbour);
    }

    if (max_fittness_sheeps.size() <= 0)
        return false;

    int r = rand() % max_fittness_sheeps.size();
    max_fittness_sheeps[r]->kill();
    hungry = 0;
    move(max_fittness_sheeps[r]);

    return true;
}

void Cell::iterate_cougar(args_t *args) {
    bool was_eating;

    if (type != cell_cougar)
        return;

    if (!age(args))
        return;

    ++hungry;

    if (hungry >= args->hold_cougar) {
        kill();
        return;
    }

    reproduction_cougar(args);

    was_eating = eating_cougar();

    if (!was_eating)
        movement();
}

void Cell::reproduction_cougar(args_t *args) {
    std::vector<Cell *> empty;
    int r;

    /*checking cougar age*/
    if (years < args->reproduction_age_cougar)
        return;

    /*checking rate of reproduction*/
    if ((years - args->reproduction_age_cougar) % args->reproduction_rate_cougar)
        return;

    /*finding empty cell where child will be on next iteration*/
    empty.clear();
    for (auto neighbour : moore_neighbours)
        if (neighbour->future_type == cell_empty)
            empty.push_back(neighbour);

    /*no place for child*/
    if (empty.size() <= 0)
        return;

    r = rand() % empty.size();
    reproduce(empty[r], args->mutation_rate_cougar);
}
