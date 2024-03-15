#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

class threadSafeQ
{
private:
    std::queue<int> rawQ;
    std::mutex m;

public:
    int& retrievedAndDelete()
    {
        int front_value = 0;
        m.lock();
        if (!rawQ.empty())
        {
            front_value = rawQ.front();
            rawQ.pop();
        }
        std::cout << "Retrieved and deleted: " << front_value << std::endl;
        m.unlock();

        return front_value;
    }

    void push(int val)
    {
        m.lock();
        rawQ.push(val);
        std::cout << "Pushed: " << val << std::endl;
        m.unlock();
    }

    threadSafeQ(std::queue <int> rawQ)
    {
        this -> rawQ = rawQ;
    }

    void printQueue(std::queue <int> q)
    {
        m.lock();
        while (!rawQ.empty())
        {
            std::cout << rawQ.front() << " ";
            rawQ.pop();
        }
        std::cout << std::endl;
        m.unlock();
    }
};

int main()
{
    std::queue <int> que;

    que.push(3);
    que.push(5);
    que.push(7);
    que.push(9);

   std::cout <<"3 5 7 9"<<std::endl;

    threadSafeQ A(que);

    std::thread popt(&threadSafeQ::retrievedAndDelete, &A);
    popt.join();

    std::thread pusht1(&threadSafeQ::push, &A, 2);
    if(pusht1.joinable()) pusht1.join();

    std::thread pusht2(&threadSafeQ::push, &A, 6);
    if(pusht2.joinable()) pusht2.join();

    A.printQueue(que);

    return 0;
}
