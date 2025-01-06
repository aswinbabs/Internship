# C++ Concepts and Assignments

This document is designed to provide a clear understanding of **Object-Oriented Programming (OOP)** principles in C++, supported with practical examples and assignments.

---

## 1️⃣ **Concepts**

### 📚 **Core OOP Principles**

1. **Object**: Combines **data** and **functions** into a single unit.
2. **Class**: A blueprint for creating objects, defining properties (data members) and behaviors (member functions).
3. **Abstraction**: Hides unnecessary details and exposes only the essential features of an object.
4. **Encapsulation**: Combines data and methods in a class while restricting direct access to some components.
5. **Inheritance**: Enables new classes (derived) to inherit properties and behaviors from existing classes (base).
6. **Polymorphism**: Allows methods or operators to operate in different contexts (e.g., function overloading, operator overloading).

---

### 🧱 **C++ Class Structure**

A class combines data and methods to encapsulate the logic and state of an object.

#### Syntax:
------------------
```cpp 
class ClassName  { 
    access_specifier: 
    // Members (variables and functions) 
}
```

### 🚀 Access Specifiers

- **Public**: Accessible from outside the class.
- **Private**: Accessible only within the class.
- **Protected**: Accessible within the class and derived classes.

> **Note**: By default, members are `private` unless specified otherwise.

🌟 Key Member Functions
- Constructor: A special function called when an object is created to initialize it.
- Destructor: Cleans up resources when the object goes out of scope.
- Copy Constructor: Creates a copy of an existing object.

```cpp class Geeks {
public:
    void setName(std::string name);
    void display();
private:
    std::string name;
};

void Geeks::setName(std::string name) {
    this->name = name;
}

void Geeks::display() {
    std::cout << "Geek Name: " << name << std::endl;
}
```
## 2️⃣ **Assignment 1**
📝 Problem Statement
Write a C++ program to simulate a simple Bank Account system using OOP concepts. The program should include:

Class Definition: BankAcounts with attributes for account name, account number, and balance.
Functionalities:
Deposit money.
Withdraw money.
Print account details.

```cpp
#include <iostream>

class BankAcounts {
private:
    std::string accname;
    int accnum;
    double accbalance;

public:
    // Constructor
    BankAcounts(std::string name, int num, double balance) {
        accname = name;
        accnum = num;
        accbalance = balance;
    }

    // Member function for deposit
    void deposit(double balance) {
        accbalance += balance;
    }

    // Member function for withdrawal
    void withdraw(double balance) {
        if (balance <= accbalance) {
            accbalance -= balance;
        } else {
            std::cout << "\nSorry! Insufficient balance";
        }
    }

    // Print account details
    void printdetails() {
        std::cout << "\nAccount Holder's name: " << accname
                  << "\nAccount Number: " << accnum
                  << "\nAccount Balance: " << accbalance;
    }
};

int main() {
    // Declared using constructor
    BankAcounts ac1("Aswin", 123123123, 10000);

    // Print initial details
    ac1.printdetails();

    std::cout << "\n";

    // Deposit money (5000)
    ac1.deposit(5000);
    std::cout << "\nStatus after deposit";
    ac1.printdetails();

    std::cout << "\n";

    // Withdraw money (30000)
    ac1.withdraw(30000);
    std::cout << "\nStatus after money withdrawn";
    ac1.printdetails();

    return 0;
}
```
**Output**

```
Account Holder's name: Aswin
Account Number: 123123123
Account Balance: 10000

Status after deposit
Account Holder's name: Aswin
Account Number: 123123123
Account Balance: 15000

Sorry! Insufficient balance
Status after money withdrawn
Account Holder's name: Aswin
Account Number: 123123123
Account Balance: 15000
```
##🎯 **Key Takeaways from Assignment1**
- Demonstrates the use of constructors to initialize object attributes.
- Implements encapsulation with private members and public methods.
- Showcases OOP principles like data hiding, abstraction, and reusability.

