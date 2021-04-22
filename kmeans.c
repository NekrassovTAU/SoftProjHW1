#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITER(x) (((x) > (200)) ? (200) : (x))
#define BUFFER_SIZE 200

#define ASSERT(expr, message) \
    if (!(expr)){             \
         printf("%s", message); \
         exit(0);\
    } \


void kMeans(int k, char *filename, int max_iter);

int main(int argc, char *argv[]) {
    /* Validating command-line inputs */
    int k, max_iter, i, d, size, tail;
    char firstRow[ BUFFER_SIZE ];
    char *token;
    float *array;

    ASSERT(argc > 1 , "Not enough arguments")

    k = atoi(argv[1]);
    max_iter = atoi(argv[2]);
    max_iter = MAX_ITER(max_iter);

    ASSERT(max_iter > 0 , "max_iter input needs to be positive\n")
    ASSERT(k > 0 , "k input needs to be positive\n")

    /* Finding d based on first row - by number of commas + 1. */
    fgets(firstRow, BUFFER_SIZE, stdin);
    for (i=0, d=1; firstRow[i]; i++){
        d += (firstRow[i] == ',');
    }
    size = 100, tail = 0;

    array = (float*) calloc(d * size, sizeof (float));

    do{/* as long as there are still lines to be read */
       /*loop to read line */
       token = strtok(firstRow, ","); /* read the current line */
       while(token != NULL) {
           array[tail] = (float) atof(token);
           token = strtok(NULL, ",");
           tail++;

           /*in-case we reached the edge of the current array, increase the size by twice */
           if (tail == d * size){
               size *= 2;
               array = realloc(array, d * size * sizeof(float));
               ASSERT(array != NULL, "Error in memory allocation")
           }
       }
    } while(fgets(firstRow, BUFFER_SIZE, stdin));

    /* cut the array to its intended size */
    if(tail < d * size - 1)  {
        array = realloc(array, tail * sizeof(float));
        ASSERT(array != NULL, "Error in memory allocation")
    }


    /* Calculations */
    /* Output & Free space */
   return 0;
}



void kMeans(int k, char *filename, int max_iter) {
    k = max_iter;
    filename[0] = 'l';
    max_iter = k+2;
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