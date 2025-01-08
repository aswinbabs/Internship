// Assignment3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

template<typename T>
class TypeChecker
{
private:
    T size;
public:
    int printSize() 
    { 

        return sizeof(size);
    }
    
};
int main()
{


    TypeChecker<int> a;
    TypeChecker<bool> b;
    TypeChecker<double> c;
    TypeChecker<std::string> d;
    
    std::cout << a.printSize() << std::endl;
    std::cout << b.printSize() << std::endl;
    std::cout << c.printSize() << std::endl;
    std::cout << d.printSize() << std::endl;


     std::cin.get();

}

