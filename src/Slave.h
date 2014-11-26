
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include "MPI.h"

void rev_split(int *begin, int *end);


double distance(double x1, double y1, double x2, double y2);

void compute_labels(int *labels,double *x, double *y,int begin,int end, int k,double * center_x,double * center_y);

