

//#include <iostream>
//using namespace std::chrono_literals;
//
//int main() {
//	auto now = std::chrono::high_resolution_clock::now;
//	auto s = now();
//	{
//		ThreadPool p(10);
//		p.start();
//
//		p.execute([] { std::this_thread::sleep_for(1s); });
//		p.execute([] { std::this_thread::sleep_for(600ms); });
//		p.execute([] { std::this_thread::sleep_for(400ms); });
//		p.execute([] { std::this_thread::sleep_for(200ms); });
//		p.execute([] { std::this_thread::sleep_for(10ms); });
//		
//		p.join_all();
//
//		std::cout << "Total elapsed: " << (now() - s) / 1.0s << "s\n";
//
//		p.execute([] { std::this_thread::sleep_for(1s); });
//		p.execute([] { std::this_thread::sleep_for(600ms); });
//		p.execute([] { std::this_thread::sleep_for(400ms); });
//		p.execute([] { std::this_thread::sleep_for(200ms); });
//		p.execute([] { std::this_thread::sleep_for(10ms); });
//		
//		p.join_all();
//
//	}
//
//	std::cout << "Total elapsed: " << (now() - s) / 1.0s << "s\n";
//}

//OpenGL GLEW
#include "Engine.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 820;

//"Bullet Manager Task"
int main(void)
{
	//TODO:
	// + Cache function bind calls to opengl API
	// + multi-threading at creating new objects and at collision calculation
	// + Auto-mode to fit desired performance

	Engine engine;

	//Engine Start
	if (engine.Initialize("Bullet Manager Task", SCR_WIDTH, SCR_HEIGHT)) {
		while (!engine.ProceedMessage())
		{
			engine.RenderFrame();
		}
	}

	engine.TerminateAndClose();
	return 0;
}



//OpenGL GLEW

//#include <iostream>
//#include <thread>
//#include <vector>
//#include <mutex>
//#include <deque>
//#include <future>
//
//
//struct tasks {
//	// the mutex, condition variable and deque form a single
//	// thread-safe triggered queue of tasks:
//	std::mutex m;
//	std::condition_variable v;
//	// note that a packaged_task<void> can store a packaged_task<R>:
//	std::deque<std::packaged_task<void()>> work;
//
//	// this holds futures representing the worker threads being done:
//	std::vector<std::future<void>> finished;
//
//	// queue( lambda ) will enqueue the lambda into the tasks for the threads
//	// to use.  A future of the type the lambda returns is given to let you get
//	// the result out.
//	template<class F, class R = std::result_of_t<F& ()>>
//	std::future<R> queue(F&& f) {
//		// wrap the function object into a packaged task, splitting
//		// execution from the return value:
//		std::packaged_task<R()> p(std::forward<F>(f));
//
//		auto r = p.get_future(); // get the return value before we hand off the task
//		{
//			std::unique_lock<std::mutex> l(m);
//			work.emplace_back(std::move(p)); // store the task<R()> as a task<void()>
//		}
//		v.notify_one(); // wake a thread to work on the task
//
//		return r; // return the future result of the task
//	}
//
//	// start N threads in the thread pool.
//	void start(std::size_t N = 1) {
//		for (std::size_t i = 0; i < N; ++i)
//		{
//			// each thread is a std::async running this->thread_task():
//			finished.push_back(
//				std::async(
//					std::launch::async,
//					[this] { thread_task(); }
//					)
//				);
//		}
//	}
//	// abort() cancels all non-started tasks, and tells every working thread
//	// stop running, and waits for them to finish up.
//	void abort() {
//		cancel_pending();
//		finish();
//	}
//	// cancel_pending() merely cancels all non-started tasks:
//	void cancel_pending() {
//		std::unique_lock<std::mutex> l(m);
//		work.clear();
//	}
//	// finish enques a "stop the thread" message for every thread, then waits for them:
//	void finish() {
//		{
//			std::unique_lock<std::mutex> l(m);
//			for (auto&& unused : finished) {
//				work.push_back({});
//			}
//		}
//		v.notify_all();
//		finished.clear();
//	}
//	~tasks() {
//		finish();
//	}
//private:
//	// the work that a worker thread does:
//	void thread_task() {
//		while (true) {
//			// pop a task off the queue:
//			std::packaged_task<void()> f;
//			{
//				// usual thread-safe queue code:
//				std::unique_lock<std::mutex> l(m);
//				if (work.empty()) {
//					v.wait(l, [&] {return !work.empty(); });
//				}
//				f = std::move(work.front());
//				work.pop_front();
//			}
//			// if the task is invalid, it means we are asked to abort:
//			if (!f.valid()) return;
//			// otherwise, run the task:
//			f();
//		}
//	}
//};
//
//
//
//class foo {
//public:
//	foo(int val) : val(val) {};
//	~foo() {};
//
//	void dosomejob()
//	{
//		int count = 0;
//		for (int i = 0; i < INT_MAX; ++i)
//			++count;
//
//		val = count + 2 + val;
//	}
//
//	int val;
//
//};
//
//std::vector<std::unique_ptr<foo>> temp = {};
//
//void function1(int start, int end)
//{
//	for (int i = start; i < end; ++i)
//		temp[i]->dosomejob();
//}
//
//int main(void)
//{
//	for (int i = 0; i < 10; ++i)
//		temp.push_back(
//			std::make_unique<foo>(i+1)
//			);
//	tasks t;
//
//	t.start(24);
//
//	t.queue([] {function1(0, 3); });
//	t.queue([] {function1(0, 3); });
//	t.queue([] {function1(0, 3); });
//	t.queue([] {function1(0, 3); });
//	t.queue([] {function1(0, 3); });
//
//	t.finish();
//	for (int i = 0; i < temp.size(); ++i)
//	{
//		std::cout << temp[i]->val << std::endl;
//	}
//
//	return 0;
//}


//#include <iostream>
//#include <deque>
//#include <functional>
//#include <thread>
//#include <condition_variable>
//#include <mutex>
//#include <random>
//#include <atomic>
//
////thread pool
//class ThreadPool
//{
//public:
//	ThreadPool(unsigned int n = std::thread::hardware_concurrency());
//
//	template<class F> void enqueue(F&& f);
//	void waitFinished();
//	~ThreadPool();
//
//	unsigned int getProcessed() const { return processed; }
//
//private:
//	std::vector< std::thread > workers;
//	std::deque< std::function<void()> > tasks;
//	std::mutex queue_mutex;
//	std::condition_variable cv_task;
//	std::condition_variable cv_finished;
//	std::atomic_uint processed;
//	unsigned int busy;
//	bool stop;
//
//	void thread_proc();
//};
//
//ThreadPool::ThreadPool(unsigned int n)
//	: busy()
//	, processed()
//	, stop()
//{
//	for (unsigned int i = 0; i < n; ++i)
//		workers.emplace_back(std::bind(&ThreadPool::thread_proc, this));
//}
//
//ThreadPool::~ThreadPool()
//{
//	// set stop-condition
//	std::unique_lock<std::mutex> latch(queue_mutex);
//	stop = true;
//	cv_task.notify_all();
//	latch.unlock();
//
//	// all threads terminate, then we're done.
//	for (auto& t : workers)
//		t.join();
//}
//
//void ThreadPool::thread_proc()
//{
//	while (true)
//	{
//		std::unique_lock<std::mutex> latch(queue_mutex);
//		cv_task.wait(latch, [this]() { return stop || !tasks.empty(); });
//		if (!tasks.empty())
//		{
//			// got work. set busy.
//			++busy;
//
//			// pull from queue
//			auto fn = tasks.front();
//			tasks.pop_front();
//
//			// release lock. run async
//			latch.unlock();
//
//			// run function outside context
//			fn();
//			++processed;
//
//			latch.lock();
//			--busy;
//			cv_finished.notify_one();
//		}
//		else if (stop)
//		{
//			break;
//		}
//	}
//}
//
//// generic function push
//template<class F>
//void ThreadPool::enqueue(F&& f)
//{
//	std::unique_lock<std::mutex> lock(queue_mutex);
//	tasks.emplace_back(std::forward<F>(f));
//	cv_task.notify_one();
//}
//
//// waits until the queue is empty.
//void ThreadPool::waitFinished()
//{
//	std::unique_lock<std::mutex> lock(queue_mutex);
//	cv_finished.wait(lock, [this]() { return tasks.empty() && (busy == 0); });
//}
//
//// a cpu-busy task.
//void work_proc()
//{
//	std::random_device rd;
//	std::mt19937 rng(rd());
//
//	// build a vector of random numbers
//	std::vector<int> data;
//	data.reserve(100000);
//	std::generate_n(std::back_inserter(data), data.capacity(), [&]() { return rng(); });
//	std::sort(data.begin(), data.end(), std::greater<int>());
//}
//
//int main()
//{
//	ThreadPool tp;
//
//	// run five batches of 100 items
//	for (int x = 0; x < 5; ++x)
//	{
//		
//		// queue 100 work tasks
//		for (int i = 0; i < 24; ++i)
//			tp.enqueue(&work_proc);
//
//		tp.waitFinished();
//		std::cout << tp.getProcessed() << '\n';
//
//		for (int i = 0; i < 24; ++i)
//			tp.enqueue(&work_proc);
//
//		tp.waitFinished();
//		std::cout << tp.getProcessed() << '\n';
//	}
//
//	// destructor will close down thread pool
//	return EXIT_SUCCESS;
//}