#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <queue>
#include <stdexcept>

template <typename T>
class Queue {
public:
    void enqueue(const T& item) {
        queue.push(item);
    }

    T dequeue() {
        if (queue.empty()) {
            throw std::runtime_error("Очередь пуста");
        }
        T item = queue.front();
        queue.pop();
        return item;
    }

    bool isEmpty() const {
        return queue.empty();
    }

    size_t size() const {
        return queue.size();
    }

private:
    std::queue<T> queue;
};

#endif // QUEUE_HPP
