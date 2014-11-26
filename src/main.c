
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
void readin(char * input,double * datax,double * datay,int number)
{

	FILE *fp;
	char *line=NULL;
	size_t len=0;
	size_t read;
	fp=fopen(input,"r");

	char ** x1=(char **)malloc(number * sizeof(char *));
	char ** y1=(char**)malloc(number * sizeof(char*));
	for(int i=0;i<number;i++){
		x1[i]=(char*)malloc(25*sizeof(char));
		y1[i]=(char*)malloc(25*sizeof(char));
	}
	
	if (fp==NULL)
		exit(EXIT_FAILURE);
	int i=0;
	while((read=getline(& line,&len,fp))!=-1 && i <number){
		int index=0;
		for(index=0;line[index]!=',';index++){
			x1[i][index]=line[index];
		}
		
		int j;
		
		for(j=index+1;j<strlen(line)-1;j++){
					y1[i][j-index-1]=line[j];
		}
		//x1[i][index]='\0';
		//y1[j-index-1]='\0';

		datax[i]=atof(x1[i]);
		datay[i]=atof(y1[i]);
		
		i++;
	}
	
	/*
	for (int i=0;i<number;i++){
		printf("%10.15f",datax[i]);
		printf("\n");
		printf("%10.15f",datay[i]);
		printf("\n");
	}
	*/

	fclose(fp);
	for(int i=0;i<number;i++){
		free(x1[i]);
		free(y1[i]);
	}
	free (x1);
	free (y1);

}
void send_double(double * x,int len,int process_id){
	int msg_tag=0;
	MPI_Send(x,len,MPI_DOUBLE,process_id,msg_tag,MPI_COMM_WORLD);
}
void recv_double(double * x,int len,int process_id){
	MPI_Status status;
	int msg_tag=0;
	MPI_Recv(x,len,MPI_DOUBLE,process_id,msg_tag,MPI_COMM_WORLD,&status);
}

void send_int(int * x,int len,int process_id){
	int msg_tag=0;
	MPI_Send(x,len,MPI_INT,process_id,msg_tag,MPI_COMM_WORLD);
}

void recv_int(int * x,int len,int process_id){
	MPI_Status status;
	int msg_tag=0;
	MPI_Recv(x,len,MPI_INT,process_id,msg_tag,MPI_COMM_WORLD,&status);
}



void random_center(int number,int k,int * value){
	 srand( (unsigned)time( NULL ) ); 
	 //value=(int*)malloc(k*sizeof(int));
	 value[0]=rand()%number;
	 for(int i=0;i<k;i++){
	 	value[i]=rand()%number;
	 	for(int j=0;j<i;j++){
	 		if(value[i]==value[j]){
	 			i--;
	 			
	 		}
	 	}
	 }
}

void split_data(int process_num,int number,int **begin, int **end){
	int each_p=number/process_num;
	int b=0;
	int e=b+each_p-1;
	int i;
	for(i=1;i<process_num;i++){
		begin[i-1][0]=b;
		end[i-1][0]=e;

		send_int(begin[i-1],1,i);
		send_int(end[i-1],1,i);
		b+=each_p;
		e+=each_p;


	}
	begin[process_num-1][0]=b;
	end[process_num-1][0]=number-1;
	send_int(begin[process_num-1],1,process_num);
	send_int(end[process_num-1],1,process_num);



}

void rev_split(int *begin, int *end){
	recv_int(begin,1,0);
	recv_int(end,1,0);
}

double distance(double x1, double y1, double x2, double y2){
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

void compute_center(int number,int k,double*x,double *y,int *labels,double *center_x,double *center_y){
	double * x_sum=(double *)malloc(k*sizeof(double));
	double * y_sum=(double *)malloc(k*sizeof(double));
	
	int * class_num=(int *)malloc(k*sizeof(int));

	for(int i=0;i<number;i++){
		class_num[labels[i]]=0;
		x_sum[labels[i]]=0;
		y_sum[labels[i]]=0;
	}	


	for(int i=0;i<number;i++){
		class_num[labels[i]]++;
		x_sum[labels[i]]+=x[i];
		y_sum[labels[i]]+=y[i];
	}
	for( int i=0;i<k;i++){
		center_x[i]=x_sum[i]/(double)class_num[i];
		center_y[i]=y_sum[i]/(double)class_num[i];

		/*
		printf("%d compute class: ",i);
		printf("%10.15f ",center_x[i]);
  		printf("%10.15f\n",center_y[i]);
		*/

	}
}

void get_laybels(int * laybels, int process_num,int number,int **begin, int **end){
	int offset=0;
	for(int i=1;i<=process_num;i++){


		recv_int(laybels+offset,end[i-1][0]-begin[i-1][0]+1,i);
		offset+=end[i-1][0]-begin[i-1][0]+1;

	}

}

void compute_labels(int *labels,double *x, double *y,int begin,int end, int k,double * center_x,double * center_y){
	printf("%d %d\n",begin,end);



	for(int i=begin;i<=end;i++){

		double x_ins=x[i];
		double y_ins=y[i];
		int min=0;


		printf("%f %f\n",x_ins,y_ins);

		
		for(int j=1;j<k;j++){
			if(distance(x_ins,y_ins,center_x[j],center_y[j]) <distance(x_ins,y_ins,center_x[min],center_y[min])){
				min=j;
			}
			printf("min is now %d\n",min);

		}
		labels[i]=min;
	}
}


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
  		
  		// read in the data and generate numeric data
  		//char * in="input.txt";
		
		readin(in,x,y,number);

		// randomise the center at first

		//random_center(number,k,center);

		center[0]=4;
		center[1]=5;

		//set  the center at the first time
		for(int i=0;i<k;i++){
			
			//printf("random center\n");
			//printf("%d\n",center[i]);

			center_x[i]=x[center[i]];
			center_y[i]=y[center[i]];
			printf("%10.15f, %10.15f\n",center_x[i],center_y[i]);
  			
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



			for(int i=0;i<number;i++){

				printf("point %d %d\n",i,labels[i]);
			}


  			
  		

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





	