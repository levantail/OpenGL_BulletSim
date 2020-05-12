#pragma once
#include <thread>
#include <atomic>
#include <vector>
#include <future>
#include <mutex>
#include <queue>
#include <algorithm>

namespace detail {
	template <typename T>
	class Queue {
	public:
		bool push(T const& value) {
			std::unique_lock<std::mutex> lock(this->mutex);
			this->q.push(value);
			return true;
		}
		// deletes the retrieved element, do not use for non integral types
		bool pop(T& v) {
			std::unique_lock<std::mutex> lock(this->mutex);
			if (this->q.empty())
				return false;
			v = this->q.front();
			this->q.pop();
			return true;
		}
		bool empty() {
			std::unique_lock<std::mutex> lock(this->mutex);
			return this->q.empty();
		}
	private:
		std::queue<T> q;
		std::mutex mutex;
	};
}

class ThreadManager 
{

public:
	ThreadManager(unsigned int nThreads = std::thread::hardware_concurrency());

	// the destructor waits for all the functions in the queue to be finished
	~ThreadManager();

	// get the number of running threads in the pool
	int size() { return static_cast<int>(this->threads.size()); }

	// number of idle threads
	int n_idle() { return this->nWaiting; }
	std::thread& get_thread(int i) { return *this->threads[i]; }

	// change the number of threads in the pool
	// should be called from one thread, otherwise be careful to not interleave, also with this->stop()
	// nThreads must be >= 0
	void resize(int nThreads);

	//Wait for all threads until they finished their jobs and continue;
	void Barrier();

	// empty the queue
	void clear_queue();

	// pops a functional wrapper to the original function
	std::function<void(int)> pop();

	// wait for all computing threads to finish and stop all threads
	// may be called asynchronously to not pause the calling thread while waiting
	// if isWait == true, all the functions in the queue are run, otherwise the queue is cleared without running the functions
	void stop(bool isWait = false);

	template<typename F, typename... Rest>
	auto push(F&& f, Rest&&... rest)->std::future<decltype(f(0, rest...))>;

	// run the user's function that excepts argument int - id of the running thread. returned value is templatized
	// operator returns std::future, where the user can get the result and rethrow the catched exceptins
	template<typename F>
	auto push(F&& f)->std::future<decltype(f(0))>;


private:

	std::vector<std::unique_ptr<std::thread>> threads;
	std::vector<std::shared_ptr<std::atomic<bool>>> flags;
	detail::Queue<std::function<void(int id)>*> q;
	std::atomic<bool> isDone;
	std::atomic<bool> isStop;
	std::atomic<int> nWaiting;  // how many threads are waiting

	std::mutex mutex;
	std::condition_variable cv;

	void init() { this->nWaiting = 0; this->isStop = false; this->isDone = false; }

	// deleted
	ThreadManager(const ThreadManager&) = delete;
	ThreadManager(ThreadManager&&) = delete;
	ThreadManager& operator=(const ThreadManager&) = delete;
	ThreadManager& operator=(ThreadManager&&) = delete;

	void set_thread(int i);
};

template<typename F>
auto ThreadManager::push(F&& f) ->std::future<decltype(f(0))>
{
	auto pck = std::make_shared<std::packaged_task<decltype(f(0))(int)>>(std::forward<F>(f));
	auto _f = new std::function<void(int id)>([pck](int id) {
		(*pck)(id);
		});
	this->q.push(_f);
	std::unique_lock<std::mutex> lock(this->mutex);
	this->cv.notify_one();
	return pck->get_future();
}
