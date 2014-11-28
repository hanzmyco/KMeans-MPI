#include "MPI.h"
#include <mpi.h>
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















