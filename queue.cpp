#include <iostream>
#include <stack>
#include <algorithm>
#include <functional>

using namespace std;

template<typename T, typename F = function<T(T, T)>>
class AggQueue {
private:
    stack<pair<T, T>> inputStack, outputStack;
    F func;

    void pushWithAgg(stack<pair<T, T>>& stack, T value) {
        T currentAgg = stack.empty() ? value : func(value, stack.top().second);
        stack.push({value, currentAgg});
    }

    void transferIfNeeded() {
        if (outputStack.empty()) {
            while (!inputStack.empty()) {
                T element = inputStack.top().first;
                pushWithAgg(outputStack, element);
                inputStack.pop();
            }
        }
    }

public:
    AggQueue(F f) : func(f) {}

    void enqueue(T value) {
        pushWithAgg(inputStack, value);
    }

    void dequeue() {
        if (inputStack.empty() && outputStack.empty()) {
            cout << "Queue is empty! Cannot dequeue." << endl;
            return;
        }
        transferIfNeeded();
        outputStack.pop();
    }

    T getAgg() {
        if (inputStack.empty() && outputStack.empty()) {
            cout << "Queue is empty!" << endl;
            throw runtime_error("Queue is empty!");
        }
        if (inputStack.empty()) {
            return outputStack.top().second;
        }
        if (outputStack.empty()) {
            return inputStack.top().second;
        }
        return func(outputStack.top().second, inputStack.top().second); 
    }

    T peek() {
        transferIfNeeded();
        return outputStack.top().first;
    }

    bool isEmpty() {
        return inputStack.empty() && outputStack.empty();
    }
};

int main() {

    function<int(int, int)> minFunc =  [](int a, int b) { return min(a, b); };
    function<int(int, int)> maxFunc =  [](int a, int b) { return max(a, b); };
    function<int(int, int)> sumFunc =  [](int a, int b) { return a + b; };

    AggQueue<int, function<int(int, int)>> minQueue(minFunc);
    AggQueue<int, function<int(int, int)>> maxQueue(maxFunc);
    AggQueue<int, function<int(int, int)>> sumQueue(sumFunc);

    minQueue.enqueue(5);
    minQueue.enqueue(3);
    minQueue.enqueue(1);
    minQueue.enqueue(7);
    cout << "Minimum: " << minQueue.getAgg() << endl; // Ожидается 1

    maxQueue.enqueue(5);
    maxQueue.enqueue(3);
    maxQueue.enqueue(1);
    maxQueue.enqueue(7);
    cout << "Maximum: " << maxQueue.getAgg() << endl; // Ожидается 7


    sumQueue.enqueue(5);
    sumQueue.enqueue(3);
    sumQueue.enqueue(1);
    sumQueue.enqueue(7);
    sumQueue.dequeue();
    cout << "Sum: " << sumQueue.getAgg() << endl; // Ожидается 11
    return 0;
}
