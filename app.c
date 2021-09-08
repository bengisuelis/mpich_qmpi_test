#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef _EXTERN_C_
#ifdef __cplusplus
#define _EXTERN_C_ extern "C"
#else /* __cplusplus */
#define _EXTERN_C_
#endif /* __cplusplus */
#endif /* _EXTERN_C_ */


int main(int argc, char** argv)
{
    printf("before init\n");
    MPI_Init(&argc, &argv);
    printf("after init\n");
/*
	printf("Init %d ", MPI_INIT_T);
	printf("Fin %d ", MPI_FINALIZE_T);
	printf("Send %d ", MPI_SEND_T);
	printf("Recv %d ", MPI_RECV_T);
	printf("Size %d ", MPI_COMM_SIZE_T);
	printf("Rank %d ", MPI_COMM_RANK_T);
*/
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("rank app %i\n", rank);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("size app %i\n", size);
    MPI_Finalize();

    printf("after finalize\n");

}
