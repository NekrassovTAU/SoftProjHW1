#include <stdio.h>

int kMeans(int k, char *filename, int max_iter);

int main() {
    printf("Hello, World!\n");
    return 0;
}



int kMeans(int k, char *filename, int max_iter) {
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

        //create an array of pointers of size LINES with pointer from each cell into array of type float of size (d+1) * size of (float)

        return 0;
    }
}