
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include "MPI.h"
#include "Master.h"
#include "Slave.h"


// argv: how many data points and how many clusters
int main(int argc, char *argv[])
{
	int numprocs, rank, namelen;
  	char processor_name[MPI_MAX_PROCESSOR_NAME];
  	int iterate_time;

  	MPI_Init(&argc, &argv);
  	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Get_processor_name(processor_name, &namelen);


  	// data
    double *x;
  	double *y;

  	// which class do they belong
  	int * labels;

  	int * center;
  	
  	int number=atoi(argv[1]);
  	int k=atoi(argv[2]);
  	// number of process going to run include process 0
  	int num_p=atoi(argv[3]);
  	iterate_time=atoi(argv[4]);
  	char * in=argv[5];

  	
  	center=(int*)malloc(k*sizeof(int));

  	double *center_x=(double *)malloc(k*sizeof(double));
  	double *center_y=(double *)malloc(k*sizeof(double));
  	


  	x=(double *)malloc(number*sizeof(double));
	y=(double *)malloc(number*sizeof(double));
	labels=(int *)malloc(number*sizeof(int));

	//split data index
	int **begin=(int**)malloc((num_p-1)*sizeof(int*));
  	int **end=(int**)malloc((num_p-1)*sizeof(int*));
  	for(int i=0;i<num_p-1;i++){
  		begin[i]=(int*)malloc(sizeof(int));
  		end[i]=(int*)malloc(sizeof(int));
  			
  	}





  	printf("Process %d on %s out of %d\n", rank, processor_name, numprocs);

  	if(rank==0){
  		
  		readin(in,x,y,number);

		// randomise the center at first
		random_center(number,k,center);

	

		//set  the center at the first time
		for(int i=0;i<k;i++){
			center_x[i]=x[center[i]];
			center_y[i]=y[center[i]];
		}


		// distribute the data
		for(int i=1;i<num_p;i++){
			send_double(x,number,i);
			send_double(y,number,i);
			send_int(labels,number,i);
			send_double(center_x,k,i);
			send_double(center_y,k,i);
			
			
		}
		split_data(num_p-1,number,begin,end);
					
		// get labels
		for(int index=0;index<iterate_time;index++){
			
			get_laybels(labels,num_p-1,number,begin, end);

			compute_center(number,k,x,y,labels,center_x,center_y);	


			for(int i=0;i<k;i++){
			printf("%10.15f, %10.15f\n",center_x[i],center_y[i]);
  			
		}

			
			




			for(int i=1;i<num_p;i++){
				send_double(center_x,k,i);
				send_double(center_y,k,i);

				}
		}
		
		

		free (x);
		free (y);
		free(labels);


  	}
  	else{
  		

  		recv_double(x,number,0);
  		recv_double(y,number,0);
  		recv_int(labels,number,0);
  		recv_double(center_x,k,0);
  		recv_double(center_y,k,0);


  		
  		int *begin1=(int*)malloc(sizeof(int));
  		int *end1=(int*)malloc(sizeof(int));
  		rev_split(begin1,end1);

  		

  		

  		for(int index=0;index<iterate_time;index++){

  			compute_labels(labels,x,y,begin1[0],end1[0], k,center_x,center_y);
  				
  			send_int(labels+begin1[0],end1[0]-begin1[0]+1,0);

  			recv_double(center_x,k,0);
  			recv_double(center_y,k,0);

  			/*
  			for(int i=0;i<k;i++){
			printf("%d class: ",i);
			printf("%10.15f ",center_x[i]);
  			printf("%10.15f\n",center_y[i]);
			}
				printf("finishied\n");
			*/
  		}
  			





  		

  	}

	MPI_Finalize();
	
	
	
}





	