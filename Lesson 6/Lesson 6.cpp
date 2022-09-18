#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <random>

//-----------1-----------
static std::mutex mtx_cout;

class pcout
{
private:
    std::lock_guard<std::mutex> lg;
public:
    pcout() : lg(mtx_cout) {};

    template<typename T>
    pcout& operator<<(const T& data)
    {
        std::cout << data;
        return *this;
    }

    pcout& operator<<(std::ostream& (*fp)(std::ostream&))                
    {                                                                    
        std::cout << fp;                                                 
        return *this;                                                    
    }                                                                    
};

void doSomething(int number)
{
    pcout() << "start thread " << number << std::endl;
    pcout() << "stop thread " << number << std::endl;
}

//-----------2-----------
bool isPrime(size_t num)
{
    size_t limit = num / 2;

    if (num > 2)
    {
        for (size_t i = 2; i <= limit; ++i)
        {
            if (num % i == 0)
                return false;
        }
    }

    return true;
}  

//-----------3-----------
std::mutex m;

void generate_things(std::vector<int>& v)
{
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_int_distribution<int> urd(0, 1000);

    std::lock_guard<std::mutex> lg(m);

    std::generate(v.begin(), v.end(), [&]()
        {
            return urd(mersenne);
        });
}

void pop_thing(std::vector<int>& v)
{
    std::lock_guard<std::mutex> lg(m);

    std::cout << *std::max_element(v.begin(), v.end()) << std::endl;
}

int main()
{
    //-----------1-----------
    std::thread th1(doSomething, 1);
    std::thread th2(doSomething, 2);
    std::thread th3(doSomething, 3);
    th1.join();
    th2.join();
    th3.join();

    //-----------2-----------
    size_t maxValue = 500000;
    size_t counThreads = 30;
    std::vector<size_t> answers;
    std::mutex m;

    for (size_t i = 0; i < maxValue;)
    {
        std::vector<std::thread> v;

        for (size_t j = i + counThreads; i <= j; ++i)
            v.push_back(std::thread([=, &m, &answers](){if (isPrime(i)){std::lock_guard<std::mutex> lg(m);answers.push_back(i);}}));

        for (auto& t : v)
            t.join();
    }

    //-----------3-----------
    std::vector<int> v(100);
    size_t count = 100;

    while (count--)
    {
        std::thread owner([&]()
            {
                generate_things(v);
            });
        std::thread thief([&]()
            {
                pop_thing(v);
            });

        owner.join();
        thief.join();
    }
}