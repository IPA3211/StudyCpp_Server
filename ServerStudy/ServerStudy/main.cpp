#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>
#include <mutex>
#include "criticalSection.h"


using namespace std;
const int MaxCount = 15000000;
const int ThreadCount = 16;

bool isPrimeNumber(int number) {

	volatile int a = 0;
	
	if (number == 1)
		return false;
	if (number == 2 || number == 3)
		return true;

	for (int i = 2; i <= number; i++) {
		if (number % i == 0)
			return false;
	}

	return true;
}

void PrintNumbers(const vector<int>& prime){
	for (int v : prime) {
		cout << v << endl;
	}
}

int main(void) {
	int num = 1;

	vector<int> primes;
	recursive_mutex num_mutex;
	recursive_mutex primes_mutex;

	auto t0 = chrono::system_clock::now();

	vector<shared_ptr<thread>> threads;


	for (int i = 1; i <= ThreadCount; i++) {
		shared_ptr<thread> thread(new thread([&]() {
		
			while (true) {

				int n;
				{
					lock_guard<recursive_mutex> num_lock(num_mutex);
					n = num;
					num++;
				}
				if (n >= MaxCount)
					break;

				if (isPrimeNumber(n)) {
					lock_guard<recursive_mutex> primes_lock(primes_mutex);
					primes.push_back(n);
				}
			}

		}));
		threads.push_back(thread);
	}

	for (auto thread : threads) {
		thread->join();
	}

	auto t1 = chrono::system_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();

	PrintNumbers(primes);

	cout << "Took " << duration << " milli Sec." << endl;
}
//
//int a;
//CriticalSection a_mutex;
//int b;
//CriticalSection b_mutex;
//
//int main() {
//	thread t1([]() {
//		while (1)
//		{
//			CriticalSectionLock lock(a_mutex);
//			a++;
//			CriticalSectionLock lock2(b_mutex);
//			b++;
//			cout << "t1 done.\n";
//		}
//	});
//
//	thread t2([]() {
//		while (1)
//		{
//			CriticalSectionLock lock2(a_mutex);
//			a++;
//			CriticalSectionLock lock(b_mutex);
//			b++;
//			cout << "t2 done.\n";
//		}
//	});
//
//	t1.join();
//	t2.join();
//
//	return 0;
//}