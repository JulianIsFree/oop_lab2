#ifndef WORKER_H
#define WORKER_H

#include <vector>
#include <string>

namespace labWorker
{
	template<class T> 
	class Worker
	{
	public:
		Worker(const std::vector<T>& args) {};
		virtual ~Worker() {};

		virtual void doWork() {};
		virtual const std::vector<T>& getOutput() const { return std::vector<T>(); };
		virtual void setInput(const std::vector<T>&) {};
	};
}

#endif // !WORKER_H