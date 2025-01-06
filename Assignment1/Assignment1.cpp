// 
#include <iostream> 


class BankAcounts {
private:
    std::string accname;
    int accnum;
    double accbalance;
    // Access specifier
public:

    // Member Functions()

    //Member function for Deposit
    void deposit(double balance) { accbalance += balance; }

    //Member function to withdraw money
    void withdraw(double balance) { if (balance <= accbalance) { accbalance -= balance; } else { std::cout << "\nSorry ! Insufficient balance"; } }



    BankAcounts(std::string name, int num, double balance) { accname = name; accnum = num; accbalance = balance; }

    void printdetails() { std::cout << "\nAccount Holder's name:" << accname << "\nAccount Number:" << accnum << "\nAccount Balance:" << accbalance; }
};
int main()
{
    //Declared using Contructors
    BankAcounts ac1("Aswin", 123123123, 10000);
    // accessing member details
    ac1.printdetails();

    std::cout << "\n";

    //deposit money 5000 
    ac1.deposit(5000);
    std::cout << "\nStatus after deposit";
    //status after deposit
    ac1.printdetails();

    std::cout << "\n";
    //withdraw money
    ac1.withdraw(30000);
    //status after money withdrawn
    std::cout << "\nStatus after money withdrawn";
    ac1.printdetails();
    return 0;
}




