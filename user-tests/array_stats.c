// Brian Frasers Workshop code was used as inspiration for some of this
// Name studentNumber //** GIT privacy EDIT
// CMPT 300

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h> // For errno global
#include <assert.h>

#define SYSCALL_ARRAY_STATS 549 // for a 64 bit system

struct my_stats {
	long min;
	long max;
	long sum;
};

int main(int argc, char *argv[])
{
	struct my_stats *my_arr_ptr, my_arr1;
	my_arr_ptr = &my_arr1;

	// Sum: 15, Max: 10, Min: -10
	long my_array[7] = {10,3,4,5,2,-10,1};

	// -----------------------------------------
	// Test actual results and values
	long size = 7;
	int result = syscall(SYSCALL_ARRAY_STATS, my_arr_ptr, my_array, size);
	assert(result == 0);
	assert(my_arr_ptr->max == 10);
	assert(my_arr_ptr->min == -10);
	assert(my_arr_ptr->sum == 15);


	size = 3;
	result = syscall(SYSCALL_ARRAY_STATS, my_arr_ptr, my_array, size);
	assert(result == 0);
	assert(my_arr_ptr->max == 10);
	assert(my_arr_ptr->min == 3);
	assert(my_arr_ptr->sum == 17);


	size = 1;
	result = syscall(SYSCALL_ARRAY_STATS, my_arr_ptr, my_array, size);
	assert(result == 0);
	assert(my_arr_ptr->max == 10);
	assert(my_arr_ptr->min == 10);
	assert(my_arr_ptr->sum == 10);

	// -----------------------------------------
	// Check bad sizes
	size = -3;
	result = syscall(SYSCALL_ARRAY_STATS, my_arr_ptr, my_array, size);
	assert(result != 0);

	size = 0;
	result = syscall(SYSCALL_ARRAY_STATS, my_arr_ptr, my_array, size);
	assert(result != 0);

	size = 500;
	result = syscall(SYSCALL_ARRAY_STATS, my_arr_ptr, my_array, size);
	assert(result != 0);

	// -----------------------------------------
	// Pass incorrect addresses
	size = 1;
	result = syscall(SYSCALL_ARRAY_STATS, size, size, size);
	assert(result != 0);

	size = -62;
	result = syscall(SYSCALL_ARRAY_STATS, &size, &size, &size);
	assert(result != 0);

	result = syscall(SYSCALL_ARRAY_STATS, size, size, size);
	assert(result != 0);


	my_arr_ptr = NULL;
	result = syscall(SYSCALL_ARRAY_STATS, my_arr_ptr, my_arr_ptr, my_arr_ptr);
	assert(result != 0);

	result = syscall(SYSCALL_ARRAY_STATS, &my_arr_ptr, &my_arr_ptr, &my_arr_ptr);
	assert(result != 0);

	result = syscall(SYSCALL_ARRAY_STATS, &my_arr_ptr, my_arr_ptr, my_arr_ptr);
	assert(result != 0);

	return 0;
}
