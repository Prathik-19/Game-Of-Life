#include "universe.h"

#include <getopt.h>
#include <inttypes.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "tsn:i:o:"
#define DELAY   50000

FILE *infile = NULL, *outfile = NULL;
Universe *A = NULL, *B = NULL;

void cleanup(void) {

    if (A) {
        uv_delete(A);
    }
    if (B) {
        uv_delete(B);
    }
    if (outfile) {
        fclose(outfile);
    }
    if (infile) {
        fclose(infile);
    }
}

void evolve(Universe *src, Universe *dst) {
    for (uint32_t r = 0; r < uv_rows(src); r++) {
        for (uint32_t c = 0; c < uv_cols(src); c++) {
            bool isAlive = uv_get_cell(src, r, c);
            uint32_t n = uv_census(src, r, c);
            if (isAlive) {
                if (n == 2 || n == 3) {
                    uv_live_cell(dst, r, c);
                } else {
                    uv_dead_cell(dst, r, c);
                }
            } else {
                if (n == 3) {
                    uv_live_cell(dst, r, c);
                } else {
                    uv_dead_cell(dst, r, c);
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    infile = stdin;
    outfile = stdout;
    bool toroidal = false, silent = false;
    uint32_t generations = 100;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 's': silent = true; break;
        case 't': toroidal = true; break;
        case 'n': generations = strtoul(optarg, NULL, 10); break;
        case 'o':
            outfile = fopen(optarg, "w");
            if (!outfile) {
                perror("error opening outfile: ");
                cleanup();
                return EXIT_FAILURE;
            }
            break;
        case 'i':
            infile = fopen(optarg, "r");
            if (!infile) {
                perror("error opening infile: ");
                cleanup();
                return EXIT_FAILURE;
            }
            break;
        default:
            fprintf(stderr, "usage: %s [-ts] [-n generations] [-i infile] [-o outfile]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    uint32_t rows, cols;
    if (fscanf(infile, "%" SCNu32 " %" SCNu32, &rows, &cols) != 2) {
        fprintf(stderr, "invalid input file\n");
        cleanup();
        return EXIT_FAILURE;
    }

    A = uv_create(rows, cols, toroidal);
    B = uv_create(rows, cols, toroidal);
    uv_populate(A, infile);

    if (silent == false) {
        initscr();
        curs_set(false);
    }

    for (uint32_t g = 0; g < generations; g += 1) {
        Universe *src = g % 2 ? B : A, *dst = g % 2 ? A : B;
        evolve(src, dst);

        if (silent == false) {
            clear();
            for (uint32_t r = 0; r < uv_rows(dst); r++) {
                for (uint32_t c = 0; c < uv_cols(dst); c++) {
                    if (uv_get_cell(dst, r, c)) {
                        mvprintw(r, c, "o");
                    }
                }
            }
            refresh();
            usleep(DELAY);
        }
    }

    if (silent == false) {
        endwin();
    }

    uv_print(generations % 2 ? B : A, outfile);
    cleanup();

    return EXIT_SUCCESS;
}
