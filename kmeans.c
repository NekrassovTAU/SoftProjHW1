#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITER(x) (((x) > (200)) ? (200) : (x))
#define BUFFER_SIZE 200
#define INITIAL_ARRAY_SIZE 100

#define ASSERT(expr, message) \
    if (!(expr)){             \
         printf("%s", message); \
         exit(0);\
    } \


void kMeans(int k, char *filename, int max_iter);

int initializeArray(int arraySize, float **array, char currRow[], int d);

int main(int argc, char *argv[]) {
    /* Validating command-line inputs */
    int k, max_iter, i, d, arraySize;
    char firstRow[ BUFFER_SIZE ];

    float *datap_array, **datapoint, *centr_array, **centroids;
    int *datap_cluster_assignment;

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

    /*Setting up the datapoint array */
    arraySize = INITIAL_ARRAY_SIZE;

    datap_array = (float*) calloc(d * arraySize, sizeof (float));
    ASSERT(datap_array != NULL, "Error in memory allocation")

    arraySize = initializeArray(arraySize, &datap_array, firstRow, d);

    /*making datapoint a 2D array and initializing datap_cluster_assignment*/
    datapoint = calloc(arraySize, sizeof(int*));
    datap_cluster_assignment = calloc(arraySize, sizeof(int));
    for (i = 0; i < arraySize * d ; i++){
        if (i % d == 0){
            datapoint[i/d] = datap_array + i;
        }
        datap_cluster_assignment[i] = 0;
    }

    /* creating initial centroids */
    centr_array = calloc(k, d * sizeof(float ));
    ASSERT(centr_array != NULL, "Error in memory allocation")

    for (i = 0; i < k*d; i++){
        centr_array[i] = datap_array[i];
    }

    /*making centroids a 2D array*/
    centroids = calloc(k, sizeof(int*));
    for (i = 0; i < k ; i++){
        centroids[i] = centr_array + i * d;
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

int initializeArray(int arraySize, float **array, char currRow[], int d) {
    char *token;
    int tail = 0;

    do{/* as long as there are still lines to be read */
        /*loop to read line */
        token = strtok(currRow, ","); /* read the current line */
        while(token != NULL) {
            (*array)[tail] = (float) atof(token);
            token = strtok(NULL, ",");
            tail++;

            /*in-case we reached the edge of the current datap_array, increase the arraySize by twice */
            if (tail == d * arraySize){
                arraySize *= 2;
                *array = realloc(*array, d * arraySize * sizeof(float));
                ASSERT(*array != NULL, "Error in memory allocation")
            }
        }
    } while(fgets(currRow, BUFFER_SIZE, stdin));

    /* cut the datap_array to its intended arraySize */
    if(tail < d * arraySize - 1)  {
        *array = realloc(*array, tail * sizeof(float));
        ASSERT(*array != NULL, "Error in memory allocation")
        arraySize = tail / d;
    }

    return arraySize;
}