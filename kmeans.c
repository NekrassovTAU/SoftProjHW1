#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 200
#define INITIAL_ARRAY_SIZE 100
#define FLT_MAX 3.402823466e+38F

#define ASSERT(expr, message) \
    if (!(expr)){             \
         printf("%s", message); \
         exit(0);\
    } \


int initializeDatapointArray(int arraySize, double **datap_array, char *currRow, int d);

int
updateCentroidsPerDatap(double ***datapoint, double ***centroid, int **datap_cluster_assignment, int d, int k, int size,
                        double ***sumArrayHead, int **countArray);

void initializeRestOfArrays(double **datap_array, double **centr_array, double ***datapoint, double ***centroid, int **datap_cluster_assignment, int d , int k, int size, double **sumArray, double ***sumArrayHead, int **countArray);

void printFinalCentroids(double ***centroid, int d, int k);


int main(int argc, char *argv[]) {
    /* Validating command-line inputs */
    int k, max_iter, i, d, arraySize, update = 1;
    char firstRow[ BUFFER_SIZE ];

    double *datap_array, **datapoint, *centr_array, **centroid, *sumArray, **sumArrayHead;
    int *datap_cluster_assignment, *countArray;

    /* This line helps with printf buffering, since it can be junky sometimes */
    setvbuf (stdout, NULL, _IONBF, 0);

    ASSERT(argc > 1 , "Not enough arguments")

    k = atoi(argv[1]);
    max_iter = 200;

    if (argc > 2){
        max_iter = atoi(argv[2]);
    }




    ASSERT(max_iter > 0 , "max_iter input needs to be positive\n")
    ASSERT(k > 0 , "k input needs to be positive\n")

    /* Finding d based on first row - by number of commas + 1. */
    fgets(firstRow, BUFFER_SIZE, stdin);
    for (i=0, d=1; firstRow[i]; i++){
        d += (firstRow[i] == ',');
    }

    /*Setting up the datapoint array, and rest of arrays */
    arraySize = INITIAL_ARRAY_SIZE;

    datap_array = (double*) calloc(d * arraySize, sizeof (double));
    ASSERT(datap_array != NULL, "Error in memory allocation\n")

    arraySize = initializeDatapointArray(arraySize, &datap_array, firstRow, d);

    initializeRestOfArrays(&datap_array, &centr_array, &datapoint, &centroid, &datap_cluster_assignment, d, k, arraySize, &sumArray, &sumArrayHead, &countArray);

    /* Calculations */



    while(max_iter > 0 && update){
        update = updateCentroidsPerDatap(&datapoint, &centroid, &datap_cluster_assignment, d, k, arraySize, &sumArrayHead, &countArray);
        max_iter--;
    }

    /* print the output */
    printFinalCentroids(&centroid, d, k);

    /* Output & Free space */

    fflush(stdout);
    free(datap_array);
    free(datapoint);
    free(centr_array);
    free(centroid);
    free(datap_cluster_assignment);
    free(sumArrayHead);
    free(sumArray);
    free(countArray);
    return 0;
}


int initializeDatapointArray(int arraySize, double **datap_array, char *currRow, int d) {
    char *token;
    int tail = 0;

    do{/* as long as there are still lines to be read */
        /*loop to read line */
        token = strtok(currRow, ","); /* read the current line */
        while(token != NULL) {
            (*datap_array)[tail] = (double) atof(token);
            token = strtok(NULL, ",");
            tail++;

            /*in-case we reached the edge of the current datap_array, increase the arraySize by twice */
            if (tail == d * arraySize){
                arraySize *= 2;
                *datap_array = realloc(*datap_array, d * arraySize * sizeof(double));
                ASSERT(*datap_array != NULL, "Error in memory allocation\n")
            }
        }
    } while(fgets(currRow, BUFFER_SIZE, stdin));

    /* cut the datap_array to its intended arraySize */
    if(tail < d * arraySize - 1)  {
        *datap_array = realloc(*datap_array, tail * sizeof(double));
        ASSERT(*datap_array != NULL, "Error in memory allocation\n")
        arraySize = tail / d;
    }

    free(token);
    return arraySize;
}

void initializeRestOfArrays(double **datap_array, double **centr_array, double ***datapoint, double ***centroid, int **datap_cluster_assignment, int d , int k, int size, double **sumArray, double ***sumArrayHead, int **countArray){
    int i;

    /*making datapoint a 2D array and initializing datap_cluster_assignment*/
    *datapoint = calloc(size, sizeof(double*));
    ASSERT((*datapoint) != NULL, "Error in memory allocation\n")
    *datap_cluster_assignment = calloc(size, sizeof(int));
    ASSERT((*datap_cluster_assignment) != NULL, "Error in memory allocation\n")
    for (i = 0; i < size ; i++){
        (*datapoint)[i] = (*datap_array) + i*d;
        (*datap_cluster_assignment)[i] = 0;
    }

    /* creating initial centroid */
    *centr_array = malloc(k * d * sizeof(double));
    ASSERT((*centr_array) != NULL, "Error in memory allocation\n")
    for (i = 0; i < k*d; i++){
        (*centr_array)[i] = (*datap_array)[i];
    }

    /*making centroid a 2D array*/
    (*centroid) = calloc(k, sizeof(double*));
    ASSERT((*centroid) != NULL, "Error in memory allocation\n")
    for (i = 0; i < k ; i++){
        (*centroid)[i] = (*centr_array) + i * d;
    }

    /* initialization of the sum arrays */
    (*sumArray) = calloc(d * k, sizeof(double));
    ASSERT((*sumArray) != NULL, "Error in memory allocation\n")
    (*sumArrayHead) = calloc(k, sizeof(double*));
    ASSERT((*sumArrayHead) != NULL, "Error in memory allocation\n")
    for (i = 0; i < k ; i++) {
        (*sumArrayHead)[i] = (*sumArray) + i*d;
    }
    (*countArray) = calloc(k, sizeof(int));
    ASSERT((*countArray) != NULL, "Error in memory allocation\n")
}



/* i had a sign to the function, to check if any point switched any cluster (only if sign = 1 we will check)*/
int
updateCentroidsPerDatap(double ***datapoint, double ***centroid, int **datap_cluster_assignment, int d, int k, int size,
                        double ***sumArrayHead, int **countArray) {
    /* For each point i, we shall calculate euclidean distance from him to each cluster j, and find min, and assign to datap_cluster_assignment */
    int i, j, v, min_cluster, update, currCluster;
    double dist, min_dist, new_value;
    
    
    update = 0;
    
    
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
        if((*datap_cluster_assignment)[i] != min_cluster){ /* there is a change in one or more of the data points cluster assignment */
            update = 1;
        }
        (*datap_cluster_assignment)[i] = min_cluster;
    }

    /* loop to initialize sum/counter*/

    for(i = 0; i < size; i++){ /*count and sum up all the sizes*/
        currCluster = (*datap_cluster_assignment)[i];
        (*countArray)[currCluster]++;
        for(v = 0; v < d; v++){
            (*sumArrayHead)[currCluster][v] += (*datapoint)[i][v];
        }
    }
    

    /*update the new clusters and initialize to 0*/
    for(j = 0; j < k; j++) { /* each loop for different cluster*/
        for (v = 0; v < d; v++) { /* each loop for opponent of the current cluster*/
            new_value = (*sumArrayHead)[j][v] / (*countArray)[j];
            (*centroid)[j][v] = new_value;
            (*sumArrayHead)[j][v] = 0;
        }
        (*countArray)[j] = 0;
    }

    return update;
}

void printFinalCentroids(double ***centroid, int d, int k){
    int i, j;
    for(i = 0; i < k; i++){
        for(j = 0; j < d; j++){
            printf("%.4f", (*centroid)[i][j]);
            if (j != d - 1){   /* not last component of the cluster*/
                printf("%s", ",");
            }
            else{
                printf("\n");
            }
        }
    }
}