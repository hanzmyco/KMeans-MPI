#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "Master.h"
#include "MPI.h"
#include <mpi.h>



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
    char **data;

    // which class do they belong
    int * labels;

    int * center;
    
  	int number=atoi(argv[1]);
  	int k=atoi(argv[2]);
  	// number of process going to run include process 0
  	int num_p=atoi(argv[3]);
  	iterate_time=atoi(argv[4]);
  	char * in=argv[5];
    int col=atoi(argv[6]);	

    center=(int*)malloc(k*sizeof(int));

    char **center_point=(char**)malloc(k*sizeof(char*));
    for(int i=0;i<k;i++){
      center_point[i]=(char *)malloc((col+1)*sizeof(char));
    }


    
    data=(char **)malloc(number*sizeof(char *));
    labels=(int *)malloc(number*sizeof(int));
    
    for(int i=0;i<number;i++){
      data[i]=(char *)malloc((col+1)*sizeof(char));
    }

    //split data index
    int **begin=(int**)malloc((num_p-1)*sizeof(int*));
    int **end=(int**)malloc((num_p-1)*sizeof(int*));
    for(int i=0;i<num_p-1;i++){
      begin[i]=(int*)malloc(sizeof(int));
      end[i]=(int*)malloc(sizeof(int));
        
    }



    printf("Process %d on %s out of %d\n", rank, processor_name, numprocs);

    if(rank==0){
      readin(in,data,number,col);

    for(int i=0;i<number;i++){
        puts(data[i]);
    }



      random_center(number,k,center);

      printf("random benter\n");
    //set  the center for the first time
    for(int i=0;i<k;i++){
      strcpy(center_point[i],data[center[i]]);

      puts(center_point[i]);
    }

    // distribute the data
    for(int i=1;i<num_p;i++){
      send_char(data,number,i,col);
      send_int(labels,number,i);
      send_char(center_point,k,i,col);
    }

    split_data(num_p-1,number,begin,end);

    
    // get labels
    for(int index=0;index<iterate_time;index++){
      
      get_laybels(labels,num_p-1,number,begin, end);


      
      compute_center(number,k,data,labels,center_point,col);  

      



      
      for(int i=1;i<num_p;i++){
        send_char(center_point,k,i,col);
        
      }
      for(int i=0;i<k;i++){
        printf("%d class\n: ",i);
        puts(center_point[i]);
      }
      printf("finishied\n");
    }
    


	}
  //slave process
  else{
    recv_char(data,number,0,col);
    recv_int(labels,number,0);
    recv_char(center_point,k,0,col);
    
    for(int i=0;i<number;i++){
      data[i][col]='\0';
      
      puts(data[i]);
    }

    int *begin1=(int*)malloc(sizeof(int));
    int *end1=(int*)malloc(sizeof(int));
    rev_split(begin1,end1);

    
    for(int index=0;index<iterate_time;index++){

        compute_labels(labels,data,begin1[0],end1[0], k,center_point,col);

        /*
      for(int i=0;i<number;i++){
        printf("%d\n",labels[i]);
      }*/
         
        int *ite=(int * )malloc(sizeof(int));
        *ite=index;
        send_int(ite,1,0);
          
        send_int(labels+begin1[0],end1[0]-begin1[0]+1,0);



        recv_char(center_point,k,0,col); 
    }
  }



  MPI_Finalize();
}
	
	






	