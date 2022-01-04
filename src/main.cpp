#include "job_manager.h"
#include <math.h>
#include <unistd.h>
#include "iostream"

int primes(int maxNum)
{
	int last = 0;
	for (int j = 2; j < maxNum; j++)
	{
		for (int f = 2; f * f <= j; f++)
		{
			if (j % f == 0)
			{
				break;
			}
			else if (f + 1 > sqrt(j))
			{
				last = j;
			}
		}
	}
	return last;
}

int main()
{
	JobManager jb;
	int last_nums[3] = {0, 0, 0};
	Job t1([&last_nums]()
		   {
			   //		std::cout << "test " << " " << std::endl;
			   last_nums[0] = primes(100);
		   });
	Job t2([&last_nums]()
		   {
			   //		std::cout << "test " << " " << std::endl;
			   last_nums[1] = primes(9000);
		   });
	Job t3([&last_nums]()
		   {
			   //		std::cout << "test " << " " << std::endl;
			   last_nums[2] = primes(85);
		   });

	jb.addJob(std::move(t1));
	jb.addJob(std::move(t2));
	jb.addJob(std::move(t3));
	jb.waitFinishAllJobs();
	std::cout << "finished, found: " << last_nums[0] << " " << last_nums[1] << " " << last_nums[2] << std::endl;
	jb.terminate();
	return 0;
}
