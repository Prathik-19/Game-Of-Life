#include "universe.h"

#include <inttypes.h>
#include <stdlib.h>

struct Universe {
    bool toroidal;
    uint32_t cols;
    uint32_t rows;
    bool **grid;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) malloc(sizeof(Universe));
    if (u) {
        u->toroidal = toroidal;
        u->cols = cols;
        u->rows = rows;
        u->grid = (bool **) calloc(rows, sizeof(bool *));

        if (u->grid) {
            for (uint32_t i = 0; i < rows; i++) {
                u->grid[i] = (bool *) calloc(cols, sizeof(bool));
                if (!u->grid[i]) {
                    for (uint32_t j = 0; j < i && u->grid[j]; j++) {
                        free(u->grid[j]);
                    }
                    free(u->grid);
                    free(u);
                    return NULL;
                }
            }
        } else {
            free(u);
            u = NULL;
        }
    }

    return u;
}

void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i++) {
        free(u->grid[i]);
    }
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (c < u->cols && r < u->rows) {
        u->grid[r][c] = true;
    }
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (c < u->cols && r < u->rows) {
        u->grid[r][c] = false;
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (c < u->cols && r < u->rows) {
        return u->grid[r][c];
    } else {
        return false;
    }
}

bool uv_populate(Universe *u, FILE *infile) {
    while (&fread) {
        uint32_t c;
        uint32_t r;
        if (fscanf(infile, "%" SCNu32 " %" SCNu32, &r, &c) != 2) {
            return false;
        } else if (r > u->rows || c > u->cols) {
            return false;
        } else {
            uv_live_cell(u, r, c);
        }
    }

    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t count = 0;

    int8_t offsets[8][2] = {
        { -1, -1 },
        { -1, 0 },
        { -1, 1 },
        { 0, -1 },
        { 0, 1 },
        { 1, -1 },
        { 1, 0 },
        { 1, 1 },
    };

    for (uint32_t i = 0; i < 8; i += 1) {
        int8_t col_offset = offsets[i][1];
        int8_t row_offset = offsets[i][0];
        int64_t col = (int64_t) c + col_offset;
        int64_t row = (int64_t) r + row_offset;

        if (row >= 0 && row < u->rows) {
            if (col >= 0 && col < u->cols) {
                count += u->grid[row][col] ? 1 : 0;
            }
        }

        if (u->toroidal) {
            col = (col + u->cols) % u->cols;
            row = (row + u->rows) % u->rows;
        }
    }

    return count;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t r = 0; r < u->rows; r++) {
        for (uint32_t c = 0; c < u->cols; c++) {
            fputc(u->grid[r][c] ? 'o' : '.', outfile);
        }
        fputc('\n', outfile);
    }
}
