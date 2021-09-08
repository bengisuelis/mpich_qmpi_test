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

struct tool_struct {
    int my_tool_id;
};
struct tool_struct my_tool_struct;

static int test_val = 0;

__attribute__ ((constructor))
void static_register_my_tool(void);

QMPI_Init_t *next_init_fn=NULL;
int next_init_id;
QMPI_Comm_rank_t *next_rank_fn=NULL;
int next_rank_id;
QMPI_Comm_size_t *next_size_fn=NULL;
int next_size_id;
QMPI_Finalize_t *next_finalize_fn=NULL;
int next_finalize_id;

int Test_Finalize (QMPI_Context context, int tool_id);
int Test_Init	  (QMPI_Context context, int tool_id, int *argc, char ***argv );
int Test_Comm_size(QMPI_Context context, int tool_id, MPI_Comm comm, int *size);
int Test_Comm_rank(QMPI_Context context, int tool_id, MPI_Comm comm, int *rank);

//------------------------------------------------------------------------------------------------------------
int Test_Finalize(QMPI_Context context, int tool_id)
{
    int ret;
    struct tool_struct *storage;

    QMPI_Get_tool_storage(context, tool_id, (void *) &storage);
    ret = (*next_finalize_fn) (context, next_finalize_id);
    return ret;
}

int Test_Comm_rank(QMPI_Context context, int tool_id, MPI_Comm comm, int *rank)
{
    int ret;
    struct tool_struct *storage;

    QMPI_Get_tool_storage(context, tool_id, (void *) &storage);
    test_val++;

    ret = (*next_rank_fn) (context, next_rank_id, comm, rank);
    return ret;
}

int Test_Comm_size (QMPI_Context context, int tool_id, MPI_Comm comm, int *size)
{
    int ret;
    struct tool_struct *storage;

    QMPI_Get_tool_storage(context, tool_id, (void *) &storage);
    test_val++;

    ret = (*next_size_fn) (context, next_size_id, comm, size );
    return ret;
}

int Test_Init( QMPI_Context context, int tool_id, int *argc, char ***argv )
{
    QMPI_Init_t *next_init_fn;
    int ret;
    struct tool_struct *storage;

    QMPI_Get_tool_storage(context, tool_id, (void *) &storage);
    QMPI_Get_function(tool_id, MPI_INIT_T, (void (**)(void)) &next_init_fn, &next_init_id);
    QMPI_Get_function(tool_id, MPI_FINALIZE_T, (void (**)(void)) &next_finalize_fn,&next_finalize_id);
    QMPI_Get_function(tool_id, MPI_COMM_RANK_T, (void (**)(void)) &next_rank_fn, &next_rank_id);
    QMPI_Get_function(tool_id, MPI_COMM_SIZE_T, (void (**)(void)) &next_size_fn, &next_size_id);

    ret = (*next_init_fn) (context, next_init_id, argc, argv);

    return ret;
}

/* ================== C Wrappers for MPI_Init ================== 200 */

void Injection  (int tool_id) {
	printf("Will try to launch Py Int \n ");
      	my_tool_struct.my_tool_id = tool_id;
	QMPI_Register_tool_storage( tool_id, &my_tool_struct );
	printf("No errors until here \n ");
    	QMPI_Register_function(tool_id, MPI_FINALIZE_T, (void (*)(void)) &Test_Finalize);
    	QMPI_Register_function(tool_id, MPI_INIT_T, (void (*)(void)) &Test_Init);
    	QMPI_Register_function(tool_id, MPI_COMM_RANK_T, (void (*)(void)) &Test_Comm_rank);
  	QMPI_Register_function(tool_id, MPI_COMM_SIZE_T, (void (*)(void)) &Test_Comm_size);
  	return ;
}


__attribute__ ((constructor))
void static_register_my_tool()
{
    printf("attribute is called \n ");
    QMPI_Register_tool_name("pytool", &Injection);
}
