/*
 * =====================================================================================
 *
 *       Filename:  SignalManager.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/19/2024 11:16:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef SIGNAL_MANAGER_H
#define SIGNAL_MANAGER_H

#include <condition_variable>
#include <mutex>
#include <iostream>
#include <thread>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

class SignalManager {
	public:
		SignalManager() : done(false), signal_received(false) {}

		void wait() {
			std::unique_lock<std::mutex> lk(m_mtx);
			if (signal_received) {
				// If a signal has already been received, proceed immediately
				signal_received = false;
			//	logTime("Thread " + threadIdToString(std::this_thread::get_id()) + " proceeding without wait.");
				return;
			}
		//	logTime("Thread " + threadIdToString(std::this_thread::get_id()) + " is waiting.");
			cv.wait(lk, [this] { return done; }); // Wait until 'done' is true
		//	logTime("Thread " + threadIdToString(std::this_thread::get_id()) + " is proceeding.");
			done = false; // Reset done flag for the next wait
		}

		void signal() {
			{
				std::lock_guard<std::mutex> lk(m_mtx);
				done = true;
				signal_received = true; // Indicate that a signal has been received
			//	logTime("Signaling from thread " + threadIdToString(std::this_thread::get_id()));
			}
			cv.notify_all(); // Notify all waiting threads
		}

	private:
		std::mutex m_mtx;
		std::condition_variable cv;
		bool done;
		bool signal_received;

		std::string threadIdToString(std::thread::id id) {
			std::ostringstream oss;
			oss << id;
			return oss.str();
		}

		void logTime(const std::string& message) {
			auto now = std::chrono::system_clock::now();
		//	auto in_time_t = std::chrono::system_clock::to_time_t(now);
			std::tm buf;
	//		localtime_r(&in_time_t, &buf);
			std::ostringstream oss;
			oss << std::put_time(&buf, "%Y-%m-%d %X");
			std::cout << "[" << oss.str() << "] " << message << std::endl;
		}
};

#endif // SIGNAL_MANAGER_H

