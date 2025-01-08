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

**Program Code**

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
## 🎯 **Key Takeaways**
1. Demonstrates the use of constructors to initialize object attributes.
2. Implements encapsulation with private members and public methods.
3. Showcases OOP principles like data hiding, abstraction, and reusability.

## 3️⃣ **Assignment 2**

📝 Problem Statement
Use `std::vector` to store multiple bank accounts using the `BankAccount` class from Assignment 1.

### Reference
- [https://en.cppreference.com/w/cpp/container/vector](https://en.cppreference.com/w/cpp/container/vector)

### Functions to Implement

1. **Add a new bank account**
   void addBankAccount(int accountNumber, std::string accountHolderName, int balance);
2. **Display all account details**
   void displayDetails();
3. **Sort account details by account balance using `std::sort`**
   void sortAccounts();
4. **Search for a bank account detail by account number using `std::find_if` (Function Overloading)**
   void displayDetails(int accountNumber);

**Program Code**

```cpp
#include <iostream> 
#include <string> 
#include <vector> 
#include <algorithm>

class BankAccounts {
private:
    static int counter;
    int sl_no;
    std::string accname;
    int accnum;
    double accbalance;

public:
    BankAccounts(std::string name, int num, double balance) {
        sl_no = ++counter;
        accname = name;
        accnum = num;
        accbalance = balance;
    }

    void displayDetails() const {
        std::cout << "\nSl.No: " << sl_no 
                  << "\nAccount Holder's Name: " << accname 
                  << "\nAccount Number: " << accnum 
                  << "\nAccount Balance: " << accbalance << std::endl;
    }

    double getBalance() const { return accbalance; }
    int getAccountNumber() const { return accnum; }
};

int BankAccounts::counter = 0;

void addBankAccount(std::vector<BankAccounts>& v, std::string& name, int num, double balance) {
    v.emplace_back(name, num, balance);
}

void sortAccounts(std::vector<BankAccounts>& v) {
    std::sort(v.begin(), v.end(), [](const BankAccounts& a, const BankAccounts& b) {
        return a.getBalance() < b.getBalance();
    });
}

void searchAccount(const std::vector<BankAccounts>& v, int searchNum) {
    auto it = std::find_if(v.begin(), v.end(), [searchNum](const BankAccounts& acc) {
        return acc.getAccountNumber() == searchNum;
    });

    if (it != v.end()) {
        std::cout << "\nAccount found:\n";
        it->displayDetails();
    } else {
        std::cout << "\nAccount not found.\n";
    }
}

int main() {
    std::vector<BankAccounts> v;
    int choice, tempNum, sNum;
    std::string tempName;
    double tempBalance;

    v.emplace_back("Aswin", 100000000, 55000);
    v.emplace_back("Aquinas", 100000001, 30000);

    while (true) {
        std::cout << "\n1. Add new bank account\n";
        std::cout << "2. Display all the account details\n";
        std::cout << "3. Sort by balance\n";
        std::cout << "4. Search by account number\n";
        std::cout << "0. Exit\n";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "\nFor new registration, Enter Name, A/C number, and Balance:\n";
            std::cin >> tempName >> tempNum >> tempBalance;
            addBankAccount(v, tempName, tempNum, tempBalance);
            break;
        case 2:
            for (const auto& account : v) {
                account.displayDetails();
            }
            break;
        case 3:
            sortAccounts(v);
            for (const auto& account : v) {
                account.displayDetails();
            }
            break;
        case 4:
            std::cout << "\nEnter the Account Number to search: ";
            std::cin >> sNum;
            searchAccount(v, sNum);
            break;
        case 0:
            std::cout << "\nExiting the program. Goodbye!\n";
            return 0;
        default:
            std::cout << "\nInvalid choice, please try again.\n";
            break;
        }
    }
}
```

---

**Output**

```
1. Add new bank account
2. Display all the account details
3. Sort by balance
4. Search by account number
1
For new registration Enter Name,A/C number and Balance
Arjun 100000002 60000
1. Add new bank account
2. Display all the account details
3. Sort by balance
4. Search by account number
2
Final list
Sl.No:1
Account Holder's name:Aswin
Account Number:100000000
Account Balance:55000
Sl.No:2
Account Holder's name:Aquinas
Account Number:100000001
Account Balance:30000
Sl.No:3
Account Holder's name:Arjun
Account Number:100000002
Account Balance:60000
1. Add new bank account
2. Display all the account details
3. Sort by balance
4. Search by account number
3
Sl.No:2
Account Holder's name:Aquinas
Account Number:100000001
Account Balance:30000
Sl.No:1
Account Holder's name:Aswin
Account Number:100000000
Account Balance:55000
Sl.No:3
Account Holder's name:Arjun
Account Number:100000002
Account Balance:60000
1. Add new bank account
2. Display all the account details
3. Sort by balance
4. Search by account number
4
Enter the Account Number to search: 100000002
Account found:

Sl.No:3
Account Holder's name:Arjun
Account Number:100000002
Account Balance:60000
1. Add new bank account
2. Display all the account details
3. Sort by balance
4. Search by account number

```

---

## 🎯 **Key Takeaways**
1. Using `std::vector` provides dynamic storage for objects.
2. STL algorithms like `std::sort` and `std::find_if` simplify operations on collections.
3. Function overloading enhances code readability and flexibility.

## 4️⃣ **Assignment 3**

📝 Problem Statement
● Create a template class named TypeChecker with a single template parameter T.
● Add a member function called printSize to the class. This function should output the size
of the type T using sizeof.
● Instantiate the class with different types, such as int, float, double, and std::string.
● Call the printSize function for each instance to display the size of the corresponding
type.
● Example input :
int main()
{
TypeChecker<int> a;
TypeChecker<bool> b;
TypeChecker<double> c;
TypeChecker<std::string> d;
a.printSize ();
b.printSize ();
c.printSize ();
d.printSize ();
return 0;
}
● Output :
Size of variable : 4
Size of variable : 1
Size of variable : 8
Size of variable : 40
**Program Code**

```cpp
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
```
**Output**
```
4
1
8
32
```
## 🎯 **Key Takeaways**
1. Understanding Templates in C++
2. Usage of sizeof Operator
3. Instantiation of Template Classes
