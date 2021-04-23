#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITER(x) (((x) > (200)) ? (200) : (x))
#define BUFFER_SIZE 200
#define INITIAL_ARRAY_SIZE 100
#define FLT_MAX 3.402823466e+38F

#define ASSERT(expr, message) \
    if (!(expr)){             \
         printf("%s", message); \
         exit(0);\
    } \


void kMeans(int k, char *filename, int max_iter);

int initializeArray(int arraySize, float **array, char currRow[], int d);

void updateCentroidsPerDatap(float ***datapoint, float ***centroid, int **datap_cluster_assignment, int d , int k, int size);

int main(int argc, char *argv[]) {
    /* Validating command-line inputs */
    int k, max_iter, i, d, arraySize;
    char firstRow[ BUFFER_SIZE ];

    float *datap_array, **datapoint, *centr_array, **centroid;
    int *datap_cluster_assignment;

    /* This line helps with printf buffering, since it can be junky sometimes */
    setvbuf (stdout, NULL, _IONBF, 0);

    ASSERT(argc > 0 , "Not enough arguments")

    if (argc == 1){
        max_iter = 200;
    }

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
    ASSERT(datapoint != NULL, "Error in memory allocation")
    datap_cluster_assignment = calloc(arraySize, sizeof(int));
    ASSERT(datap_cluster_assignment != NULL, "Error in memory allocation")
    for (i = 0; i < arraySize ; i++){
        datapoint[i] = datap_array + i*d;
        datap_cluster_assignment[i] = 0;
    }

    /* creating initial centroid */
    centr_array = (float *) malloc(k * d * sizeof(float ));
    ASSERT(centr_array != NULL, "Error in memory allocation")
    for (i = 0; i < k*d; i++){
        centr_array[i] = datap_array[i];
    }

    /*making centroid a 2D array*/
    centroid = (float **) calloc(k, sizeof(int*));
    ASSERT(centroid != NULL, "Error in memory allocation")
    for (i = 0; i < k ; i++){
        centroid[i] = centr_array + i * d;
    }

    /* Calculations */

    updateCentroidsPerDatap(&datapoint, &centroid, &datap_cluster_assignment, d, k, arraySize);



    /* Output & Free space */

    fflush(stdout);
    free(datap_array);
    free(datapoint);
    free(centr_array);
    free(centroid);
    free(datap_cluster_assignment);
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

void updateCentroidsPerDatap(float ***datapoint, float ***centroid, int **datap_cluster_assignment, int d, int k, int size){
    /* For each point i, we shall calculate euclidean distance from him to each cluster j, and find min, and assign to datap_cluster_assignment */
    int i, j, v, min_cluster;
    float dist, min_dist;
    for (i = 0 ; i < size ; i++){
        min_dist = FLT_MAX, min_cluster = -1;

        for (j = 0; j < k; j++ ){
            dist = 0;
            for (v = 0; v < d; v++){
                dist += ((*datapoint)[i][v] - (*centroid)[j][v])*((*datapoint)[i][v] - (*centroid)[j][v]);
            }

            if (min_dist > dist){
                min_dist = dist;
                min_cluster = j;
            }
        }
        (*datap_cluster_assignment)[i] = min_cluster;
    }
}