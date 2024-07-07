/*
 * =====================================================================================
 *
 *       Filename:  ThreadPool.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/28/2023 03:10:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */
#ifndef ThreadPool_h
#define ThreadPool_h 
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>
#include "common/logger.h"


class ThreadPool
{
	public:

		ThreadPool (int threads) ;
		~ThreadPool ()  {
			logger_print(LOG_DEBUG,"Running form ~ThreadPool");

			//finish();	
			//join();
		}

		void beginTask (std::function <void (void)> func);
		size_t size() {
			return m_workers.size();
		}
		static std::shared_ptr<ThreadPool> Instance() {
			// an elegant way to create instance only once  (aka)
			static std::shared_ptr<ThreadPool> singleToneInit = std::make_shared<ThreadPool>(7);
			return singleToneInit ;	
		}

		void join() {
			for ( auto & t : m_workers) {
				t.join();
			}
		}

		void finish() {
			m_shutdown = true;
			m_cv.notify_all();

		}
		void submitTask(std::function<void(void)> task){
			beginTask(task); 
		}
		bool shutdown() {
			return m_shutdown;
		}
	protected:

		void workerEntry ();

		std::mutex m_mutex;
		std::condition_variable m_cv;
		bool m_shutdown = false;
		std::queue <std::function <void (void )>> m_tasks;
		std::vector <std::thread> m_workers;
};



#endif

