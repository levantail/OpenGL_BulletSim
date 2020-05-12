#include "ThreadManager.h"

ThreadManager::ThreadManager(unsigned int nThreads /*= std::thread::hardware_concurrency()*/)
{
	this->init();
	nThreads = std::clamp(nThreads, 1u, std::thread::hardware_concurrency());
	this->resize(nThreads);
}

ThreadManager::~ThreadManager() {
	this->stop(true);
}

void ThreadManager::resize(int nThreads) 
{
	if (!this->isStop && !this->isDone) 
	{
		int oldNThreads = static_cast<int>(this->threads.size());
		if (oldNThreads <= nThreads) 
		{  // if the number of threads is increased
			this->threads.resize(nThreads);
			this->flags.resize(nThreads);

			for (int i = oldNThreads; i < nThreads; ++i) 
			{
				this->flags[i] = std::make_shared<std::atomic<bool>>(false);
				this->set_thread(i);
			}
		}
		else {  // the number of threads is decreased
			for (int i = oldNThreads - 1; i >= nThreads; --i) 
			{
				*this->flags[i] = true;  // this thread will finish
				this->threads[i]->detach();
			}
			{
				// stop the detached threads that were waiting
				std::unique_lock<std::mutex> lock(this->mutex);
				this->cv.notify_all();
			}
			this->threads.resize(nThreads);  // safe to delete because the threads are detached
			this->flags.resize(nThreads);  // safe to delete because the threads have copies of shared_ptr of the flags, not originals
		}
	}
}

void ThreadManager::Barrier()
{
	while (nWaiting != threads.size())
	{
		;
	}
}

void ThreadManager::clear_queue() 
{
	std::function<void(int id)>* _f;
	while (this->q.pop(_f))
		delete _f; // empty the queue
}

std::function<void(int)> ThreadManager::pop() 
{
	std::function<void(int id)>* _function = nullptr;
	this->q.pop(_function);
	// at return, delete the function even if an exception occurred
	std::unique_ptr<std::function<void(int id)>> func(_function);
	std::function<void(int)> f;
	if (_function)
		f = *_function;
	return f;
}

void ThreadManager::stop(bool isWait/* = false*/) 
{
	if (!isWait) {
		if (this->isStop)
			return;
		this->isStop = true;
		for (int i = 0, n = this->size(); i < n; ++i) 
		{
			*this->flags[i] = true;  // command the threads to stop
		}
		this->clear_queue();  // empty the queue
	}
	else 
	{
		if (this->isDone || this->isStop)
			return;
		this->isDone = true;  // give the waiting threads a command to finish
	}
	{
		std::unique_lock<std::mutex> lock(this->mutex);
		this->cv.notify_all();  // stop all waiting threads
	}
	for (int i = 0; i < static_cast<int>(this->threads.size()); ++i) 
	{  // wait for the computing threads to finish
		if (this->threads[i]->joinable())
			this->threads[i]->join();
	}
	// if there were no threads in the pool but some functors in the queue, the functors are not deleted by the threads
	// therefore delete them here
	this->clear_queue();
	this->threads.clear();
	this->flags.clear();
}

template<typename F, typename... Rest>
auto ThreadManager::push(F&& f, Rest&&... rest) ->std::future<decltype(f(0, rest...))> 
{
	auto pck = std::make_shared<std::packaged_task<decltype(f(0, rest...))(int)>>(
		std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<Rest>(rest)...)
		);
	auto _f = new std::function<void(int id)>([pck](int id) {
		(*pck)(id);
		});
	this->q.push(_f);
	std::unique_lock<std::mutex> lock(this->mutex);
	this->cv.notify_one();
	return pck->get_future();
}

void ThreadManager::set_thread(int i) 
{
	// a copy of the shared ptr to the flag
	std::shared_ptr<std::atomic<bool>> flag(this->flags[i]);
	auto f = [this, i, flag/* a copy of the shared ptr to the flag */]() {
		std::atomic<bool>& _flag = *flag;
		std::function<void(int id)>* _f;
		bool isPop = this->q.pop(_f);
		while (true) {
			while (isPop) {  // if there is anything in the queue
				std::unique_ptr<std::function<void(int id)>> func(_f); // at return, delete the function even if an exception occurred
				(*_f)(i);
				if (_flag)
					return;  // the thread is wanted to stop, return even if the queue is not empty yet
				else
					isPop = this->q.pop(_f);
			}
			// the queue is empty here, wait for the next command
			std::unique_lock<std::mutex> lock(this->mutex);
			++this->nWaiting;
			this->cv.wait(lock, [this, &_f, &isPop, &_flag]() { isPop = this->q.pop(_f); return isPop || this->isDone || _flag; });
			--this->nWaiting;
			if (!isPop)
				return;  // if the queue is empty and this->isDone == true or *flag then return
		}
	};
	this->threads[i].reset(new std::thread(f));
}