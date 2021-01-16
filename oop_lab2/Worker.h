#ifndef WORKER_H
#define WORKER_H

#include <vector>
#include <string>
#include "Block.h"

namespace labWorker
{
	template<class T> 
	class Worker
	{
	public:
		virtual void doWork() = 0;
	};
	
	// once Worker did his work and it's result obtained it must be destroyed
	class WorkflowWorker : public Worker<std::string>
	{
	protected:
		std::vector<std::string> input;
		std::vector<std::string> output;
		std::vector<std::string> args;
	public:
		WorkflowWorker() {};
		WorkflowWorker(const std::vector<std::string>& args) : args(args) {};
		virtual void doWork() override { output = input; };
		virtual const std::vector <std::string> & getOutput() const  { return output; };
		virtual void setInput(const std::vector<std::string>& newInput)  { input = newInput; };

		virtual labBlock::BlockType getBlockType() { return labBlock::BlockType::EMPTY_BLOCK; }
	};

	// add input-output check and rename class
	class WorkflowWorkerSmartPointer
	{
	private:
		WorkflowWorker *worker;
	public:
		WorkflowWorkerSmartPointer(WorkflowWorker * worker) : worker(worker) {};
		WorkflowWorkerSmartPointer(const WorkflowWorkerSmartPointer& other) { worker = new WorkflowWorker(*other.worker); };
		WorkflowWorkerSmartPointer(WorkflowWorkerSmartPointer&& other) { *this = std::move(other); };

		~WorkflowWorkerSmartPointer() { delete worker; }
		WorkflowWorker& operator*() const { return *worker; };
		WorkflowWorker* operator->() const { return worker; };
		WorkflowWorker* operator()() { return worker; };
		WorkflowWorkerSmartPointer& operator=(const WorkflowWorkerSmartPointer& other) { worker = new WorkflowWorker(*other); return *this; };
		//WorkflowWorkerSmartPointer& operator=(const WorkflowWorkerSmartPointer& other) = delete;
		WorkflowWorkerSmartPointer& operator=(WorkflowWorkerSmartPointer&& other) { worker = other.worker; other.worker = nullptr; return *this; };
	};
	
	class FileReaderWorker : public WorkflowWorker
	{
	public:
		FileReaderWorker(const std::vector<std::string>& args) : WorkflowWorker(args) {};
		virtual void doWork() override;
		virtual labBlock::BlockType getBlockType() { return labBlock::BlockType::READFILE; }
	};

	class FileWriterWorker : public WorkflowWorker
	{
	public:
		FileWriterWorker(const std::vector<std::string>& args) : WorkflowWorker(args) {};
		virtual void doWork() override;

		virtual labBlock::BlockType getBlockType() { return labBlock::BlockType::WRITEFILE; }
	};

	class FileDumpWorker : public FileWriterWorker
	{
	public:
		FileDumpWorker(const std::vector<std::string>& args) : FileWriterWorker(args) {};
		virtual void doWork() override;

		virtual labBlock::BlockType getBlockType() { return labBlock::BlockType::DUMP; }
	};

	class GrepWorker : public WorkflowWorker
	{
	public:
		GrepWorker(const std::vector<std::string>& args) : WorkflowWorker(args) {};
		virtual void doWork() override;

		virtual labBlock::BlockType getBlockType() { return labBlock::BlockType::GREP; }
	};

	class SortWorker : public WorkflowWorker
	{
	public:
		SortWorker(const std::vector<std::string>& args) : WorkflowWorker(args) {};
		virtual void doWork() override;

		virtual labBlock::BlockType getBlockType() { return labBlock::BlockType::SORT; }
	};

	class ReplaceWorker : public WorkflowWorker
	{
	public:
		ReplaceWorker(const std::vector<std::string>& args) : WorkflowWorker(args) {};
		virtual void doWork() override;

		virtual labBlock::BlockType getBlockType() { return labBlock::BlockType::REPLACE; }
	};

}

#endif // !WORKER_H