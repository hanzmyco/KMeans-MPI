
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "MPI.h"

void rev_split(int *begin, int *end){
	recv_int(begin,1,0);
	recv_int(end,1,0);
}


double distance(double x1, double y1, double x2, double y2){
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
void compute_labels(int *labels,double *x, double *y,int begin,int end, int k,double * center_x,double * center_y){
	printf("%d %d\n",begin,end);



	for(int i=begin;i<=end;i++){

		double x_ins=x[i];
		double y_ins=y[i];
		int min=0;
		
		for(int j=1;j<k;j++){
			if(distance(x_ins,y_ins,center_x[j],center_y[j]) <distance(x_ins,y_ins,center_x[min],center_y[min])){
				min=j;
			}
			

		}
		labels[i]=min;
	}
}


