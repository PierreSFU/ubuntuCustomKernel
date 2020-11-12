// Brian Frasers Workshop code was used as inspiration for some of this
// Name studentNumber //** GIT privacy EDIT
// CMPT 300

#include <linux/kernel.h>
#include <linux/syscalls.h>

struct array_stats_s {
	long min;
	long max;
	long sum;
};

//syscall(struct* my_array,long* data,long size);
// stats is where we store things, data is our array, size
// is the size of our array
SYSCALL_DEFINE3(array_stats, struct array_stats_s*, stats, 
				long*, data, long, size)
{
	int stop_warning = 0;

	// Tests, validity of inputs
	//struct array_stats_s* my_stats;
	long my_data;

	// Our function counters/tracker
	int i = 0;
	int initialization = 0;

	// Our value trackers
	long my_min = 0;
	long my_max = 0;
	long my_sum = 0;

	// Error Checkers
	if(size <= 0){
		return -EINVAL;
	}
	else if(stats == NULL){
		return -EFAULT;
	}

	// Copy data over
	for(i = 0; i < size; i++)
	{
		// Nothing to copy
		if(copy_from_user(&my_data,&data[i], sizeof(long)) != 0){
			return -EFAULT;
		}
		else
		{
			// *** Technically possible, but is likely invalid ***
			if(my_data >= 100000000 ||my_data <= -100000000){
				//printk("This may be an error! Depending on what the instructor wants!");
				return -EINVAL;
			}

			// Initialize to first value 
			if(initialization == 0){
				my_min = my_data;
				my_max = my_data;
				initialization = 1;
			}

			// Update only if it's greater than
			if(my_min > my_data){
				my_min = my_data;
			}
			// Update only if it's less than
			if(my_max < my_data){
				my_max = my_data;
			}

			my_sum = my_data + my_sum;
		}
	}

	// Copy data over to user, since everything was successful
	stop_warning = copy_to_user(&stats->max,&my_max, sizeof(long));
	stop_warning = copy_to_user(&stats->min,&my_min, sizeof(long));
	stop_warning = copy_to_user(&stats->sum,&my_sum, sizeof(long));

	if(stop_warning >= 1){
	return -EFAULT;
	}

	return 0;
}
