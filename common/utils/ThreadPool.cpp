/*
 * =====================================================================================
 *
 *       Filename:  ThreadPool.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/28/2023 03:09:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include "ThreadPool.h"
#include <condition_variable>
#include <mutex>
#include <thread>
#include <functional>


ThreadPool::ThreadPool (int threads) : m_shutdown (false)
{
	// Create the specified number of threads
	for (int i = 0; i < threads; i++) {
		auto func =  [&]() {this->workerEntry();};
		m_workers.push_back(std::thread(func));
	}

	m_shutdown = false;
}

void ThreadPool::workerEntry ()
{
	std::function <void (void )> task;
	while (1)
	{
		{
			std::unique_lock<std::mutex> lck(m_mutex);			
			while (!m_shutdown && m_tasks.empty())  
				m_cv.wait(lck);

			if (m_tasks.empty()) {
				return;
			} else {
				task = std::move(m_tasks.front());
				m_tasks.pop();
			}
		}
		task();
		
	}
}


void ThreadPool::beginTask (std::function <void (void)> func) {
	std::unique_lock<std::mutex> lck(m_mutex);

	m_tasks.push(func);
	m_cv.notify_one();
}


#if 0
        std::mutex g_mutex;

// g++ ThreadPool.cpp -o ThreadPool
#include <cstdlib>
int main()
{
	// Create two threads
	ThreadPool p (20);

//	std::this_thread::sleep_for (std::chrono::seconds (5));

	int n;
	int i=1;
	for (int i =0;i<10;i++) {
		n = rand() % 5;
		p.beginTask([n,i]()->void{
			g_mutex.lock();
			std::cout << "Thred num "<<i<<" Sleeping for " << n << " seconds" << std::endl;
			g_mutex.unlock();

			std::this_thread::sleep_for (std::chrono::seconds (n+1));
		});

	}

	//std::this_thread::sleep_for (std::chrono::seconds (1000));



}
#endif

