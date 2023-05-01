#include "args.h"
#include "automat.h"
#include "cell.h"
#include "csv.h"
#include <unistd.h>

int main(int, char *argv[]) {
    args_t args;

    load_args(argv, &args);

    CA automat = CA(&args);
    automat.set_seed(args.seed);
    automat.generate();


    csvWriter csv = csvWriter(&automat, &args);
    
    print_conf(&args);

    if (args.tty && args.print)
        printf("\033[H\033[3J\033[2J\033[?25l");

    if (args.print) {
        printf("Iteration: %d, Wolf count: %d, Sheep count: %d, seed: %d\n", 0, automat.wolf_count(), automat.sheep_count(), args.seed);
        automat.print_matrix();
    }

    for (int i = 1; i < args.iterations; ++i) {
        csv.log();

        if (args.print)
            usleep(args.sleep * 1000);

        automat.iterate();

        if (args.print) {
            printf("\033[H\033[KIteration: %d, Wolf count: %d, Sheep count: %d, seed: %d", i, automat.wolf_count(), automat.sheep_count(), args.seed);
            automat.print_matrix();
        }
    }

    if (args.print)
        automat.print_matrix();

    if (args.tty && args.print)
        printf("\033[?25h\033[%d;%dH", args.y + 3, 0);
}
