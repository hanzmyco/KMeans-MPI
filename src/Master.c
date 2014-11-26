#include "Master.h"

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


		datax[i]=atof(x1[i]);
		datay[i]=atof(y1[i]);
		
		i++;
	}
	
	fclose(fp);
	for(int i=0;i<number;i++){
		free(x1[i]);
		free(y1[i]);
	}
	free (x1);
	free (y1);

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




