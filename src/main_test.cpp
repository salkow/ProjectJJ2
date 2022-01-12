// #include <math.h>
// #include <cassert>
// #include <unistd.h>
// #include "job_manager.h"
// #include "iostream"
// #include "vector.h"
// #include "array.h"
// #include "string_breaker.h"
// #include "string"
// #include "implementation.h"

// using namespace bud;

// int primes2(const int maxPrime)
// {
// 	int last = 0;
// 	for (int j = 2; j < (maxPrime * 1000); j++)
// 	{
// 		for (int f = 2; f * f <= j; f++)
// 		{
// 			if (j % f == 0)
// 			{
// 				break;
// 			}
// 			else if (f + 1 > sqrt(j))
// 			{
// 				last = j;
// 			}
// 		}
// 	}
// 	return last;
// }

// int primes(const bud::vector<int> &nums, const int idx)
// {
// 	int last = 0;
// 	for (int j = 2; j < nums[idx]; j++)
// 	{
// 		for (int f = 2; f * f <= j; f++)
// 		{
// 			if (j % f == 0)
// 			{
// 				break;
// 			}
// 			else if (f + 1 > sqrt(j))
// 			{
// 				last = j;
// 			}
// 		}
// 	}
// 	return last;
// }

// ErrorCode matchDocument(const bud::vector<string> &words, size_t start, size_t end,
// 						Result &res)
// {

// 	res.m_query_ids.insert(primes2(static_cast<int>(words[start][0])));
// 	return ErrorCode::EC_FAIL;
// }

// void match()
// {
// 	vector<Result> m_res;
// 	JobManager m_jm;
// 	char test[] = "asdkas d as ds df dsf gdfgio4i 3 toedigdf goi 3oit fofigdofgi 4oit";
// 	const bud::vector<string> words = string_breaker(test);
// 	if (words.size() == 75)
// 	{
// 		auto x = true;
// 	}
// 	//	assert(words.size() == 1066);

// 	const size_t real_num_of_threads = bud::min(static_cast<size_t>(NUM_OF_THREADS), words.size() - 1);
// 	size_t split = words.size() / real_num_of_threads;
// 	size_t start = 0;
// 	size_t end = split;

// 	bud::array<Result, NUM_OF_THREADS> res;
// 	bud::array<ErrorCode, NUM_OF_THREADS> err;

// 	m_jm.waitFinishAllJobs();
// 	assert(m_jm.num_of_jobs() == 0);

// 	size_t i;
// 	for (i = 0; i < real_num_of_threads - 1; i++)
// 	{
// 		m_jm.addJob(Job([&, start, end, i]() { //words
// 			err[i] = matchDocument(words, start, end, res[i]);
// 		}));

// 		start = end + 1;
// 		end += split;
// 	}

// 	m_jm.addJob(Job([&, start, real_num_of_threads]()
// 					{
// 						//		assert(words.size() == 1066);
// 						//		assert(words.size()-1 == end);
// 						err[real_num_of_threads - 1] = matchDocument(words, start, words.size() - 1, res[real_num_of_threads - 1]);
// 					}));

// 	//	assert(m_jm.num_of_jobs() != 0);
// 	m_jm.waitFinishAllJobs();

// 	Result fin_res;
// 	fin_res.m_doc_id = DocID(123);
// 	for (size_t i2 = 0; i2 < real_num_of_threads; i2++)
// 	{
// 		const auto &item = res[i2];
// 		for (const auto &x : item.m_query_ids)
// 		{
// 			// fin_res.m_query_ids.insert(x);
// 		}
// 	}

// 	m_res.emplace_back(std::move(fin_res));

// 	// queries_matched_words_reset();
// 	assert(m_jm.num_of_jobs() == 0);
// 	assert(m_jm.get_num_of_running_jobs() == 0);
// }

// void f()
// {
// 	JobManager jb;
// 	bud::vector<int> nums;
// 	bud::array<int, 100> results;

// 	for (int i = 0; i < 100; i++)
// 	{
// 		nums.push_back(i * 1000);
// 	};

// 	for (int i = 0; i < 99; i++)
// 	{
// 		jb.addJob(Job([&, i]()
// 					  { results[i] = primes(nums, i); }));
// 	}
// 	jb.addJob(Job([&]()
// 				  { results[100] = primes(nums, 100); }));
// 	jb.waitFinishAllJobs();

// 	for (auto &result : results)
// 	{
// 		// std::cout << result << std::endl;
// 	}
// }

// int main()
// {
// 	for (int i = 0; i < 5; i++)
// 	{

// 		match();
// 		std::cout << "hello " << i << std::endl;
// 	}
// 	return 0;
// }
