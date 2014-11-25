
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
	 value[0]=rand()%number+1;
	 for(int i=0;i<k;i++){
	 	value[i]=rand()%number+1;
	 	for(int j=0;j<i;j++){
	 		if(value[i]==value[j]){
	 			i--;
	 			break;
	 		}
	 	}
	 }
}

void split_data(int process_num,int number){
	int each_p=number/process_num;
	int begin=0;
	int end=begin+each_p-1;
	int i;
	for(i=1;i<process_num;i++){
		send_int(begin,1,i);
		send_int(end,1,i);
		begin+=each_p;
		end+=each_p;


	}
	send_int(begin+each_p,1,process_num);
	send_int(number-1,1,process_num);



}

void rev_split(int *begin, int *end){
	recv_int(begin,1,0);
	recv_int(end,1,0);
}






// argv: how many data points and how many clusters


int main(int argc, char *argv[])
{
	int numprocs, rank, namelen;
  	char processor_name[MPI_MAX_PROCESSOR_NAME];

  	MPI_Init(&argc, &argv);
  	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Get_processor_name(processor_name, &namelen);

    double *x;
  	double *y;

  	int *begin=(int*)malloc(sizeof(int));
  	int *end=(int*)malloc(sizeof(int));

  	int * center;
  	
  	int number=atoi(argv[1]);
  	int k=atoi(argv[2]);
  	// number of process going to run include process 0
  	int num_p=atoi(argv[3]);

  	center=(int*)malloc(k*sizeof(int));


  	printf("Process %d on %s out of %d\n", rank, processor_name, numprocs);

  	if(rank==0){
  		
  		// read in the data and generate numeric data
  		char * in="input.txt";
		x=(double *)malloc(number*sizeof(double));
		y=(double *)malloc(number*sizeof(double));
		readin(in,x,y,number);

		// randomise the center at first
	
		random_center(number,k,center);



		// distribute the data and random center
		for(int i=1;i<num_p;i++){
			send_double(x,number,i);
			send_double(y,number,i);
			//send_int(center,k,i);


		}


		//split_data(num_p-1,number);

		

		free (x);
		free (y);


  	}
  	else{
  		x=(double *)malloc(sizeof(double));

  		recv_double(x,number,0);
  		printf("%10.15f\n",x[0]);
  		recv_double(y,number,0);
  		printf("%10.15f\n",x[0]);
  		//recv_int(center,k,0);
  		//rev_split(begin,end);


  		

  	}













	MPI_Finalize();
	
	
	
}





	