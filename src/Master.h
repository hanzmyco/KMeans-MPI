
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "MPI.h"
void readin(char * input,double * datax,double * datay,int number);

void random_center(int number,int k,int * value);

void split_data(int process_num,int number,int **begin, int **end);

void compute_center(int number,int k,double*x,double *y,int *labels,double *center_x,double *center_y);

void get_laybels(int * laybels, int process_num,int number,int **begin, int **end);




