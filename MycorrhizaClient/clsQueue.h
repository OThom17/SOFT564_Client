#pragma once

#include <queue>
#include <mutex>

template <typename Type>

class clsQueue
{
private:
	std::queue<Type> internal_queue;
	std::mutex lock;

public:
	void push(Type val)
	{
		std::lock_guard<std::mutex> guard(this->lock);

		this->internal_queue.push(val);
	}

	Type pop()
	{
		std::lock_guard<std::mutex> guard(this->lock);

		Type tmp = this->internal_queue.front();
		this->internal_queue.pop();
		return tmp;
	}

	size_t size()
	{
		std::lock_guard<std::mutex> guard(this->lock);

		return this->internal_queue.size();
	}
};
