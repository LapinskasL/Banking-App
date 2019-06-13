//Name:             Lukas Lapinskas
//Date:             12-04-2017
//Assignment:       Final
//Summary:          Online Banking Program

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

fstream ofs("Lapinskas_final.txt", ios::out);

// global variables used for header and footer
string myname	= "Lukas Lapinskas";
string dat		= "12/04/2017";
string crnum	= "CIS2541-003";
string lab		= "Final";
string summ		= "Online Banking Program";
string eofl		= "EOF message";

// formatting variables
string lnu	= "________________________________________"
			  "________________________________________";
string pipe = "|";
string lnu2 = "|_______________________________________"
			  "_______________________________________|";
string lnu3 = "|_____________|____________|____________"
			  "|____________|____________|____________|";

// menu2 function text variables
string menu2pick = "Select an action: ";

// character variables used in various functions
char tab = '\t';
char colon = ':';
char dollsymb = '$';
char sp = ' ';

//*********************************************************
//* Definition of variable dollars.                       *
//* This string dollars variable temporarily holds the    *
//* value that the user wants to deposit/withdraw/pay. It *
//* is then checked by goodnum function if the string can *
//* be converted to a double data type. The variable is   *
//* also used in printing the transactions table.         *
//*********************************************************
string dollars;

// vectors used for printing table and its contents
vector<string> dates = { "Jan 1, 2017", "Feb 1, 2017", "Mar 1, 2017",
						 "Apr 1, 2017", "May 1, 2017", "Jun 1, 2017",
						 "Jul 1, 2017", "Aug 1, 2017", "Sep 1, 2017" };

//*********************************************************
//* Definition of vector tranaday (transactions a day).   *
//* This vector contains the predetremined number of      *
//* transactions (based on the table in final pfd) a day. *
//*********************************************************
vector<int> tranaday = { 2, 2, 2, 1, 2, 2, 2, 3, 1 };

//*********************************************************
//* Vector colnames is used in printing table column      *
//* names in ofsprnt0 function, but elements 1-5 are also *
//* are reused in many other functions.                   *
//*********************************************************
vector<string> colnames = { "Dates", "Checking", "Savings", 
							"IRA", "Mortgage", "Credit Card" };

//*********************************************************
//* vectpos is used to increment vector tranaday in       *
//* ofsprnt1 function.                                    *
//*********************************************************
int vectpos = 0;

//*********************************************************
//* counter is used in ofsprnt1 function with each element*
//* of tranaday. It is reset when a day passes (based on  *
//* if value in tranaday[vectpos] == counter.)            *
//*********************************************************
int counter = 0;

// colpos elements act as temporary holders for the transactions
vector<string> colpos = { " ", " ", " ", " ", " " };

// text variable with error message for goodnum function
string invalid = "ERROR! Must be a positive dollar value number: ";

// text variable for Payment and Transfer class declarations
string initamt = "starting amount: ";

// text variables used in dep function
string damt = "Deposit amount: ",
	   depd = "dep";

// text variables used in wdl function
string sempty = "ERROR! The account is empty!",
	     wamt = "Withdraw amount: ",
	   nomore = "ERROR! Withdrawal must be no more than the balance($",
	     wdld = "wdl",
	 rparenth = ")";

// text variables in makepay function
string paidoff = " has been already paid off!",
		payamt = "Payment amount: ",
	   toohigh = "ERROR! Payment must be no more than $",
		 payed = "pay";

// menu2 function text variables
string line1_0 = "Deposit and Withdrawal",
	   line1_1 = "1. Deposit",
	   line1_2 = "2. Withdraw",
	   line1_3 = "3. Main Menu";

// menu function text variables
string line0 = "Accounts",
	   line1 = "1. Checking",
	   line2 = "2. Savings",
	   line3 = "3. IRA",
	   line4 = "4. Make Mortgage Payment",
	   line5 = "5. Make Credit Card Payment",
	   line6 = "6. Quit";

// menu2sel function message variables
string menupick = "Please select an account and press enter: ",
		 errmsg = "ERROR! Invalid choice. Choose option 1 through",
		quitmsg = "The program has quit. ";

//*********************************************************
//* Definition of function dotposok (dot position okay).  *
//* Accepts a passed by value parameter num, which is     *
//* received from function isdouble. This function checks *
//* if the amount of digits after the dot in variable num *
//* is 2 or less (to represent cent value). If it's 2 or  *
//* less, it returns true. If more than 2 - false. This   *
//* function is not called if argument has no dot.        *
//*********************************************************
bool dotposok(string num)
{
	const int size = num.size();

	if ((num[0] == '.' && size == 2) ||
		num[size - 1] == '.' ||
		num[size - 2] == '.' ||
		num[size - 3] == '.')
	{
		return true;
	}
	else
	{
		return false;
	}
}

//*********************************************************
//* Definition of function isdouble.                      *
//* Accepts a passed by value parameter dollars, which is *
//* checked to see if it can be converted to a double     *
//* data type (only digits from 0-9 and a single dot (.)  *
//* is allowed in the value.) It also checks to see if    *
//* value has 2 or less significant digits after the dot. *
//* If the value can be converted, the function returns   *
//* true, if it can't - false.                            *
//*********************************************************
bool isdouble(string dollars)
{
	bool isnum = false;

	// dotcount holds the number of dots in dollars
	int dotcount = 0;
	int i = 0;
	while (i < dollars.size())
	{
		if (dollars[i] == '.')
		{
			dotcount++;
		}
		else if (isdigit(dollars[i]))
		{
			isnum = true;
		}
		else
		{
			return false;
		}

		if (dotcount > 1)
		{
			return false;
		}
		i = i + 1;
	}
	if (dotcount == 1 && dollars.size() == 1)
	{
		return false;
	}

	// this if statement checks if the double has 2 or less
	// digits after the dot
	if (dotcount == 1)
	{
		isnum = dotposok(dollars);
	}

	return isnum;
}

//*********************************************************
//* Definition of function goodnum (good number).         *
//* Accepts a passed by value parameter money, which is   *
//* passed to isdouble() function and checked if it's in  *
//* an appropriate format. If isdouble() returns true,    *
//* the goodnum function converts money variable to a     *
//* double and returns it. If false, the user is asked to *
//* enter a new value.                                    *
//*********************************************************
double goodnum(string money)
{
	while (isdouble(money) == false)
	{
		cout << tab << invalid;
		getline(cin, money);
	}
	dollars = money;

	// dol holds money value converted to a double
	double dol = stod(money);

	return dol;
}

//*********************************************************
//* Declaration of class Transfer.                        *
//* This class is used to create an object for each of    *
//* these accounts: checking, savings, ira. This is       *
//* because they have different functionality than        *
//* the mortgage and credit card accounts. This class     *
//* contains four inline member functions.                *
//*********************************************************
class Transfer
{
private:
	double balance;

public:

	//*********************************************************
	//* Definition of inline member function startbal.        *
	//* Accepts a pass by value parameter called type, which  *
	//* contains the name of the account. The function        *
	//* requests a starting balance of the "type" account and *
	//* validates the user's input. Once the user enters a    *
	//* correct value, it is stored in the balance variable.  *
	//*********************************************************
	void startbal(string type)
	{
		cout << tab << type << sp << initamt << dollsymb;
		getline(cin, dollars);

		balance = goodnum(dollars);
		cout << endl;
	}

	//*********************************************************
	//* Definition of inline member function deposit.         *
	//* Accepts a passed by value dollars parameter, which is *
	//* added to the balance variable.                        *
	//*********************************************************
	void deposit(double dollars)
	{
		balance += dollars;
	}

	//*********************************************************
	//* Definition of inline member function wiwthdraw.       *
	//* Accepts a passed by value dollars parameter, which is *
	//* subtracted from the balance variable.                 *
	//*********************************************************
	void withdraw(double dollars)
	{
		balance -= dollars;
	}

	//*********************************************************
	//* Definition of inline member function getbal.          *
	//* Returns the value in balance variable.                *
	//*********************************************************
	double getbal()
	{
		return balance;
	}

};

//*********************************************************
//* Declaration of class Payment.                         *
//* This class is used to create an object for each of    *
//* these accounts: mortgage and credit card. This is     *
//* because they have different functionality than        *
//* the checking, savings, ira accounts. This class       *
//* contains three inline member functions.               *
//*********************************************************
class Payment
{
private:
	double amount;

public:

	//*********************************************************
	//* Definition of inline member function startamt.        *
	//* Accepts a pass by value parameter called type, which  *
	//* contains the name of the account. The function        *
	//* requests a starting amount of the "type" account and  *
	//* validates the user's input. Once the user enters a    *
	//* correct value, it is stored in the amount variable.   *
	//*********************************************************
	void startamt(string type)
	{
		cout << tab << type << sp << initamt << dollsymb;
		getline(cin, dollars);

		amount = goodnum(dollars);
		cout << endl;
	}

	//*********************************************************
	//* Definition of inline member function pay.             *
	//* Accepts a passed by value dollars parameter, which is *
	//* subtracted from the amount variable.                  *
	//*********************************************************
	void pay(double dollars)
	{
			amount -= dollars;
	}

	//*********************************************************
	//* Definition of inline member function getamt.          *
	//* Returns the value in amount variable.                 *
	//*********************************************************
	double getamt()
	{
		return amount;
	}
};

//*********************************************************
//* Declaration of structure Account.                     *
//* This structure contains the definitions of all of the *
//* Transfer and Payment objects. The structure provides  *
//* the capability to create multiple user accounts that  *
//* contain checking, savings, ira, mortgage, and credit  *
//* card "bank accounts". In this program, however, only  *
//* one account object is defined and processed.          *
//*********************************************************
struct Account
{
	Transfer checking;
	Transfer savings;
	Transfer ira;

	Payment mortgage;
	Payment cc;
};

// definition of Account object myacc
Account myacc;



//*********************************************************
//* Definition of function getbals (get balances).        *
//* Gets initial balances for the 3 Transfer and 2        *
//* Payment objects.                                      *
//*********************************************************
void getbals()
{
	cout << string(3, '\n');
	myacc.checking.startbal(colnames[1]);
	myacc.savings.startbal(colnames[2]);
	myacc.ira.startbal(colnames[3]);
	myacc.mortgage.startamt(colnames[4]);
	myacc.cc.startamt(colnames[5]);
}

//*******************************
//* header functions, this      *
//* displays the header         *
//* information for the program *
//*******************************
void hdr()
{
	ofs << lnu << endl;
	ofs << pipe
		<< left << setw(20) << myname
		<< left << setw(20) << dat
		<< left << setw(20) << crnum
		<< left << setw(18) << lab
		<< pipe << endl;
}

//******************************
//* summary of what is printed *
//* in this program            *
//******************************
void ftr()
{
	ofs << lnu2 << endl;
	ofs << pipe << left << setw(78) << summ << pipe << endl;
}

//***********************
//* end of file message *
//***********************
void eof()
{
	ofs << lnu2 << endl;
	ofs << pipe
		<< left << setw(17) << eofl
		<< left << setw(19) << myname
		<< left << setw(12) << dat
		<< left << setw(15) << crnum
		<< left << setw(15) << lab
		<< pipe << endl;
	ofs << lnu2 << endl;
	ofs << endl;
}

//*****************************
//* creates a line with pipes *
//*****************************
void pipefunc()
{
	ofs << lnu2 << endl;
}

//*********************************************************
//* Definition of function title.                         *
//* Defines a title variable, formats and prints its      *
//* contents.                                             *
//*********************************************************
void title()
{
	string title = "Transaction History";

	ofs << setw(33) << pipe << left
		<< setw(32)
		<< setw(46) << title
		<< pipe;
	ofs << endl;
}

//*********************************************************
//* Definition of function ofsprnt0.                      *
//* Prints the row of column names in text file table.    *
//* This is the first row that is printed.                *
//*********************************************************
void ofsprnt0()
{
	ofs << pipe << setw(13) << colnames[0];

	for (int i = 1; i < 6; i++)
	{
		ofs << pipe << setw(12) << colnames[i];
	}
	ofs << pipe << endl;
	ofs << lnu3 << endl;
}

//*********************************************************
//* Definition of function ofsprnt1.                      *
//* Prints a row containing predefined date and a         *
//* predefined number of transactions per date (Example:  *
//* For the first two transactions, a day passes. But the *
//* fourth day passes only after 1 transaction.) This     *
//* was based one the table provided in the final pdf.    *
//* After the row is printed, the colpos elements are     *
//* emptied out. This function is called after each       *
//* transaction.                                          *
//*********************************************************
void ofsprnt1()
{
	counter++;

	if (counter == tranaday[vectpos] && vectpos <= 8)
	{
		ofs << left << pipe << setw(13) << dates[vectpos];

		for (int i = 0; i < 5; i++)
		{
			ofs << pipe << setw(12) << colpos[i];
		}
		ofs << pipe << endl;
		ofs << lnu3 << endl;

		counter = 0;
		vectpos++;
		colpos = { " ", " ", " ", " ", " " };
	}
}

//*********************************************************
//*				FOR dep AND wdl FUNCTIONS                 *
//*                                                       *
//* Accepts two parameter variables: type and pos. type   *
//* is passed by reference because it needs to work with  *
//* the original object. pos is passed by value and is    *
//* used to pic colpos element (colpos[pos]), in which    *
//* dollars variable (the dollar amount being deposited   *
//* or withdrawn) is put into that element.               *
//* The functions ask user for input value, check if      *
//* value is in the acceptable format using               *
//* function goodnum, and that value is deposited or      *
//* withdrawn.                                            *
//*                                                       *
//*********************************************************

//*********************************************************
//* Definition of function dep (deposit).                 *
//*********************************************************
void dep(Transfer &type, int pos)
{
	cout << tab << damt;
	getline(cin, dollars);

	// dol local variable to hold goodnum return value
	double dol = goodnum(dollars);

	// transaction is not recorded if value is 0
	if (dollars == "0")
	{
		return;
	}

	type.deposit(dol);
	
	// strdol holds converted dol value for input into colpos[pos]
	string strdol = to_string(dol);
	colpos[pos] = dollsymb + dollars + depd;

	ofsprnt1();
}

//*********************************************************
//* Definition of function wdl (withdrawal).              *
//* In addition to previous explanation, it displays a    *
//* message telling the user the account is empty if the  *
//* user attempts to withdraw with an empty balance.      *
//* Also, displays an error message if user attempts to   *
//* withdraw more than the balance.                       *
//*********************************************************
void wdl(Transfer &type, int pos)
{
	if (type.getbal() == 0)
	{
		cout << tab << sempty << endl;
		system("pause");
		system("cls");
		return;
	}

	cout << tab << wamt;
	getline(cin, dollars);

	double dol = goodnum(dollars);
	while (type.getbal() < dol)
	{
		cout << tab << nomore
			 << type.getbal() << rparenth << colon << sp;
		getline(cin, dollars);
		dol = goodnum(dollars);
	}

	// transaction not recorded if value is 0
	if (dollars == "0")
	{
		return;
	}
	type.withdraw(dol);
	colpos[pos] = dollsymb + dollars + wdld;

	ofsprnt1();
}

//*********************************************************
//* Definition of function onedigit.                      *
//* Accepts two passwd by value parameter variables: str  *
//* and lastcase. str is user's input when selecting a    *
//* menu option, and lastcase contains the case number    *
//* that exits the menu. This function handles all the    *
//* invalid input for menus. This means that switch/case  *
//* statements do not need the "default" statement.       *
//*********************************************************
string onedigit(string str, int lastcase)
{
	while ( str.size() != 1 || !isdigit(str[0]) ||
		      stoi(str) < 0 || stoi(str) > lastcase )
	{
		cout << tab << errmsg << sp << lastcase << colon << sp;
		getline(cin, str);
	}
	return str;
}

//*********************************************************
//* Displays a formatted menu in console containing       *
//* deposit and withdrawal options, and a main menu       *
//* option. Parameter acc contains name of the account.   *
//*********************************************************
void menu2(string acc)
{
	cout << string(3, '\n');
	cout << string(6, '\t') << acc << sp << line1_0 << string(2, '\n');
	cout << string(6, '\t') << line1_1 << endl
		 << string(6, '\t') << line1_2 << endl
		 << string(6, '\t') << line1_3 << string(2, '\n');
}

//*********************************************************
//* Definition of function showbals (show balances).      *
//* This function shows formatted balances of all the     *
//* accounts. After the initial balances are entered by   *
//* the user, the balances are always displayed while the *
//* program runs and are updated after each transaction.  *
//*********************************************************
void showbals()
{
	cout << fixed << setprecision(2);
	cout << string(3, '\n');

	cout << string(1, '\t') << setw(13) << left 
		 << colnames[1] + colon << dollsymb << myacc.checking.getbal() << endl;
	cout << string(1, '\t') << setw(13) << left 
		 << colnames[2] + colon << dollsymb << myacc.savings.getbal() << endl;
	cout << string(1, '\t') << setw(13) << left 
		 << colnames[3] + colon << dollsymb << myacc.ira.getbal() << endl;
	cout << string(1, '\t') << setw(13) << left 
		 << colnames[4] + colon << dollsymb << myacc.mortgage.getamt() << endl;
	cout << string(1, '\t') << setw(13) << left 
		 << colnames[5] + colon << dollsymb << myacc.cc.getamt() << endl;
}

//*********************************************************
//* Definition of function menu2sel (menu 2 selection).   *
//* Accepts three parameters: acc, type, and pos. acc     *
//* holds the name of the account, which is passed to     *
//* menu2 function. type is passed by reference to work   *
//* with the original object. pos contains the column     *
//* position of account's transaction. type and pos are   *
//* passed to dep and wdl functions.
//* Uses case/switch statements, enclosed in a do-while   *
//* loop, to select one of the menu options. Cases 1 and  *
//* 2 allow the user to deposit or withdraw. Case 3       *
//* returns to main menu.                                 *
//*********************************************************
void menu2sel(string acc, Transfer &type, int pos)
{
	string option;

	do
	{
		system("cls");

		showbals();

		menu2(acc);

		cout << tab << menu2pick;
		getline(cin, option);
		option = onedigit(option, 3);
		cout << endl;

		switch (stoi(option))
		{
		case 1: dep(type, pos);
			break;
		case 2: wdl(type, pos);
			break;
		case 3: system("cls");
			return;
		}
	} while (stoi(option) != 3);
}

//*********************************************************
//* Displays a formatted menu in console containing list  *
//* of accounts (checking, savings....) and quit option.  *
//*********************************************************
void menu()
{
	cout << string(3, '\n');
	cout << string(6, '\t') << line0 << string(2, '\n');
	cout << string(6, '\t') << line1 << endl
		 << string(6, '\t') << line2 << endl
		 << string(6, '\t') << line3 << endl
		 << string(6, '\t') << line4 << endl
		 << string(6, '\t') << line5 << endl
		 << string(6, '\t') << line6 << string(2, '\n');
}

//*********************************************************
//* Definition of function makepay (make payment).        *
//* Accepts three parameters: type, acc, and pos. type    *
//* is passed by reference to work with the original      *
//* object. acc holds the name of the account. pos        *
//* contains the column position of the account in the    *
//* txt file transaction table (pos is used the same way  *
//* in dep and wdl functions). This function validates    *
//* input when user attemts to make a payment. Once it's  *
//* valid, it makes the payment.                          *
//*********************************************************
void makepay(Payment &type, string acc, int pos)
{
	//displays message if account is paid off
	if (type.getamt() == 0)
	{
		cout << tab << acc << paidoff << endl;
		system("pause");
		system("cls");
		return;
	}

	cout << tab << payamt;
	getline(cin, dollars);
	double dol = goodnum(dollars);

	// checks if input is more than amount that can be paid
	while (type.getamt() < dol)
	{
		cout << tab << toohigh
			<< type.getamt() << colon << sp;
		getline(cin, dollars);
		dol = goodnum(dollars);
	}
	
	// transaction is not recorded if input is 0
	if (dollars == "0")
	{
		system("cls");
		return;
	}
	type.pay(dol);
	colpos[pos] = dollsymb + dollars + payed;
	system("cls");

	ofsprnt1();
}

//*********************************************************
//* Definition of function menusel (menu selection).      *
//* Uses case/switch statements, enclosed in a do-while   *
//* loop, to select one of the menu options. Cases 1 - 5  *
//* select an account, and case 6 quits the program.      *
//*********************************************************
void menusel()
{
	string option;

	do
	{
		showbals();

		menu();

		cout << tab << menupick;
		getline(cin, option);
		option = onedigit(option, 6);
		cout << endl;

		switch (stoi(option))
		{
		case 1: menu2sel(colnames[1], myacc.checking, 0);
			break;
		case 2: menu2sel(colnames[2], myacc.savings, 1);
			break;
		case 3: menu2sel(colnames[3], myacc.ira, 2);
			break;
		case 4: makepay(myacc.mortgage, colnames[4], 3);
			break;
		case 5: makepay(myacc.cc, colnames[5], 4);
			break;
		case 6: cout << tab << quitmsg << endl;
			break;
		}
	} while (stoi(option) != 6);
}

//*****************
//* main function *
//*****************
int main()
{
	getbals();

	system("cls");

	hdr();
	pipefunc();
	pipefunc();
	title();
	pipefunc();

	ofsprnt0();

	menusel();

	ftr();
	pipefunc();
	eof();

	ofs.close();
	system("pause");
	return 0;
}