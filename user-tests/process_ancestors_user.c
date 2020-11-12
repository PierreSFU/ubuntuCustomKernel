// Brian Frasers Workshop code was used as inspiration for some of this
// Pierre Drego 301301848
// CMPT 300

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <assert.h>
#include <errno.h>         // For errno global

#define SYSCALL_PROCESS_ANCESTORS 550


#define ANCESTOR_NAME_LEN 16
struct my_process_info {
long pid; /* Process ID */
char name[ANCESTOR_NAME_LEN]; /* Program name of process */
long state; /* Current process state */
long uid; /* User ID of process owner */
long nvcsw; /* # voluntary context switches */
long nivcsw; /* # involuntary context switches */
long num_children; /* # children process has */
long num_siblings; /* # sibling process has */
};


// Assumption "structs is passed as a pointer not a memory address",
// syscall(structs) not syscall(&structs), however it should work OKAY
// regardless

int main(int argc, char *argv[])
{
    struct my_process_info array_of_structs[6];
	
	array_of_structs[0].pid = 100;
	array_of_structs[0].name[0] = '*';
	array_of_structs[0].state = 200;
	array_of_structs[0].uid = 300;
	array_of_structs[0].nvcsw = 400;
	array_of_structs[0].nivcsw = 500;
	array_of_structs[0].num_children = 600;
	array_of_structs[0].num_siblings = 700;

    long array_size = 10;
    long* num_filled;

	// printf("\nDiving to kernel level\n\n");

	int result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    &array_of_structs, array_size, &num_filled);
	assert(result == 0);

	// printf("\nRising to user level w/ result = %d\n\n", result);

	

	// Used for looking at each array and testing last value, optional
/*
	int i = 0;
	printf("Userspace, My number of filled arrays is %ld\n", (long)num_filled);
	

	for(i = 0; i < (long)num_filled; i++){
	
		printf("\n--Array[%d] ", i);
		printf("My PID is %ld\n", array_of_structs[i].pid);
		printf("My name is %s\n", array_of_structs[i].name);
		printf("My state is %ld\n", array_of_structs[i].state);
		printf("My uid is %ld\n", array_of_structs[i].uid);
		printf("My nvscw is %ld\n", array_of_structs[i].nvcsw);
		printf("My nivcsw is %ld\n", array_of_structs[i].nivcsw);
		printf("Number of children %ld\n", array_of_structs[i].num_children);
		printf("Number of siblings %ld\n\n", array_of_structs[i].num_siblings);
	
	}

	i--; // Testing last array
	printf("My location %d, pid is %ld\n\n", i, array_of_structs[i].pid);
	assert(array_of_structs[i].pid == 0); // Make sure the last parent is the swapper
*/
	// WEAK test, this COULD be very inaccurate. Makes sure everything has changed
	assert(array_of_structs[0].pid  != 100);
	assert(array_of_structs[0].name[0] != '*');
	assert(array_of_structs[0].state != 200);
	assert(array_of_structs[0].uid != 300);
	assert(array_of_structs[0].nvcsw != 400);
	assert(array_of_structs[0].nivcsw != 500);
	assert(array_of_structs[0].num_children != 600);
	assert(array_of_structs[0].num_siblings != 700);

	// -----------------------------------------

	int potato = 0;
	int* potato_ptr = NULL;
	// Pass a good pointer
	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    array_of_structs, array_size, &num_filled);
	assert(result == 0);

	// Pass an invalid value
	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    potato, array_size, &num_filled);
	assert(result != 0);

	// Pass a bad pointer
	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    potato_ptr, array_size, &num_filled);
	assert(result != 0);

	// -----------------------------------------
	// Pass wrong everything, with different memory
	// values
	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    potato, potato, potato);
	assert(result != 0);

	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    potato_ptr, potato_ptr, potato_ptr);
	assert(result != 0);

	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    &potato_ptr, &potato_ptr, &potato_ptr);
	assert(result != 0);

	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    &potato_ptr, potato_ptr, &potato_ptr);
	assert(result != 0);

	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    potato_ptr, &potato_ptr, potato_ptr);
	assert(result != 0);

	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    &potato, potato_ptr, &potato_ptr);
	assert(result != 0);

	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    potato_ptr, &potato, potato_ptr);
	assert(result != 0);

	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    &potato_ptr, potato, &potato);
	assert(result != 0);

	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    potato, &potato_ptr, potato_ptr);
	assert(result != 0);

	// -----------------------------------------
	// Pass an invalid array_size
	array_size = -32;
	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    array_of_structs, array_size, &num_filled);
	assert(result != 0);

	// Pass a valid array_size
	array_size = 33;
	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    array_of_structs, array_size, &num_filled);
	assert(result == 0);

	// -----------------------------------------
	// Test Num_filled
	array_size = 1;
	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    array_of_structs, array_size, &num_filled);
	assert((long)num_filled == 1);

	array_size = 3;
	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    array_of_structs, array_size, &num_filled);
	assert((long)num_filled == 3);	

	array_size = 0;
	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    array_of_structs, array_size, &num_filled);
	assert(num_filled == 0);

	array_size = 33;
	result = syscall(SYSCALL_PROCESS_ANCESTORS, 
    array_of_structs, array_size, &num_filled);
	assert((long)num_filled >= 3 && (long)num_filled <= 33); // May be inaccurate
	
	return 0;
}

/* OPTIONAL
// NOTE: **** different structures tested, but nothing happened already ****

	//int errorCode = errno;
	//printf("\nRising to user level w/ result = %d (err #%d)\n\n", 
	//	result, errorCode);
*/