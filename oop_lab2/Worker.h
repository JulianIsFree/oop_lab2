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
		virtual void doWork() {};
		virtual const std::vector<T>& getOutput() const { return std::vector<T>(); };
		virtual void setInput(const std::vector<T>&) {};
	};
	
	// once Worker did his work and it's result obtained it must be destroyed
	class WorkflowWorker : public Worker<std::string>
	{
	protected:
		std::vector<std::string> input;
		std::vector<std::string> output;
		std::vector<std::string> args;
	public:
		WorkflowWorker(const std::vector<std::string>& args) : args(args) {};
		virtual void doWork() override { output = input; };
		virtual const std::vector <std::string> & getOutput() const override { return output; };
		virtual void setInput(const std::vector<std::string>& newInput) override { input = newInput; };
	};

	class FileReaderWorker : public WorkflowWorker
	{
	public:
		FileReaderWorker(const std::vector<std::string>& args) : WorkflowWorker(args) {};
		virtual void doWork() override;
	};

	class FileWriterWorker : public WorkflowWorker
	{
	public:
		FileWriterWorker(const std::vector<std::string>& args) : WorkflowWorker(args) {};
		virtual void doWork() override;
	};

	class FileDumpWorker : public FileWriterWorker
	{
	public:
		FileDumpWorker(const std::vector<std::string>& args) : FileWriterWorker(args) {};
		virtual void doWork() override;
	};

	class GrepWorker : public WorkflowWorker
	{
	public:
		GrepWorker(const std::vector<std::string>& args) : WorkflowWorker(args) {};
		virtual void doWork() override;
	};

	class SortWorker : public WorkflowWorker
	{
	public:
		SortWorker(const std::vector<std::string>& args) : WorkflowWorker(args) {};
		virtual void doWork() override;
	};

	class ReplaceWorker : public WorkflowWorker
	{
	public:
		ReplaceWorker(const std::vector<std::string>& args) : WorkflowWorker(args) {};
		virtual void doWork() override;
	};

}

#endif // !WORKER_H