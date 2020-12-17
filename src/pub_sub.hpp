#ifndef PUB_SUB_HPP
#define PUB_SUB_HPP

#include <mutex>
#include <queue>

template<class T>
class TPubSubMQ {
private:
	std::queue<T> MessageQueue;
	std::mutex MQMutex;
public:
	explicit TPubSubMQ() noexcept : MessageQueue(), MQMutex() {}
	~TPubSubMQ() {}

	bool Empty() {
		MQMutex.lock();
		bool res = MessageQueue.empty();
		MQMutex.unlock();
		return res;
	}

	T Front() {
		MQMutex.lock();
		T elem = MessageQueue.front();
		MQMutex.unlock();
		return elem;
	}

	void Pop() {
		MQMutex.lock();
		MessageQueue.pop();
		MQMutex.unlock();
	}

	void Push(const T & message) {
		MQMutex.lock();
		MessageQueue.push(message);
		MQMutex.unlock();
	}
};

#endif /* PUB_SUB_HPP */