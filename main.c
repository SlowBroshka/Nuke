#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_POINTS 1000
#define MEMSIZE 5

struct Point;
struct Board;
struct PairPoints;

void addPointtoBorder(struct Board *, const struct Point);
void fillBoard(struct Board *, char const *);
void dbPrint(const struct Board * const);
struct Board* initialBoard();
void destroyBoard(struct Board *);

double_t getDistance(const struct Point, const struct Point);
struct PairPoints getCrossPoints(const struct Point, const struct Point,
                                 const double_t, const size_t);

struct Point getPoint(const struct Board *, const size_t);
size_t getNumPoints(const struct Board *);
struct Point getCrossPoint(const struct Point, const struct Point,
                           const double_t, const size_t);

bool inRadius(const struct Point, const size_t, const struct Point);
bool isCross(const struct Point, const struct Point, const size_t);
bool isCrossd(const double_t, const size_t);


struct Point{
    double x;
    double y;
};
struct PairPoints{
    struct Point p1;
    struct Point p2;
};
struct Board{
    size_t npoints;
    struct Point *ppoints;
};

int main(int argc, char **argv) {
    char const * fname = (argc > 1) ? argv[1] : "../coords.txt";
    size_t radius = (argc > 2) ? (size_t)atoi(argv[2]) : 10;

    struct Board *board = initialBoard();
    fillBoard(board, fname);

    size_t i = 0;
    size_t j = 0;
    size_t npoints = getNumPoints(board);
    struct Board *nukePoints = initialBoard();

    for (i = 0; i < npoints; ++i){
        for(j = i; j < npoints; ++j){
            struct Point p1 = getPoint(board, i);
            struct Point p2 = getPoint(board, j);
            double_t distance = getDistance(p1, p2);
            if (isCrossd(distance, radius)){
                if (radius + radius == distance){
                    struct Point p = getCrossPoint(p1, p2, distance, radius);
                    printf("Point1:( %.2f : %.2f )^( %.2f : %.2f ) = ( %.2f : %.2f )\n",
                           p1.x, p1.y, p2.x, p2.y, p.x, p.y);
                    addPointtoBorder(nukePoints, p);
                }else{
                    struct PairPoints p = getCrossPoints(p1, p2, distance, radius);
                    printf("Point1:( %.2f : %.2f )^( %.2f : %.2f ) = ( %.2f : %.2f ) + ( %.2f : %.2f )\n",
                           p1.x, p1.y, p2.x, p2.y, p.p1.x, p.p1.y, p.p2.x, p.p2.y);
                    addPointtoBorder(nukePoints, p.p1);
                    addPointtoBorder(nukePoints, p.p2);
                }
            }
        }
    }
    size_t indx = 0;
    size_t maxpoints = 0;

    npoints = getNumPoints(nukePoints);
    for (i = 0; i < npoints; ++i){
        struct Point point = getPoint(nukePoints, i);
        size_t pointscount = 0;
        size_t nboardpoints = getNumPoints(board);
        for(j = 0; j < nboardpoints; ++j){
            struct Point circles = getPoint(board, j);
            if (inRadius(circles, radius, point)){
                ++pointscount;
            }
        }
        if (pointscount > maxpoints){
            indx = i;
            maxpoints = pointscount;
        }
    }
    printf("BestPoint: (%f : %f)\nDestroyed: %d\n", getPoint(nukePoints, indx).x,
           getPoint(nukePoints, indx).y, maxpoints);
    dbPrint(board);
    destroyBoard(board);
    destroyBoard(nukePoints);
    return 0;
}
bool inRadius(const struct Point circlePoint, const size_t radius, const struct Point p){
    if (pow((p.x - circlePoint.x), 2) + pow((p.y - circlePoint.y), 2) <= pow(radius, 2)){
        return true;
    }else{
        return false;
    }
}
struct Point getPoint(const struct Board *board, const size_t indx){
    if (indx < board->npoints){
        return board->ppoints[indx];
    }else{
        printf("[i] ERROR out of range\n");
        return board->ppoints[board->npoints - 1];
    }
}
size_t getNumPoints(const struct Board *board){
    return board->npoints;
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

    if (fp == NULL) {
        printf("Cannot open file [ %s ]\n", fname);
        return;
    }
    struct Point c;
    int tmpx, tmpy;
    while(fscanf(fp,"%d,%d", &tmpx, &tmpy) == 2){
        c.x = (double)tmpx;
        c.y = (double)tmpy;
        addPointtoBorder(board, c);
    }
    fclose(fp);
}
void dbPrint(const struct Board * const board){
    if (board == NULL)
        return;
    size_t npoints = board->npoints;
    int i = 0;
    printf("-----------------\nPoints: %d\n", npoints);
    for(i; i < npoints; ++i){
        printf("%d: (%.2f : %.2f) <--- %p\n",i, board->ppoints[i].x, board->ppoints[i].y, &board->ppoints[i]);
    }
    printf("-----------------\n");
}
double_t getDistance(const struct Point p1, const struct Point p2){ //if p1 == p2???
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

bool isCrossd(const double_t distance, const size_t r){
    if((distance > r + r) || (distance == 0)){
        return false;
    }else{
        return true;
    }
}
bool isCross(const struct Point p1, const struct Point p2, const size_t r){
    double_t distance = getDistance(p1, p2);
    if((distance > r + r) || (distance == 0)){
        return false;
    }else{
        return true;
    }
}
struct Point getCrossPoint(const struct Point p1, const struct Point p2,
                            const double_t distance, const size_t radius){
    struct Point p;
    p.x = (p2.x + p1.x) / 2.0;
    p.y = (p2.y + p1.y) / 2.0;
    return p;
}
struct PairPoints getCrossPoints(const struct Point p1, const struct Point p2,
                                 const double_t distance, const size_t radius){
    double_t h = sqrt(pow(radius, 2) - pow(distance, 2) / 4.0);
    h = (2.0 * h) / distance;   // p.x = x2 +/- h(y1-y0)/distance

    struct Point p1_cross, p2_cross, p_tmp;
    p_tmp.x = (p1.x + p2.x) / 2.0;
    p_tmp.y = (p1.y + p2.y) / 2.0;

    p1_cross.x = p_tmp.x + h * (p2.y - p_tmp.y);
    p1_cross.y = p_tmp.y - h * (p2.x - p_tmp.x);
    p2_cross.x = p_tmp.x - h * (p2.y - p_tmp.y);
    p2_cross.y = p_tmp.y + h * (p2.x - p_tmp.x);

    struct PairPoints pp;
    pp.p1 = p1_cross;
    pp.p2 = p2_cross;
    return pp;
}


