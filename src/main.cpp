#include "job_manager.h"
#include <math.h>
#include <unistd.h>
#include "iostream"

void primes(int maxNum){
	for(int j = 2;j < maxNum;j++){
		for(int f = 2;f*f <= j;f++){
			if(j%f == 0){
				break;
			}else if(f+1 > sqrt(j)){
				std::cout << j << " " << std::endl;
			}
		}
	}
}

int main(){
	JobManager jb;
	Job t([](){
//		std::cout << "test " << " " << std::endl;
		primes(100);
	});

	jb.addJob(std::move(t));
	sleep(10000);
	return 0;
}