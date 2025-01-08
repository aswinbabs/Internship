#include <iostream> 
#include <string> 
#include <vector> 
#include <algorithm>

//function to create a new account


class BankAccounts {
private:
	static int counter;
	int sl_no;
	std::string accname;
	int accnum;
	double accbalance;
	// Access specifier
public:
	// Member Functions()

	BankAccounts(std::string name, int num, double balance)
	{
		sl_no = ++counter;
		accname = name;
		accnum = num;
		accbalance = balance;
	}
	
	 
	
	 
	//function to display
	void displayDetails()
	{ 
		std::cout << "\nSl.No:" 
			      << sl_no << "\nAccount Holder's name:" 
			      << accname << "\nAccount Number:" 
			      << accnum << "\nAccount Balance:"
			      << accbalance; 
	}

	//function to get balance
	double getBalance()
	{
		return accbalance;
	}

	//function to get accnum
	int getAccountNumber()
	{
		return accnum;
	}
};

//initialize counter outside class
int BankAccounts::counter = 0;

//function to create new account
void addBankAccount(std::vector<BankAccounts> &v, std::string& name, int num, double balance)
{
	
	v.emplace_back(name, num, balance);

}

//function to sort accounts based on balance
void sortAccounts(std::vector<BankAccounts>&v)
{
   //using std::sort()
	std::sort(v.begin(), v.end(), [](BankAccounts& a, BankAccounts& b)
		{
			//Ascending order
			return a.getBalance() < b.getBalance();
		});

}

//function to search account using account number
void searchAccount(std::vector<BankAccounts>& v, int searchNum)
{
	auto it = std::find_if(v.begin(), v.end(), [searchNum](BankAccounts& num)
		{
			return num.getAccountNumber() == searchNum;
		});

	// If account found, display details, else show an error message
	if (it != v.end()) {
		std::cout << "\nAccount found:\n";
		it->displayDetails();  // Display account details
	}
	else {
		std::cout << "\nAccount not found.\n";  // Account not found
	}
}
int main()
{
	// Vector of class objects 
	std::vector<BankAccounts> v;
	int choice, tempNum,sNum;
	std::string tempName;
	double tempBalance;



	//prev data
	v.emplace_back("Aswin", 100000000, 55000); //Name ,  A/c number , Balance
	v.emplace_back("Aquinas", 100000001, 30000);
	
	

	while (1)
	{

	//Selects b/w choice
	std::cout << "\n1. Add new bank account\n";
	std::cout << "2. Display all the account details\n";
	std::cout << "3. Sort by balance\n";
	std::cout << "4. Search by account number\n";
	std::cin >> choice;
	
		switch (choice)
		{
		case 1:

			//new account
			std::cout << "\nFor new registration Enter Name,A/C number and Balance\n";
			std::cin >> tempName >> tempNum >> tempBalance;
			addBankAccount(v, tempName, tempNum, tempBalance);
			break;
		case 2:

			//List of all account details
			std::cout << "\nFinal list";

			for (int i = 0; i < v.size(); i++)
			{
				v[i].displayDetails();
			}
			break;
		case 3:

			//List of accounts sorted by balance
			sortAccounts(v);
			for (int i = 0; i < v.size(); i++)
			{
				v[i].displayDetails();
			}
			break;
		case 4:

			//Search using account number
			std::cout << "\nEnter the Account Number to search: ";
			std::cin >> sNum;
			searchAccount(v, sNum);
			break;
		default:

			//
			std::cout << "\nInvalid choice, please try again.\n";
			break;
		}
	}

	return 0;

}
