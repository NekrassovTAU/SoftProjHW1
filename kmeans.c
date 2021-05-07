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


/* void kMeans(int k, char *filename, int max_iter); */

int initializeDatapointArray(int arraySize, double **datap_array, char *currRow, int d);

int updateCentroidsPerDatap(double ***datapoint, double ***centroid, int **datap_cluster_assignment, int d , int k, int size);

void initializeRestOfArrays(double **datap_array, double **centr_array, double ***datapoint, double ***centroid, int **datap_cluster_assignment, int d , int k, int size);

void printFinalCentroids(double ***centroid, int d, int k);


int main(int argc, char *argv[]) {
    /* Validating command-line inputs */
    int k, max_iter, i, d, arraySize, update = 1;
    char firstRow[ BUFFER_SIZE ];

    double *datap_array, **datapoint, *centr_array, **centroid;
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

    /*Setting up the datapoint array, and rest of arrays */
    arraySize = INITIAL_ARRAY_SIZE;

    datap_array = (double*) calloc(d * arraySize, sizeof (double));
    ASSERT(datap_array != NULL, "Error in memory allocation")

    arraySize = initializeDatapointArray(arraySize, &datap_array, firstRow, d);

    initializeRestOfArrays(&datap_array, &centr_array, &datapoint, &centroid, &datap_cluster_assignment, d, k, arraySize);

    /* Calculations */

    while(max_iter > 0 && update){
        update = updateCentroidsPerDatap(&datapoint, &centroid, &datap_cluster_assignment, d, k, arraySize);
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
   return 0;
}


/*
void kMeans(int k, char *filename, int max_iter) {
    k = max_iter;
    filename[0] = 'l';
    max_iter = k+2;
    int lines, d = 1;
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

        //create an array of pointers of size LINES with pointer from each cell into array of type double of size (d+1) * size of (double)

        return 0;
    }
} */


// 1. (line 108) why datap_array is of type ** ? i believe it was of type * in the original function.
// 1.1 now go back to 116 and figure it out.


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
                ASSERT(*datap_array != NULL, "Error in memory allocation")
            }
        }
    } while(fgets(currRow, BUFFER_SIZE, stdin));

    /* cut the datap_array to its intended arraySize */
    if(tail < d * arraySize - 1)  {
        *datap_array = realloc(*datap_array, tail * sizeof(double));
        ASSERT(*datap_array != NULL, "Error in memory allocation")
        arraySize = tail / d;
    }

    free(token);
    return arraySize;
}

void initializeRestOfArrays(double **datap_array, double **centr_array, double ***datapoint, double ***centroid, int **datap_cluster_assignment, int d , int k, int size){
    int i;

    /*making datapoint a 2D array and initializing datap_cluster_assignment*/
    *datapoint = calloc(size, sizeof(int*));
    ASSERT((*datapoint) != NULL, "Error in memory allocation")
    *datap_cluster_assignment = calloc(size, sizeof(int));
    ASSERT((*datap_cluster_assignment) != NULL, "Error in memory allocation")
    for (i = 0; i < size ; i++){
        (*datapoint)[i] = (*datap_array) + i*d;
        (*datap_cluster_assignment)[i] = 0;
    }

    /* creating initial centroid */
    *centr_array = (double *) malloc(k * d * sizeof(double ));
    ASSERT((*centr_array) != NULL, "Error in memory allocation")
    for (i = 0; i < k*d; i++){
        (*centr_array)[i] = (*datap_array)[i];
    }

    /*making centroid a 2D array*/
    (*centroid) = (double **) calloc(k, sizeof(int*));
    ASSERT((*centroid) != NULL, "Error in memory allocation")
    for (i = 0; i < k ; i++){
        (*centroid)[i] = (*centr_array) + i * d;
    }
}
/* i had a sign to the function, to check if any point switched any cluster (only if sign = 1 we will check)*/
int updateCentroidsPerDatap(double ***datapoint, double ***centroid, int **datap_cluster_assignment, int d, int k, int size){
    /* For each point i, we shall calculate euclidean distance from him to each cluster j, and find min, and assign to datap_cluster_assignment */
    int i, j, v, min_cluster, update;
    double dist, min_dist, new_value, counter;
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
        if( datap_cluster_assignment)[i] != min_cluster){ /* there is a change in one or more of the data points cluster assignment */
            update = 1;
        }
        (*datap_cluster_assignment)[i] = min_cluster;
    }

    /*
     * Alternative Way with complexity of (size + k*d), instead of size*k*d.
     *
     *  // initialization of the arrays. lets say it happens in the function you build earlier with proper changes of **
    *sumArray = calloc(d * k, sizeof(double));
    ASSERT((*sumArray) != NULL, "Error in memory allocation")
    *sumArrayHead = calloc(k, sizeof(double*));
    ASSERT((*sumArrayHead) != NULL, "Error in memory allocation")
    for (i = 0; i < k ; i++) {
        (*sumArrayHead)[i] = (*sumArray) + i*d;
    }
    *counterArray = calloc(k, sizeof(int));
    ASSERT((*counterArray) != NULL, "Error in memory allocation")

    // loop to initialize sum/counter

    for(i = 0; i < size; i++){ //count and sum up all the sizes
        currCluster = (*datap_cluster_assignment)[i];
        (*counterArray)[currCluster]++;
        for(v = 0; v < d; v++){
            (**sumArrayHead)[currCluster][v] += (*datapoint)[i][v];
        }
    }
    // update the new clusters and initialize to 0
    for(j = 0; j < k; k++) { // each loop for different cluster
        for (v = 0; v < d; v++) { // each loop for opponent of the current cluster
            new_value = (**sumArrayHead)[k][v] / (*counterArray)[k];
            (*centroid)[j][v] = new_value;
            (**sumArrayHead)[k][v] = 0;
        }
        (*counterArray)[k] = 0
    }
    */

    for(j = 0; j < k; k++) { // each loop for different cluster
        for (v = 0; v < d; v++) { // each loop for opponent of the current cluster
            new_value = 0, counter = 0;
            for (i = 0; i < size; i++) {
                if(*datap_cluster_assignment[i] == j){
                    counter++;
                    new_value += (*datapoint)[i][v];
                }
            }
            if (counter != 0) { // if its allowed to divide 0/0 so there is no need of this if statement.
                new_value /= counter;
            }
            (*centroid)[j][v] = new_value;

           /* if(new_value != (*centroid)[j][v]){
                update = 1; // we did change. question: what if there is no change in any value, but some how we switched point's clusters - is that possible?
                (*centroid)[j][v] = new_value;
            }*/
        }
    }
    return update;
   /*
    if(! update){ // print and finish - here, or just exit and print in the main by using another function.
        printf("The final clusters are :"); // print the clusters by the rules in the PDF
        exit(0);
    } else{
        //send recursive call or loop over in main with the current function
    }
    */
}

void printFinalCentroids(double ***centroid, int d, int k){
    int i, j;
    for(i = 0; i < k; i++){
        for(j = 0; j < d; j++){
            printf("%.4f", (*centroid)[k][j]);
            if (j != d - 1){   // not last component of the cluster
                printf("%s", ",");
            }
            else{
                printf("\n");
            }
        }
    }
}