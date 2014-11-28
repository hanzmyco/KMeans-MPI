
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MPI.h"
#include "Slave.h"

void rev_split(int *begin, int *end){
	recv_int(begin,1,0);
	recv_int(end,1,0);
}


int distance(char * x1, char * x2,int col){
	int d=0;
	for(int i=0;i<col;i++){
		if(x1[i]!=x2[i])
			d++;	
	}
	return d;
}
void compute_labels(int *labels,char **data, int begin,int end, int k,char **center_point,int col){
	printf("%d %d\n",begin,end);



	for(int i=begin;i<=end;i++){
		char * x_ins=(char*)malloc((col+1)*sizeof(char));
		strcpy(x_ins,data[i]);
		x_ins[col]='\0';
		
		int min=0;
		
		for(int j=1;j<k;j++){
			if(distance(x_ins,center_point[j],col) <distance(x_ins,center_point[min],col)){
				min=j;
			}
			

		}
		labels[i]=min;
	}
}


