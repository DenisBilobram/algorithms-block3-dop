#include <iostream>
#include <stack>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <cassert>

template<typename T, typename F>
class AggregateQueue {
private:
    std::stack<T> inputStack, outputStack;
    std::stack<T> inputAggStack, outputAggStack;
    F func;

    void pushWithAgg(std::stack<T>& stack, std::stack<T>& aggStack, T value) {
        T currentAgg = aggStack.empty() ? value : func(value, aggStack.top());
        stack.push(value);
        aggStack.push(currentAgg);
    }

    void transferIfNeeded() {
        if (outputStack.empty()) {
            while (!inputStack.empty()) {
                T element = inputStack.top();
                inputStack.pop();
                pushWithAgg(outputStack, outputAggStack, element);
                inputAggStack.pop();
            }
        }
    }

public:
    AggregateQueue(F f) : func(f) {}

    void enqueue(T value) {
        pushWithAgg(inputStack, inputAggStack, value);
    }

    void dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty! Cannot dequeue.");
        }
        transferIfNeeded();
        outputStack.pop();
        outputAggStack.pop();
    }

    T getAgg() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty!");
        }
        if (inputStack.empty()) {
            return outputAggStack.top();
        }
        if (outputStack.empty()) {
            return inputAggStack.top();
        }
        return func(outputAggStack.top(), inputAggStack.top());
    }

    T peek() {
        transferIfNeeded();
        return outputStack.top();
    }

    bool isEmpty() {
        return inputStack.empty() && outputStack.empty();
    }
};

int main() {
    auto minFunc = [](int a, int b) { return std::min(a, b); };
    auto maxFunc = [](int a, int b) { return std::max(a, b); };
    auto sumFunc = [](int a, int b) { return a + b; };

    AggregateQueue<int, decltype(minFunc)> minQueue(minFunc);
    AggregateQueue<int, decltype(maxFunc)> maxQueue(maxFunc);
    AggregateQueue<int, decltype(sumFunc)> sumQueue(sumFunc);

    minQueue.enqueue(5);
    minQueue.enqueue(3);
    minQueue.enqueue(1);
    minQueue.enqueue(7);
    assert(minQueue.getAgg() == 1);

    maxQueue.enqueue(5);
    maxQueue.enqueue(3);
    maxQueue.enqueue(1);
    maxQueue.enqueue(7);
    assert(maxQueue.getAgg() == 7);

    sumQueue.enqueue(5);
    sumQueue.enqueue(3);
    sumQueue.enqueue(1);
    sumQueue.enqueue(7);
    sumQueue.dequeue();
    assert(sumQueue.getAgg() == 11);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
