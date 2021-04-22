#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_ITER(x) = (min((x),(200))
#define BUFFER_SIZE = 200

#define ASSERT(expr, message) \
    if (!(expr)){             \
         printf("%s", message); \
         exit(0);\
    } \


void kMeans(int k, char *filename, int max_iter);

int main(int argc, char *argv[]) {
    /* Validating command-line inputs */
    int k, max_iter, i, d;
    char* firstRow;

    ASSERT(argc <= 1 , "Not enough arguments");

    k = atoi(argv[0]);
    max_iter = atoi(argv[1]);

    ASSERT(max_iter > 0 , "max_iter input needs to be positive");
    ASSERT(k > 0 , "k input needs to be positive");


    /* Allocating all needed space for computations - and reallocating if needed */

    /* Finding d based on first row - by number of commas + 1. */
    firstRow = fgets(BUFFER_SIZE)
    for (i=0, d=1; firstRow[i]; i++){
        d += (firstRow[i] == ',');
    }




    /* Calculations */
    /* Output & Free space */
    return 0;
}



void kMeans(int k, char *filename, int max_iter) {
   /* int lines, d = 1;
    char c;
    FILE *in_file = fopen(filename, "r");

    for (c = getc(in_file); c != EOF; c = getc(in_file)) {
        if (c == ',' && lines == 0) {  // count , (d)
            d += 1;
        } else {
            if (c == '\n') { // count \n
                lines += 1; // is there addition in the last line as well? might start lines from 1
            }
        }

        //create an array of pointers of size LINES with pointer from each cell into array of type float of size (d+1) * size of (float)

        return 0;
    }*/
}