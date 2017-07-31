#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

//Found at stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11
//Changed to be re-usable
class Barrier
{
	private:
		std::mutex _mutex;
		std::condition_variable _cv;
		std::size_t _count;
		bool canContinue = false;
	public:
		explicit Barrier(std::size_t count) : _count{count} { }
		void Wait(size_t newCount)
		{
			std::unique_lock<std::mutex> lock{_mutex};
			if(--_count == 0) {
				canContinue = true;
				_cv.notify_all();
			} else {
				canContinue = false;
				_cv.wait(lock, [this] {	return canContinue; });
				_count = newCount;
			}
		}
};
