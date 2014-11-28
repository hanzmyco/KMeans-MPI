#include "Master.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MPI.h"


void readin(char * input,char ** data,int number,int col)
{

	FILE *fp;
	char *line=NULL;
	size_t len=0;
	size_t read;
	fp=fopen(input,"r");

	
	if (fp==NULL)
		exit(EXIT_FAILURE);
	int i=0;
	while((read=getline(& line,&len,fp))!=-1 && i <number){
		
		strncpy(data[i],line,col);
		data[i][col]='\0';
		i++;
	}
	
	fclose(fp);
}

void random_center(int number,int k,int * value){
	 srand( (unsigned)time( NULL ) ); 
	 
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
// ACGT
void compute_center(int number,int k,char **x,int *labels,char **center_point,int col){
	
	int *** x_sum=(int ***)malloc(k*sizeof(int**));
	for(int i=0;i<k;i++){
		x_sum[i]=(int**)malloc(col*sizeof(int*));
		for(int j=0;j<col;j++){
			x_sum[i][j]=(int*)malloc(4*sizeof(int));
			for(int m=0;m<4;m++){
				x_sum[i][j][m]=0;
			}
		}
	}

	
	for(int i=0;i<number;i++){
		for(int j=0;j<col;j++){

	

			switch(x[i][j]){
				case 'A':
					x_sum[labels[i]][j][0]++;
					break;
				case 'C':
					x_sum[labels[i]][j][1]++;
					break;
				case 'G':
					x_sum[labels[i]][j][2]++;
					break;
				case 'T':
					x_sum[labels[i]][j][3]++;
					break;

			}

	
		}
	}



	for(int i=0;i<k;i++){
		for(int j=0;j<col;j++){
			int max=0;
			for(int m=1;m<4;m++){
				if(x_sum[labels[i]][j][m]>x_sum[labels[i]][j][max])
					max=m;
			}
			switch(max){
				case 0:
					center_point[i][j]='A';
					break;
				case 1:
					center_point[i][j]='C';
					break;
				case 2:
					center_point[i][j]='G';
					break;
				case 3:
					center_point[i][j]='T';
					break;

			}
		}
	}
		/*
		printf("%d compute class: ",i);
		printf("%10.15f ",center_x[i]);
  		printf("%10.15f\n",center_y[i]);
		*/

	
}

void get_laybels(int * laybels, int process_num,int number,int **begin, int **end){
	int offset=0;

	int * ite=(int *)malloc(sizeof(int));

	
	for(int i=1;i<=process_num;i++){

		recv_int(ite,1,i);
		printf("get ite: %d\n",*ite);
	



		recv_int(laybels+offset,end[i-1][0]-begin[i-1][0]+1,i);
		offset+=end[i-1][0]-begin[i-1][0]+1;

	}

}




