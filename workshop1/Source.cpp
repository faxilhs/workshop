//header files
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>   
#include <conio.h>
#include <cstdlib>
#include <mysql.h>
using namespace std;


//start db connection
int qstate;
MYSQL* conn;
MYSQL_RES* res;
MYSQL_ROW row;

class db_response
{
public:
	static void ConnectionFunction()
	{
		conn = mysql_init(0);
		if (conn)
			cout << "Database Connected" << endl;
		else
			cout << "Failed To Connect!" << endl;

		conn = mysql_real_connect(conn, "localhost", "root", "", "brs", 3306, NULL, 0);
		if (conn)
			cout << "Database Connected To MySql" << endl;
		else
			cout << "Failed To Connect!" << endl;
	}
};

//user register account
void Register() {
	system("cls");

	char citizenship;
	string UsrIc, UsrPassport, Country, name, email, phone, password, rePsw;

	cout << "+---------------------------------------------------------+" << endl;
	cout << "                  REGISTER A USER ACCOUNT                  " << endl;
	cout << "+---------------------------------------------------------+" << endl;
	cout << "Please fill in the following information.\n";

	cout << "1. Citizenship" << endl;
	cout << "   a. Citizen" << endl;
	cout << "   b. Permanent-citizen" << endl;
	cout << "   c. Non-citizen" << endl;
	cout << "Please enter your option: ";
	cin >> citizenship;

	cin.ignore();
	do {
		cout << "\n2. Identification Card Number (without '-'): ";
		getline(cin, UsrIc);
	} while (UsrIc.size() != 12);
	cout << "3. Passport Number (optional for citizen): ";
	getline(cin, UsrPassport);
	cout << "4. Country: ";
	getline(cin, Country);
	cout << "5. Full Name: ";
	getline(cin, name);
	cout << "6. Email: ";
	getline(cin, email);
	cout << "7. Phone Number (e.g: 012-3456789): ";
	getline(cin, phone);
	do {
		cout << "8. Password" << endl
			<< "In order to protect your account, make sure your password fulfill the following" << endl;
		cout << "      - A lowercase letter" << endl << "      -A capital letter" << endl;
		cout << "      -A number" << endl << "      -In between 8 to 12 characters"
			<< endl << "Please enter you password: ";

		char ch;
		while ((ch = _getch()) != 13)
		{
			password += ch;
			cout << '*';
		}

		cout << "\n9. Reconfirm your password: ";


		while ((ch = _getch()) != 13)
		{
			rePsw += ch;
			cout << '*';
		}

		if (rePsw != password)
		{
			cout << "\n\n!!!The password is different. Please enter again!!!" << endl << endl;
		}
	} while (password != rePsw);

	string checkUser_query = "SELECT * from users WHERE User_ICNo = '" + UsrIc + "'";
	const char* cu = checkUser_query.c_str();
	qstate = mysql_query(conn, cu);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1)
		{
			cout << "You have created an account. Press enter to login.";
			system("pause");
			UsrLog();
		}
		else
		{
			string insertUser_query = "INSERT into users (User_ICNo, Citizenship, Passport_No, Country, User_Name, User_Email, User_MobileNo, User_Psw) values ('"
				+ UsrIc + "', '" + citizenship + "', '" + UsrPassport + "', '" + Country + "', '" + name + "', '" + email + "', '" + phone + "', '" + password + "')";
			const char* q = insertUser_query.c_str();
			qstate = mysql_query(conn, q);
		}
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
	cout << "\nSuccessfully register! Press enter to login!\n";
}

void Login()
{
	system("cls");

	int choose;
	cout << "+----------------------------------------------------------------+" << endl;
	cout << "|                           LOGIN AS?                            |" << endl;
	cout << "+----------------------------------------------------------------+" << endl;
	cout << "1. User" << endl << "2. Staff" << endl << "3. Admin" << endl << "4. Exit";
	cout << "\n\nPlease select your option: ";
	cin >> choose;
	if (choose == 1)
	{
		UsrLog();
	}
	else if (choose == 2)
	{
		staffLog();
	}
	else if (choose == 3)
	{
		adminLog();
	}
	else if (choose == 4)
	{
		//back to main page
		cout << "None";
	}
	else
		cout << "Invalid input!";
}

int main() {
	db_response::ConnectionFunction();
	system("cls");

	int choose;

	cout << "+-------------------------------------------------------------+" << endl;
	cout << "|                  BIRTH REGISTRATION SYSTEM                  |" << endl;
	cout << "+-------------------------------------------------------------+" << endl;

	system("pause");
	cout << "I would like to..." << endl;
	cout << "1. Register" << endl << "2. Login" << endl << "Please select your option: ";
	cin >> choose;

	if (choose == 1)
	{
		Register();
	}
	else if (choose == 2)
	{
		Login();
	}
	else
	{
		cout << "Invalid Input! Choose only '1' or '2'!" << endl << endl;
		system("pause");
		main();
	}

	return 0;
}