#include "MPI.h"
#include <mpi.h>
void send_char(char ** x,int len,int process_id,int col){
	int msg_tag=0;
	for(int i=0;i<len;i++){
		MPI_Send(x[i],col,MPI_CHAR,process_id,msg_tag,MPI_COMM_WORLD);
	}

	
}
void recv_char(char ** x,int len,int process_id,int col){
	MPI_Status status;
	int msg_tag=0;
	for(int i=0;i<len;i++){
		MPI_Recv(x[i],col,MPI_CHAR,process_id,msg_tag,MPI_COMM_WORLD,&status);

	}
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















