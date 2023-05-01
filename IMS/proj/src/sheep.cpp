#include "cell.h"

void Cell::iterate_sheep(args_t *args) {
    if (type != cell_sheep)
        return;

    if (!age(args))
        return;

    reproduction_sheep(args);

    // movement();
}

void Cell::reproduction_sheep(args_t *args) {
    std::vector<Cell *> empty;
    int r;

    /*checking wolf age*/
    if (years < args->reproduction_age_sheep)
        return;

    /*checking rate of reproduction*/
    if ((years - args->reproduction_age_sheep) % args->reproduction_rate_sheep)
        return;

    /*finding empty cell where child will be on next iteration*/
    empty.clear();
    for (auto neighbour : von_neuman_neighbours)
        if (neighbour->future_type == cell_empty)
            empty.push_back(neighbour);

    /*no place for child*/
    if (empty.size() <= 0)
        return;

    r = rand() % empty.size();
    reproduce(empty[r], args->mutation_rate_sheep);
}

bool Cell::generate_sheep(args_t *args) {
    generate_random_history();

    years = rand() % args->reproduction_rate_sheep;
    type = cell_sheep;
    future_type = cell_sheep;
    fittness = 0;
    hungry = 0;

    for (int i = 0; i < years; ++i)
        fittness += life_history[i];

    if (fittness >= args->fitness_treshold_sheep) {
        kill();
        return false;
    }
    return true;
}