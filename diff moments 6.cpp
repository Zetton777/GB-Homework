#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <algorithm>

//потокобезопасная оболочка для объекта cout
class pcout : public std::ostringstream
{
public:
    pcout() = default;

    ~pcout()
    {
        std::lock_guard<std::mutex> guard(_mutexPrint);
        std::cout << this->str();
    }

private:
    static std::mutex _mutexPrint;
};

std::mutex pcout::_mutexPrint{};

//функция болванка для проверки pcout
void DoWork()
{
    for (size_t i = 1; i <= 10; i++)
    {
        pcout{} << "Thread ID = " << std::this_thread::get_id() << "\tDoWork\t" << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

//проверка числа на простое
int isPrime(int k)
{
    
    if (k <= 1)
        return 0;
    if (k == 2 || k == 3)
        return 1;

    if (k % 2 == 0 || k % 3 == 0)
        return 0;

    for (int i = 5; i * i <= k; i = i + 6)
        if (k % i == 0 || k % (i + 2) == 0)
            return 0;

    return 1;
}

//функция возвращаяющая i-ое простое число
int PrimeNum(int n)
{
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    pcout{} << "Thread ID = " << std::this_thread::get_id() << "=========== \tPrimeNum started \t=============" << std::endl;

    int i = 2;

    while (n > 0)
    {

        if (isPrime(i))
            n--;

        i++;  
    }
    i -= 1;     

    std::this_thread::sleep_for(std::chrono::seconds(3));
    pcout{} << "Thread ID = " << std::this_thread::get_id() << "=========== \tPrimeNum stopped \t=============" << std::endl;
    return i;
}


class House
{
public:
    std::vector<int> items;

    void HouseOwner()
    {
        srand(time(0));
        for (size_t i = 0; i < 10; i++)
        {
            items.insert(items.begin(), rand() % 100);
            
            pcout{} << "House owner added an item: " << items[0] << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(1));
            
        }
    }

    void Thief()
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        do 
        {
            std::sort(items.begin(), items.end());
            pcout{} << "Thief stole: " << items.back() << std::endl;
            items.pop_back();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } while (!items.empty());
    }
};




int main()
{
    
    std::thread T(DoWork);


    int result;
    std::thread th([&result]() {
        result = PrimeNum(1000000);
        });

    for (size_t i = 1; i <= 10; i++)
    {
        pcout{} << "Thread ID = " << std::this_thread::get_id() << "\tmain works\t" << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    pcout{} << "Prime number is: " << result << std::endl;

    House h1;
   
    std::thread owner([&]() {
        h1.HouseOwner();
        });


    std::thread thief([&]() {
        h1.Thief();
        });
    

    th.join();
    T.join();
    owner.join();
    thief.join();

   

    return 0;
}

