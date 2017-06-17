#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 1000
#define MEMSIZE 5

struct Point;
struct Board;

void addPointtoBorder(struct Board *, const struct Point);
void fillBoard(struct Board *, char const *);
void dbPrint(const struct Board * const);
struct Board* initialBoard();
void destroyBoard(struct Board *);

struct Point{
    int x;
    int y;
};

struct Board{
    size_t npoints;
    struct Point *ppoints;
};

int main(int argc, char **argv) {
    char const * fname = (argc > 1) ? argv[1] : "/home/slowbro/CLionProjects/untitled/coords.txt";
    int radius = (argc > 2) ? atoi(argv[2]) : 10;

    struct Board *board = initialBoard();
    fillBoard(board, fname);


    dbPrint(board);
    destroyBoard(board);
    return 0;
}


struct Board* initialBoard(){
    struct Board *board = (struct Board*)malloc(sizeof(struct Board));
    board->npoints = 0;
    board->ppoints = (struct Point*)malloc(MEMSIZE * sizeof(struct Point));
    return board;
}
void destroyBoard(struct Board *board){
    if (board == NULL)
        return;
    if (board->npoints != NULL)
        free(board->ppoints);
    free(board);
}

void addPointtoBorder(struct Board *board, const struct Point point){
    board->npoints += 1;
    if (board->npoints % MEMSIZE == 0){
        printf("Calloc\n");
        board->ppoints = (struct Point*)realloc(board->ppoints, sizeof(struct Point) * (board->npoints + MEMSIZE));
    }
    board->ppoints[(board->npoints) - 1] = point;
}
void fillBoard(struct Board *board, char const *fname){
    FILE * fp = fopen(fname, "r");
    struct Point c;
    if (fp == NULL) {
        printf("Cannot open file [ %s ]\n", fname);
        return;
    }
    while(fscanf(fp,"%d,%d", &c.x, &c.y) == 2){
        addPointtoBorder(board, c);
    }
    fclose(fp);
}

void dbPrint(const struct Board * const board){
    if (board == NULL)
        return;
    size_t npoints = board->npoints;
    int i = 0;
    printf("Points: %d\n", npoints);
    for(i; i < npoints; ++i){
        printf("%d: (%d : %d) <--- %p\n",i, board->ppoints[i].x, board->ppoints[i].y, &board->ppoints[i]);
    }
}