#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>
#include <mutex>


using namespace std;
const int MaxCount = 15000;
const int ThreadCount = 16;

bool isPrimeNumber(int number) {
	if (number == 1)
		return false;
	if (number == 2 || number == 3)
		return true;

	for (int i = 2; i * i <= number; i++) {
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


	for (int i = 1; i <= MaxCount; i++) {
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