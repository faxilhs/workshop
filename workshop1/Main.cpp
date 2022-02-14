//header files
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>   
#include <conio.h>
#include <cstdlib>
#include <mysql.h>
#include <ctime>
#include <cmath>
using namespace std;

//for dob checking
const int MIN_VALID_YR = 1920;

//start db connection
int qstate, qstate1, qstate2, qstate3;
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

//public variable
int choose;
string userNo, mumNo, dadNo, certId, staffIc;

//take current time
time_t t = time(NULL);
tm* tPtr = localtime(&t);
int current_date = tPtr->tm_mday;
int current_month = tPtr->tm_mon + 1;
int current_year = tPtr->tm_year + 1900;

//check dob if in future
bool checkFutureDate(int year, int month, int day) {
	if (year > (current_year))
		return true;
	else if (year == current_year) {
		if (month > current_month)
			return true;
		else if (month == current_month) {
			if (day > current_date)
				return true;
			else
				return false;
		}
	}
}

//calculate age
void findAge(int current_date, int current_month, int current_year, int birth_date, int birth_month, int birth_year)
{
	// days of every month
	int month[] = { 31, 28, 31, 30, 31, 30, 31,
						  31, 30, 31, 30, 31 };

	// if birth date is greater then current birth month then do not count this month and 
	//add 30 to the date so as to subtract the date and get the remaining days
	if (birth_date > current_date) {
		current_date = current_date + month[birth_month - 1];
		current_month = current_month - 1;
	}

	// if birth month exceeds current month, then do not count this year and add 12 to the month so
	// that we can subtract and find out the difference
	if (birth_month > current_month) {
		current_year = current_year - 1;
		current_month = current_month + 12;
	}

	if (birth_year > current_year)
	{
		birth_year = birth_year - 100;
	}

	// calculate date, month, year
	int calculated_year = current_year - birth_year;

	// print the present age
	printf("%d", calculated_year);
}

bool check_digit(string str) {
	for (int i = 0; i < str.length(); i++)
		if (isdigit(str[i]) == false)
			return false;
	return true;
}

//header for interface
void head(){
	cout << endl << endl;
	cout << "\t\t\t+--------------------------------------------------------------+" << endl;
	cout << "\t\t\t|                   BIRTH REGISTRATION SYSTEM                  |" << endl;
	cout << "\t\t\t+--------------------------------------------------------------+" << endl;
}

//check password
class PasswordStatus
{
	bool len, low, up, dig;
public:
	PasswordStatus()
	{len = low = up = dig = false;}

	void ShowStatus()
	{
		cout << "\n\n\t\t\t\t\tYour Password Status" << endl;
		cout << "\t\t\t\t\tLength : " << (len ? "OK" : "Too Short") << endl;
		cout << "\t\t\t\t\tContains lowercase letter : " << (low ? "Yes" : "No") << endl;
		cout << "\t\t\t\t\tContains uppercase letter : " << (up ? "Yes" : "No") << endl;
		cout << "\t\t\t\t\tContains Digit : " << (dig ? "Yes" : "No") << endl;
	}

	void checkValidity(string pass)
	{
		int sLen = pass.length();
		len = (sLen >= 8);

		for (int i = 0; i < sLen; i++)
		{
			char c = pass[i];
			if (!low && islower(c)) { low = true; continue; }
			if (!up && isupper(c)) { up = true; continue; }
			if (!dig && isdigit(c)) { dig = true; continue; }
		}
	}
	
	bool IsTotalyValid()
	{
		return low && up && dig && len;
	}
};

class list {
public:
	void staffList() {
		cout << "\n\n\t\t\t\t\t\t\tStaff List\n\n";

		cout << "\t\t" << left << setw(14) << setfill('-') << left << '+'
			<< setw(31) << setfill('-') << left << '+'
			<< setw(23)
			<< setfill('-') << left << '+' << setw(21)
			<< setfill('-')
			<< '+' << '+' << endl;
		cout << "\t\t" << setfill(' ') << '|' << left << setw(13)
			<< "IC No"
			<< setfill(' ') << '|' << setw(30) << "Name"
			<< setfill(' ') << '|' << setw(22) << "Email"
			<< setfill(' ') << '|' << right << setw(20)
			<< "Phone Number" << '|' << endl;

		cout << "\t\t" << left << setw(14) << setfill('-') << left
			<< '+' << setw(31) << setfill('-') << left << '+'
			<< setw(23)
			<< setfill('-') << left << '+' << setw(21) << setfill('-')
			<< '+' << '+' << endl;

		if (!qstate) {
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{

				cout << "\t\t" << setfill(' ') << '|' << left << setw(13) << row[0]
					<< setfill(' ') << '|' << setw(30) << row[1]
					<< setfill(' ') << '|' << setw(22) << row[2]
					<< setfill(' ') << '|' << right << setw(20)
					<< row[3] << '|' << endl;
			}
			cout << "\t\t" << left << setw(14) << setfill('-') << left
				<< '+' << setw(31) << setfill('-') << left << '+'
				<< setw(23)
				<< setfill('-') << left << '+' << setw(21)
				<< setfill('-')
				<< '+' << '+' << endl;
		}
	}

	void applicationlist() {
		cout << "\n\t\t\t\t\t       " << "Your Application List" << "\n";
		cout << "\t\t" << left << setw(16) << setfill('-') << left << '+'
			<< setw(32) << setfill('-') << left << '+'
			<< setw(16) << setfill('-') << left << '+' << setw(16)
			<< setw(16) << setfill('-') << left << '+' << setw(11)
			<< setfill('-') << left << '+' << '+' << endl;
		cout << "\t\t" << setfill(' ') << '|' << left << setw(15)
			<< "Certificate ID"
			<< setfill(' ') << '|' << setw(31) << "Baby Name"
			<< setfill(' ') << '|' << setw(15) << "Mother ID No"
			<< setfill(' ') << '|' << setw(15) << "Father ID No"
			<< setfill(' ') << '|' << setw(10) << "Status"
			<< setfill(' ') << '|' << endl;

		cout << "\t\t" << left << setw(16) << setfill('-') << left
			<< '+' << setw(32) << setfill('-') << left << '+'
			<< setw(16) << setfill('-') << left << '+' << setw(16)
			<< setw(16) << setfill('-') << left << '+' << setw(11)
			<< setfill('-') << left << '+' << '+' << endl;

		if (!qstate) {
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{
				string mumNo = row[7] ? row[7] : "NULL";
				string dadNo = row[8] ? row[8] : "NULL";
				cout << "\t\t" << setfill(' ') << '|' << left << setw(15) << row[0]
					<< setfill(' ') << '|' << setw(31) << row[1]
					<< setfill(' ') << '|' << setw(15) << mumNo
					<< setfill(' ') << '|' << setw(15) << dadNo
					<< setfill(' ') << '|' << setw(10) << row[10]
					<< setfill(' ') << '|' << endl;
			}
			cout << "\t\t" << left << setw(16) << setfill('-') << left
				<< '+' << setw(32) << setfill('-') << left << '+'
				<< setw(16) << setfill('-') << left << '+' << setw(16)
				<< setw(16) << setfill('-') << left << '+' << setw(11)
				<< setfill('-') << left << '+' << '+' << endl;
		}
	}

	void fullList() {
		int qstate1;
		string seacrh_baby = "select * from certificate";
		const char* q1 = seacrh_baby.c_str();
		qstate1 = mysql_query(conn, q1);
		cout << "\n\t\t\t\t\t\t Application List" << endl;

		cout << left << setw(8) << setfill('-') << left << '+'
			<< setw(31) << setfill('-') << left << '+'
			<< setw(14) << setfill('-') << left << '+'
			<< setw(14) << setfill('-') << left << '+'
			<< setw(16) << setfill('-') << left << '+'
			<< setw(16) << setfill('-') << left << '+'
			<< setw(16) << setfill('-') << left << '+'
			<< setw(11) << setfill('-') << '+' << '+' << endl;

		cout << setfill(' ') << '|' << left << setw(7)
			<< "Cert_ID"
			<< setfill(' ') << '|' << setw(30) << "Baby Name"
			<< setfill(' ') << '|' << setw(13) << "Date of Birth"
			<< setfill(' ') << '|' << setw(13) << "Time of Birth"
			<< setfill(' ') << '|' << setw(15) << "User ID No"
			<< setfill(' ') << '|' << setw(15) << "Mother ID No"
			<< setfill(' ') << '|' << setw(15) << "Father ID No"
			<< setfill(' ') << '|' << setw(10) << "Status" << '|' << endl;

		cout << left << setw(8) << setfill('-') << left << '+'
			<< setw(31) << setfill('-') << left << '+'
			<< setw(14) << setfill('-') << left << '+'
			<< setw(14) << setfill('-') << left << '+'
			<< setw(16) << setfill('-') << left << '+'
			<< setw(16) << setfill('-') << left << '+'
			<< setw(16) << setfill('-') << left << '+'
			<< setw(11) << setfill('-') << '+' << '+' << endl;

		if (!qstate) {
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{
				string mumNo = row[7] ? row[7] : "NULL";
				string dadNo = row[8] ? row[8] : "NULL";
				cout << setfill(' ') << '|' << right << setw(7) << row[0]
					<< setfill(' ') << '|' << left << setw(30) << row[1]
					<< setfill(' ') << '|' << setw(13) << row[2]
					<< setfill(' ') << '|' << setw(13) << row[3]
					<< setfill(' ') << '|' << setw(15) << row[6]
					<< setfill(' ') << '|' << setw(15) << mumNo
					<< setfill(' ') << '|' << setw(15) << dadNo
					<< setfill(' ') << '|' << setw(10) << row[10] << '|' << endl;
			}
			cout << left << setw(8) << setfill('-') << left << '+'
				<< setw(31) << setfill('-') << left << '+'
				<< setw(14) << setfill('-') << left << '+'
				<< setw(14) << setfill('-') << left << '+'
				<< setw(16) << setfill('-') << left << '+'
				<< setw(16) << setfill('-') << left << '+'
				<< setw(16) << setfill('-') << left << '+'
				<< setw(11) << setfill('-') << '+' << '+' << endl;
		}
	}

	void detail() {
		int qstate1, qstate2, qstate3;

		while (row = mysql_fetch_row(res))
		{	
			cout << "\n\t\t\t\t\t\t BABY INFO \n";
			cout << "\t\t\t\t  1. Baby Name : " << row[1] << endl;
			cout << "\t\t\t\t  2. Baby Gender : " << row[4] << endl;
			cout << "\t\t\t\t  3. Date of Birth : " << row[2] << endl;
			cout << "\t\t\t\t  4. Time of Birth : " << row[3] << endl;
			cout << "\t\t\t\t  5. Place of Birth : " << row[5] << endl;
			string mumNo = row[7] ? row[7] : "NULL";
			string dadNo = row[8] ? row[8] : "NULL";

			string search_mum = "select * from mother where Mother_DocNo = '" + mumNo + "'";
			const char* m = search_mum.c_str();
			qstate2 = mysql_query(conn, m);
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{
				cout << "\n\t\t\t\t\t\tMOTHER INFO \n";
				cout << "\t\t\t\t  1. Name : " << row[0] << endl;
				cout << "\t\t\t\t  2. Type of Identification Document : " << row[1] << endl;
				cout << "\t\t\t\t  3. Identification document number : " << row[2] << endl;
				cout << "\t\t\t\t  4. Citizenship : " << row[3] << endl;
				cout << "\t\t\t\t  5. Age : " << row[4] << endl;
				cout << "\t\t\t\t  6. Race : " << row[5] << endl;
				cout << "\t\t\t\t  7. Religion : " << row[6] << endl;
				cout << "\t\t\t\t  8. Mobile Number : " << row[7] << endl;
				cout << "\t\t\t\t  9. Job : " << row[8] << endl;
				cout << "\t\t\t\t  10. Education Status : " << row[9] << endl;
				cout << "\t\t\t\t  11. Marital Status : " << row[10] << endl;
				cout << "\t\t\t\t  12. Full Address: " << row[11] << ", " << row[12] << " " << row[13] << ", " << row[14] << endl;
			}

			string search_dad = "select * from father where Father_DocNo = '" + dadNo + "'";
			const char* d = search_dad.c_str();
			qstate3 = mysql_query(conn, d);
			res = mysql_store_result(conn);
			if (res->row_count == 1) {
				while (row = mysql_fetch_row(res))
				{
					cout << "\n\t\t\t\t\t\tFATHER INFO \n";
					cout << "\t\t\t\t  1. Name : " << row[0] << endl;
					cout << "\t\t\t\t  2. Type of Identification Document : " << row[1] << endl;
					cout << "\t\t\t\t  3. Identification document number : " << row[2] << endl;
					cout << "\t\t\t\t  4. Citizenship : " << row[3] << endl;
					cout << "\t\t\t\t  5. Age : " << row[4] << endl;
					cout << "\t\t\t\t  6. Race : " << row[5] << endl;
					cout << "\t\t\t\t  7. Religion : " << row[6] << endl;
					cout << "\t\t\t\t  8. Mobile Number : " << row[7] << endl;
					cout << "\t\t\t\t  9. Job : " << row[8] << endl;
					cout << "\t\t\t\t  10. Education Status : " << row[9] << endl << endl;
				}
			}
			else {
				cout << "\n\t\t\t\t\t\t     FATHER INFO \n";
				cout << "\t\t\t\t\t\tNo Father Information\n";
			}
		}	
	}
};

//check email
class Email {
public:
	bool isChar(char c)
	{
		return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
	} 
	// Function to check the character is an digit or not
	bool isDigit(const char c)
	{
		return (c >= '0' && c <= '9');
	}
	// Function to check email id is valid or not
	bool is_valid(string email)
	{
		// Check the first character is an alphabet or not
		if (!isChar(email[0])) {
			// If it's not an alphabet email id is not valid
			return 0;
		}
		// Variable to store position of At and Dot
		int At = -1, Dot = -1;

		// Traverse over the email id string to find position of Dot and At
		for (int i = 0;	i < email.length(); i++) {
			// If the character is '@'
			if (email[i] == '@') {
				At = i;
			}
			// If character is '.'
			else if (email[i] == '.') {
				Dot = i;
			}
		}
		// If At or Dot is not present
		if (At == -1 || Dot == -1)
			return 0;
		// If Dot is present before At
		if (At > Dot)
			return 0;
		// If Dot is present at the end
		return !(Dot >= (email.length() - 1));
	}
};

void Register();
void Login(); 
void UsrLog();
void UsrMain();
void birthRes();
void checkStatus();
void birthCert();
void deleteApplication();
void editPsw();
void staffLog();
void staffMain(); 
void viewApplication();
void brsReport();
void rMonth();
void approved();
void pending();
void adminLog();
void adminMain();
void resStaff();
void updateStaff();
void deleteStaff();

int main() {
	db_response::ConnectionFunction();
	system("cls");
	
	cout << endl << endl;
	cout << "\t\t\t+--------------------------------------------------------------+\n";
	cout << "\t\t\t|                                                              |\n";
	cout << "\t\t\t|                                                              |\n";
	cout << "\t\t\t|                                                              |\n";
	cout << "\t\t\t|                          WELCOME TO                          |\n";
	cout << "\t\t\t|                   BIRTH REGISTRATION SYSTEM                  |\n";
	cout << "\t\t\t|                                                              |\n";
	cout << "\t\t\t|                                                              |\n";
	cout << "\t\t\t|                                                              |\n";
	cout << "\t\t\t+--------------------------------------------------------------+\n\n\n\t\t\t\t\t";
	
	system("pause");system("cls");

	head();
	cout << endl << endl;
	cout << "\t\t\t\t\tI would like to..." << endl;
	cout << "\t\t\t\t\t1. Register" << endl << "\t\t\t\t\t2. Login" << endl << "\t\t\t\t\t3. Exit" << endl << "\t\t\t\t\tPlease select your option: ";
	cin >> choose;

	if (choose == 1)
		Register();
	else if (choose == 2)
		Login();
	else if (choose == 3)
		main();
	else 
	{
		cout << "\n\t\t\t\t\tInvalid Input. Please try again.\n\n";
		system("pause");
		main();
	}
	return 0;
}

void Register() {
	system("cls");
	PasswordStatus ps;
	string doctype;
	char pw[15];
	head();
	cout << "\n\t\t    To register account as a user. Please fill in the following information.\n";
	cout << "\t\t    (Enter 0 to back to main page)\n\n";

doctype:
	cout << "\t\t\t\t1. Type of Identification Document" << endl;
	cout << "\t\t\t\t     a. MYKAD" << endl;
	cout << "\t\t\t\t     b. MYKAS" << endl;
	cout << "\t\t\t\t     c. MYPR" << endl;
	cout << "\t\t\t\t     d. Passport" << endl;
	cout << "\t\t\t\t   Please enter your option: "; cin >> doctype;

	if (doctype == "a" || doctype == "A")
		doctype = "MYKAD";
	else if (doctype == "b" || doctype == "B")
		doctype = "MYKAS";
	else if (doctype == "c" || doctype == "C")
		doctype = "MYPR";
	else if (doctype == "d" || doctype == "D")
		doctype = "Passport";
	else if (doctype == "0")
		main();
	else {
		cout << "\n\t\t\t\t   Invalid input. Try Again.\n\n";
		goto doctype;
	}

	cin.ignore();
	if (doctype == "MYKAD" || doctype == "MYKAS" || doctype == "MYPR") {
	ic:
		cout << "\n\t\t\t\t2. Identification Card Number (without '-'): ";
		getline(cin, userNo);
		if (userNo == "0")
			main();
		else if (userNo.size() != 12 || !check_digit(userNo)) {
			cout << "\n\t\t\t\tThe IC number should be 12 digits only. Please enter again.\n";  //Malaysian IC No is fixed to 12 integer only
			goto ic;
		}	
	}
	else if (doctype == "Passport") {
	passport:
		cout << "\n\t\t\t\t2. Passport Number: ";
		getline(cin, userNo);
		if (userNo == "") {
			cout << "\n\t\t\t\t  Please input the passport number\n";
			goto passport;
		}
		else if (userNo == "0")
			main();
	}

	string check_user = "select * from users where User_DocNo = '" + userNo + "'";
	const char* c = check_user.c_str();
	qstate = mysql_query(conn, c);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1)
		{
			cout << "\n\t\t\t\tYou already have an account. Press enter to login.\n\n\t\t\t\t";
			system("pause");
			Login();
		}
		else
		{
		password:
			int i = 0;
			char ch;
			cout << endl << "\t\t\t\t3. Password" << endl
				<< "\t\t\t\tIn order to protect your account, make sure your \n\t\t\t\t   password fulfill the following requirement\n";
			cout << "\t\t\t\t\t-More than 8 characters" << endl << "\t\t\t\t\t-A lowercase letter" << endl
				<< "\t\t\t\t\t-An uppercase letter" << endl << "\t\t\t\t\t-A number"
				<< endl << "\t\t\t\tPlease enter you password: ";

			while ((ch = _getch()) != 13)
			{
				pw[i] = ch;
				i++;
				printf("*");
			}
			pw[i] = '\0';

			ps.checkValidity(pw);
			ps.ShowStatus();
			if (!ps.IsTotalyValid()) {
				cout << "\n\t\t\tYour password does not fulfill the requirement. Please try another password.\n";
				goto password;
			}

			string insertUser_query = "INSERT into users (User_DocType, User_DocNo , User_Psw) values ('" + doctype + "', '"
				+ userNo + "', '" + pw + "')";
			const char* q = insertUser_query.c_str();
			qstate1 = mysql_query(conn, q);
		}
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
	cout << "\n\t\t\t\t\tPassword accepted. \n\t\t\t\t\tSuccessfully register!\n\n\t\t\t\t     ";
	system("pause");
	Login();
}

void Login()
{
	system("cls");
	cout << "\n\n\t\t\t+------------------------------------------------------------+" << endl;
	cout << "\t\t\t|                         LOGIN AS?                          |" << endl;
	cout << "\t\t\t+------------------------------------------------------------+\n\n";
	cout << "\t\t\t\t\t\t1. User" << endl << "\t\t\t\t\t\t2. Staff" << 
		endl << "\t\t\t\t\t\t3. Admin" << endl << "\t\t\t\t\t\t4. Exit";
	cout << "\n\n\t\t\t\t\tPlease select your option: ";
	cin >> choose;
	switch (choose) {
	case 1:
		UsrLog();
		break;
	case 2:
		staffLog();
		break;
	case 3:
		adminLog();
		break;
	case 4:
		main();
		break;
	default:
		cout << "\t\t\t\t\tInvalid input!! Please try again!\n";
		system("pause");
		Login();
	}
}

//USER
void UsrLog()
{
	system("cls");
	string doctype, userPsw;
	cout << "\n\n\t\t\t";
	cout << '+' << setfill('-') << setw(56) << '+' << endl;
	cout << "\t\t\t|                    USER LOGIN PAGE                    |" << endl;
	cout << "\t\t\t" << '+' << setfill('-') << setw(56) << '+' << endl << endl;

	cout << "\t\t\t     Enter your the following information to login.\n";
	cout << "\t\t\t     (Enter 0 to back to main page)\n\n";

doctype:
	cout << "\t\t\t\t1. Type of Identification Document" << endl;
	cout << "\t\t\t\t   a. MYKAD" << endl;
	cout << "\t\t\t\t   b. MYKAS" << endl;
	cout << "\t\t\t\t   c. MYPR" << endl;
	cout << "\t\t\t\t   d. Passport" << endl;
	cout << "\t\t\t\tPlease enter your option: "; cin >> doctype;
	if (doctype == "a" || doctype == "A")
		doctype = "MYKAD";
	else if (doctype == "b" || doctype == "B")
		doctype = "MYKAS";
	else if (doctype == "c" || doctype == "C")
		doctype = "MYPR";
	else if (doctype == "d" || doctype == "D")
		doctype = "Passport";
	else if (doctype == "0")
		main();
	else {
		cout << "\n\t\t\t\tInvalid input. Try Again.\n\n";
		goto doctype;
	}

	if (doctype == "MYKAD" || doctype == "MYKAS" || doctype == "MYPR") {
		cout << "\n\t\t\t\t2. Identification Card Number (without '-'): ";
		cin.ignore(); getline(cin, userNo);
		if (userNo == "0")
			main();
	}
	else if (doctype == "Passport") {
		cout << "\n\t\t\t\t2. Passport Number: ";
		cin.ignore(); getline(cin, userNo);
		if (userNo == "0")
			main();
	}
	
	cout << "\n\t\t\t\t3. Password: ";

	char ch;
	while ((ch = _getch()) != 13)
	{
		userPsw += ch;
		cout << '*';
	}
	string checki = "SELECT * FROM users WHERE User_DocNo = '" + userNo + "' AND User_Psw = '" + userPsw + "'";
	const char* i = checki.c_str();
	qstate1 = mysql_query(conn, i);

	if (!qstate1) {
		res = mysql_store_result(conn);
		if (row = mysql_fetch_row(res))
		{
			cout << "\n\n\t\t\t\t\t      LOGIN Success!!\n\n\t\t\t\t\t";
			system("pause");

			UsrMain();
		}
		else
		{
			cout << "\n\t\t\tFail to login. Please check your Ic No or Passport No and password.\n\n\t\t\t\t\t";
			system("pause");
			UsrLog();
		}
	}
}

void UsrMain() 
{
	system("cls");

	head();
	cout << "\n\t\t\t\t    Hi! Welcome to Birth Registration System.\n\n";
	cout << "\t\t\t\t\t1. Make registration for my baby" << endl;
	cout << "\t\t\t\t\t2. Check application status" << endl;
	cout << "\t\t\t\t\t3. View birth certificate" << endl;
	cout << "\t\t\t\t\t4. Delete application" << endl;
	cout << "\t\t\t\t\t5. Edit Password" << endl;
	cout << "\t\t\t\t\t6. Exit" << endl << endl;
	cout << "\t\t\t\t  What you want to do here? Please enter your option: ";
	cin >> choose;
	switch (choose) {
	case 1:
		birthRes();
		break;
	case 2:
		checkStatus();
		break;
	case 3:
		birthCert();
		break;
	case 4:
		deleteApplication();
		break;
	case 5:
		editPsw();
		break;
	case 6:
		main();
		break;
	default:
		cout << "Wrong input. Please enter again." << endl;
		system("pause");
		UsrMain();
		break;
	}
}

void birthRes()
{
	system("cls");
	//baby info
	string baby, gender, place;
	char dob[11], tob[5];
	//mother info
	string mother, mumDocType, mumCitizen, mumAge, mumRace, mumReligion, mumMobile, mumJob, mumEdu, marital, addr, postcode, city, state;
	//father info
	string father, dadDocType, dadCitizen, dadAge, dadRace, dadReligion, dadMobile, dadJob, dadEdu;

	head();

	cout << "\n\t\t\t  Fill in the following details to make the birth registration.\n";
	cout << "\t\t\t  (Enter 0 to back to main page)\n\n";

	//BABY INFORMATION
	cout << "\t\t\t\t\t\t   BABY INFO" << endl;
baby:
	cout << "\t\t\t  1. Baby Name: ";
	cin.ignore();
	getline(cin, baby);
	if (baby == "") {
		cout << "\t\t\t  Please input the baby's name.\n";
		goto baby;
	}
	else if (baby == "0")
		main();
gender:
	cout << "\t\t\t  2. Baby Gender (M/F): "; getline(cin, gender);
	if (gender == "") {
		cout << "\t\t\t  Please input the baby's gender.\n";
		goto gender;
	}
	else if (gender != "m" && gender != "M" && gender != "F" && gender != "f") {
		cout << "\t\t\t  Invalid input. Try Again.\n";
		goto gender;
	}
	else if (gender == "0")
		main();

date:
	int yyyy, mm, dd;
	cout << "\t\t\t  3. Date of Birth (YYYY-MM-DD): "; cin >> dob; cin.ignore();
	if (strlen(dob) < 10 || strlen(dob) > 10)
	{
		cout << "\n\t\t\t  Error:Input should be in yyyy-mm-dd format. Please try again.\n\n";
		goto date;
	}
	else if (strlen(dob) == 10)
	{
		char y[5], m[3], d[3];
		int i, j;

		for (i = 0; i < 4; i++)
			y[i] = dob[i];

		for (i = 5, j = 0; i < 7; i++, j++)
			m[j] = dob[i];

		for (i = 8, j = 0; i < 10; i++, j++)
			d[j] = dob[i];

		yyyy = atoi(y);
		mm = atoi(m);
		dd = atoi(d);
		if (yyyy > current_year || yyyy < 1900) {
			cout << "\t\t\t  Year is not valid. Please try again.\n\n"; goto date;
		}
		else
		{
			if (mm >= 1 && mm <= 12)
			{
				//check days
				if ((dd >= 1 && dd <= 31) && (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12)) {
					if (checkFutureDate(yyyy, mm, dd) == true) {
						cout << "\t\t\t  Note that the date cannot in future.Please try again.\n\n";
						goto date;
					}
					else
						cout << "\t\t\t  Date of birth is accepted.\n\n";
				}
				else if ((dd >= 1 && dd <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11))
				{
					if (checkFutureDate(yyyy, mm, dd) == true) {
						cout << "\t\t\t  Note that the date cannot in future.Please try again.\n\n";
						goto date;
					}
					else
						cout << "\t\t\t  Date of birth is accepted.\n\n";
				}
				else if ((dd >= 1 && dd <= 28) && (mm == 2))  //February
				{
					if (checkFutureDate(yyyy, mm, dd) == true) {
						cout << "\t\t\t  Note that the date cannot in future.Please try again.\n\n";
						goto date;
					}
					else
						cout << "\t\t\t  Date of birth is accepted.\n\n";
				}
				else if (dd == 29 && mm == 2 && (yyyy % 400 == 0 || (yyyy % 4 == 0 && yyyy % 100 != 0)))  //leap year
				{
					if (checkFutureDate(yyyy, mm, dd) == true) {
						cout << "\t\t\t  Note that the date cannot in future.Please try again.\n\n";
						goto date;
					}
					else
						cout << "\t\t\t  Date of birth is accepted.\n\n";
				}
				else {
					cout << "\t\t\t  Day is not valid.Please try again.\n\n";
					goto date;
				}
			}
			else
			{
				cout << "\t\t\t  Month is not valid.Please try again.\n\n";
				goto date;
			}
		}
	}
time:
	int hr, min;
	cout << "\t\t\t  4. Time of Birth (HH:MM): "; cin >> tob; cin.ignore();
	if (strlen(tob) < 5 || strlen(tob) > 5)
	{
		cout << "\n\t\t\t  Error:Input should be in HH:MM format. Please try again.\n\n";
		goto time;
	}
	else if (strlen(tob) == 5)
	{
		char h[3], m[3];
		int i, j;

		for (i = 0; i < 2; i++)
			h[i] = tob[i];

		for (i = 3, j = 0; i < 5; i++, j++)
			m[j] = tob[i];

		hr = atoi(h);
		min = atoi(m);
		if (hr >= 0 && hr <= 23) {
			if (min >= 0 && min <= 59)
			{
				printf("\t\t\t  Time of birth is accepted.\n\n");
			}
			else
			{
				printf("\t\t\t  Minute is not valid.Please try again.\n\n");
				goto time;
			}
		}
		else
		{
			cout << "\t\t\t  Hour is not valid. Please try again.\n\n"; goto time;
		}
	}

place:
	cout << "\t\t\t  5. Place of birth: "; getline(cin, place);
	if (place == "") {
		cout << "\t\t\t  Please input the place of birth.\n";
		goto place;
	}
	else if (place == "0")
		main();

	//MOTHER INFORMATION
	cout << "\n\t\t\t\t\t\t   MOTHER INFO" << endl;
mum:
	cout << "\t\t\t  1. Name: "; getline(cin, mother);
	if (mother == "") {
		cout << "\t\t\t  Please input the mother's name.\n";
		goto mum;
	}
	else if (mother == "0")
		main();

doctypemum:
	cout << "\t\t\t  2. Type of Identification Document" << endl;
	cout << "\t\t\t     a. MYKAD" << endl;
	cout << "\t\t\t     b. MYKAS" << endl;
	cout << "\t\t\t     c. MYPR" << endl;
	cout << "\t\t\t     d. Passport" << endl;
	cout << "\t\t\t  Please enter your option: "; getline(cin, mumDocType);

	if (mumDocType == "a" || mumDocType == "A")
		mumDocType = "MYKAD";
	else if (mumDocType == "b" || mumDocType == "B")
		mumDocType = "MYKAS";
	else if (mumDocType == "c" || mumDocType == "C")
		mumDocType = "MYPR";
	else if (mumDocType == "d" || mumDocType == "D")
		mumDocType = "Passport";
	else if (mumDocType == "0")
		main();
	else {
		cout << "\t\t\t  Invalid input. Try Again.\n";
		goto doctypemum;
	}

	if (mumDocType == "MYKAD" || mumDocType == "MYKAS" || mumDocType == "MYPR") {
	mumic:
		cout << "\n\t\t\t  3. Identification Card Number (without '-'): ";
		getline(cin, mumNo);
		if (mumNo == "0")
			main();
		else if (!check_digit(mumNo) || mumNo.size() != 12) {
			cout << "\n\t\t\t  The IC number should be 12 digits only. Please enter again.\n";  //Malaysian IC No is fixed to 12 integer only
			goto mumic;
		}
		if (mumNo[11] % 2 != 0) {
			cout << "\n\t\t\t  Invalid Input. Last Digit for a mother's IC should be even number.\n\t\t\t  Please try again.\n";
			goto mumic;
		}
	}
	else if (mumDocType == "Passport") {
	mumpassport:
		cout << "\n\t\t\t  3. Passport Number: ";
		getline(cin, mumNo);
		if (mumNo == "") {
			cout << "\n\t\t\t  Please input the passport number\n";
			goto mumpassport;
		}
		else if (mumNo == "0")
			main();
	}

citizenmum:
	cout << "\t\t\t  4. Citizenship" << endl;
	cout << "\t\t\t     a. Citizen" << endl;
	cout << "\t\t\t     b. Permanent-citizen" << endl;
	cout << "\t\t\t     c. Non-citizen" << endl;
	cout << "\t\t\t  Please enter your option: ";  getline(cin, mumCitizen);

	if (mumCitizen == "a" || mumCitizen == "A")
		mumCitizen = "Citizen";
	else if (mumCitizen == "b" || mumCitizen == "B")
		mumCitizen = "Permanent-citizen";
	else if (mumCitizen == "c" || mumCitizen == "C")
		mumCitizen = "Non-citizen";
	else if (mumCitizen == "0")
		main();
	else {
		cout << "\t\t\t  Invalid input. Please try again\n";
		goto citizenmum;
	}
mumage:
	cout << "\n\t\t\t  5. Age: ";  
	if (mumDocType == "MYKAD" || mumDocType == "MYKAS" || mumDocType == "MYPR") {
		
		char y[2], m[2], d[2];
		int i, j;
		for (i = 0; i < 2; i++)
			y[i] = mumNo[i];

		for (i = 2, j = 0; i < 4; i++, j++)
			m[j] = mumNo[i];

		for (i = 4, j = 0; i < 6; i++, j++)
			d[j] = mumNo[i];

		int birth_year = atoi(y) + 2000;
		int birth_month = atoi(m);
		int birth_date = atoi(d);
		findAge(current_date, current_month, current_year, birth_date, birth_month, birth_year);
	}
	else if (mumDocType == "Passport") {
		getline(cin, mumAge);
		if (mumAge == "") {
			cout << "\t\t\t  Please input the mother's age.\n";
			goto mumage;
		}
		else if (mumAge == "0")
			main();
		else if (!check_digit(mumAge) || mumAge.size() != 2) {
			cout << "\t\t\t Something wrong with the input. Please try again\n";
			goto mumage;
		}
	}
mumrace:
	cout << "\n\t\t\t  6. Race\n";
	cout << "\t\t\t     a. Malay\n"; cout << "\t\t\t     b. Chinese\n"; cout << "\t\t\t     c. Indian\n"; cout << "\t\t\t     d. others\n";
	cout << "\t\t\t  Please enter your option: "; getline(cin, mumRace);
	if (mumRace == "a" || mumRace == "A")
		mumRace = "Malay";
	else if (mumRace == "b" || mumRace == "B")
		mumRace = "Chinese";
	else if (mumRace == "c" || mumRace == "C")
		mumRace = "Indian";
	else if (mumRace == "d" || mumRace == "D")
	{
	mumrace2:
		cout << "\t\t\t  Enter your race: "; getline(cin, mumRace);
		if (mumRace == "0")
			main();
		else if (mumRace == "") {
			cout << "\t\t\t  Please input the mother's race.\n";
			goto mumrace2;
		}
	}
	else if (mumRace == "0")
		main();
	else {
		cout << "\t\t\t  Invalid input. Please try again.\n";
		goto mumrace;
	}


mumreli:
	cout << "\t\t\t  7. Religion: "; getline(cin, mumReligion);
	if (mumReligion == "") {
		cout << "\t\t\t  Please input the mother's religion.\n";
		goto mumreli;
	}
	else if (mumReligion == "0")
		main();
mummobile:
	cout << "\t\t\t  8. Mobile Number: "; getline(cin, mumMobile);
	if (mumMobile == "") {
		cout << "\t\t\t  Please input the mother's mobile no.\n";
		goto mummobile;
	}
	else if (mumMobile == "0")
		main();
mumjob:
	cout << "\t\t\t  9. Job: "; getline(cin, mumJob);
	if (mumJob == "") {
		cout << "\t\t\t  Please input the mother's job.\n";
		goto mumjob;
	}
	else if (mumJob == "0")
		main();
educationmum:
	cout << "\t\t\t  10. Education Status" << endl;
	cout << "\t\t\t      a. Primary School " << endl;
	cout << "\t\t\t      b. Secondary School" << endl;
	cout << "\t\t\t      c. Pre-University" << endl;
	cout << "\t\t\t      d. Higher Education" << endl;
	cout << "\t\t\t      e. Non specified" << endl;
	cout << "\t\t\t  Please enter your option: ";  getline(cin, mumEdu);

	if (mumEdu == "a" || mumEdu == "A")
		mumEdu = "Primary School";
	else if (mumEdu == "b" || mumEdu == "B")
		mumEdu = "Secondary School";
	else if (mumEdu == "c" || mumEdu == "C")
		mumEdu = "Pre-University";
	else if (mumEdu == "d" || mumEdu == "D")
		mumEdu = "Higher Education";
	else if (mumEdu == "e" || mumEdu == "E")
		mumEdu = "Non specified";
	else if (mumEdu == "0")
		main();
	else {
		cout << "\t\t\t  Invalid input. Please try again\n";
		goto educationmum;
	}

marital:
	cout << "\n\t\t\t  11. Marital Status: " << endl;
	cout << "\t\t\t      a. Single " << endl;
	cout << "\t\t\t      b. Married" << endl;
	cout << "\t\t\t      c. Widowed" << endl;
	cout << "\t\t\t      d. Divorced" << endl;
	cout << "\t\t\t  Please enter your option: ";  getline(cin, marital);

	if (marital == "a" || marital == "A")
		marital = "Single";
	else if (marital == "b" || marital == "B")
		marital = "Married";
	else if (marital == "c" || marital == "C")
		marital = "Widowed";
	else if (marital == "d" || marital == "D")
		marital = "Divorced";
	else if (marital == "0")
		main();
	else {
		cout << "\t\t\t  Input invalid. Try Again.\n";
		goto marital;
	}
mumadd:
	cout << "\n\t\t\t  12. Address: "; getline(cin, addr);
	if (addr == "") {
		cout << "\t\t\t  Please input the address.\n";
		goto mumadd;
	}
	else if (addr == "0")
		main();
mumpost:
	cout << "\t\t\t  13. Postcode: "; getline(cin, postcode);
	if (postcode == "") {
		cout << "\t\t\t  Please input the postcode.\n";
		goto mumpost;
	}
	else if (postcode == "0")
		main();
	else if (!check_digit(postcode) || postcode.size() != 5) {
		cout << "\t\t\t  Something wrong with your input. A postcode should be 5 digits.\n\t\t\t  Please try again.\n";
		goto mumpost;
	}
	
city:
	cout << "\t\t\t  14. City: "; getline(cin, city);
	if (city == "") {
		cout << "\t\t\t  Please input the city.\n";
		goto city;
	}
	else if (city == "0")
		main();
state:
	cout << "\t\t\t  15. State: ";  getline(cin, state);
	if (state == "") {
		cout << "\t\t\t  Please input the state.\n";
		goto state;
	}
	else if (state == "0")
		main();

	//FATHER INFORMATION
	if (marital == "Married") {
		cout << "\n\t\t\t\t\t\t   FATHER INFO" << endl;
	dad:
		cout << "\t\t\t  1. Name: "; getline(cin, father);
		if (father == "") {
			cout << "\t\t\t  Please input the father's name.\n";
			goto dad;
		}
		else if (father == "0")
			main();
	doctypedad:
		cout << "\t\t\t  2. Type of Identification Card" << endl;
		cout << "\t\t\t     a. MYKAD" << endl;
		cout << "\t\t\t     b. MYKAS" << endl;
		cout << "\t\t\t     c. MYPR" << endl;
		cout << "\t\t\t     d. Passport" << endl;
		cout << "\t\t\t  Please enter your option: "; getline(cin, dadDocType);

		if (dadDocType == "a" || dadDocType == "A")
			dadDocType = "MYKAD";
		else if (dadDocType == "b" || dadDocType == "B")
			dadDocType = "MYKAS";
		else if (dadDocType == "c" || dadDocType == "C")
			dadDocType = "MYPR";
		else if (dadDocType == "d" || dadDocType == "D")
			dadDocType = "Passport";
		else if (dadDocType == "0")
			main();
		else {
			cout << "\t\t\t  Input invalid. Try Again.\n";
			goto doctypedad;
		}

		if (dadDocType == "MYKAD" || dadDocType == "MYKAS" || dadDocType == "MYPR") {
		dadic:
			cout << "\n\t\t\t  3. Identification Card Number (without '-'): ";
			getline(cin, dadNo);
			if (dadNo == "0")
				main();
			else if (!check_digit(dadNo) || dadNo.size() != 12) {
				cout << "\n\t\t\t  The IC number should be 12 digits only. Please enter again.\n";  //Malaysian IC No is fixed to 12 integer only
				goto dadic;
			}
			if (dadNo[11] % 2 == 0) {
				cout << "\n\t\t\t  Invalid Input. Last Digit for a father's IC should be odd number.\n\t\t\t  Please try again.\n";
				goto dadic;
			}
		}
		else if (dadDocType == "Passport") {
		dadpassport:
			cout << "\n\t\t\t  3. Passport Number: ";
			cin.ignore(); getline(cin, dadNo);
			if (dadNo == "0")
				main();
			else if (dadNo == "") {
				cout << "\n\t\t\t  Please input the passport number\n";
				goto dadpassport;
			}
		}


	citizendad:
		cout << "\t\t\t  4. Citizenship" << endl;
		cout << "\t\t\t     a. Citizen" << endl;
		cout << "\t\t\t     b. Permanent-citizen" << endl;
		cout << "\t\t\t     c. Non-citizen" << endl;
		cout << "\t\t\t  Please enter your option: "; getline(cin, dadCitizen);

		if (dadCitizen == "a" || dadCitizen == "A")
			dadCitizen = "Citizen";
		else if (dadCitizen == "b" || dadCitizen == "B")
			dadCitizen = "Permanent-citizen";
		else if (dadCitizen == "c" || dadCitizen == "C")
			dadCitizen = "Non-citizen";
		else if (dadCitizen == "0")
			main();
		else {
			cout << "\t\t\t  Invalid input. Please try again\n";
			goto citizendad;
		}

	dadage:
		cout << "\n\t\t\t  5. Age: ";
		if (dadDocType == "MYKAD" || dadDocType == "MYKAS" || dadDocType == "MYPR") {

			char y[2], m[2], d[2];
			int i, j;
			for (i = 0; i < 2; i++)
				y[i] = dadNo[i];

			for (i = 2, j = 0; i < 4; i++, j++)
				m[j] = dadNo[i];

			for (i = 4, j = 0; i < 6; i++, j++)
				d[j] = dadNo[i];

			int birth_year = atoi(y) + 2000;
			int birth_month = atoi(m);
			int birth_date = atoi(d);
			findAge(current_date, current_month, current_year, birth_date, birth_month, birth_year);
		}
		else if (dadDocType == "Passport") {
			getline(cin, dadAge);
			if (dadAge == "") {
				cout << "\t\t\t  Please input the father's age.\n";
				goto dadage;
			}
			else if (dadAge == "0")
				main();
			else if (!check_digit(dadAge) || dadAge.size() != 2) {
				cout << "\t\t\t Something wrong with the input. Please try again.\n";
				goto dadage;
			}
		}
	dadrace:
		cout << "\n\t\t\t  6. Race\n";
		cout << "\t\t\t     a. Malay\n"; cout << "\t\t\t     b. Chinese\n"; cout << "\t\t\t     c. Indian\n"; cout << "\t\t\t     d. others\n";
		cout << "\t\t\t  Please enter your option: "; getline(cin, dadRace);
		if (dadRace == "a" || dadRace == "A")
			dadRace = "Malay";
		else if (dadRace == "b" || dadRace == "B")
			dadRace = "Chinese";
		else if (dadRace == "c" || dadRace == "C")
			dadRace = "Indian";
		else if (dadRace == "d" || dadRace == "D")
		{
		dadrace2:
			cout << "\t\t\t  Enter your race: "; getline(cin, dadRace);
			if (dadRace == "0")
				main();
			else if (dadRace == "") {
				cout << "\t\t\t  Please input the father's race.\n";
				goto dadrace2;
			}
		}
		else if (dadRace == "0")
			main();
		else {
			cout << "\t\t\t  Invalid input. Please try again.\n";
			goto dadrace;
		}

	dadreli:
		cout << "\t\t\t  7. Religion: ";  getline(cin, dadReligion);
		if (dadReligion == "") {
			cout << "\t\t\t  Please input the father's religion.\n";
			goto dadreli;
		}
		else if (dadReligion == "0")
			main();
	dadmobile:
		cout << "\t\t\t  8. Mobile Number: "; getline(cin, dadMobile);
		if (dadMobile == "") {
			cout << "\t\t\t  Please input the father's mobile no.\n";
			goto dadmobile;
		}
		else if (dadMobile == "0")
			main();
	dadjob:
		cout << "\t\t\t  9. Job: "; getline(cin, dadJob);
		if (dadJob == "") {
			cout << "\t\t\t  Please input the father's job.\n";
			goto dadjob;
		}
		else if (dadJob == "0")
			main();

	educationdad:
		cout << "\t\t\t  10. Education Status" << endl;
		cout << "\t\t\t      a. Primary School " << endl;
		cout << "\t\t\t      b. Secondary School" << endl;
		cout << "\t\t\t      c. Pre-University" << endl;
		cout << "\t\t\t      d. Higher Education" << endl;
		cout << "\t\t\t      e. Non specified" << endl;
		cout << "\t\t\t  Please enter your option: "; getline(cin, dadEdu);

		if (dadEdu == "a" || dadEdu == "A")
			dadEdu = "Primary School";
		else if (dadEdu == "b" || dadEdu == "B")
			dadEdu = "Secondary School";
		else if (dadEdu == "c" || dadEdu == "C")
			dadEdu = "Pre-University";
		else if (dadEdu == "d" || dadEdu == "D")
			dadEdu = "Higher Education";
		else if (dadEdu == "e" || dadEdu == "E")
			dadEdu = "Non specified";
		else if (dadEdu == "0")
			main();
		else {
			cout << "\t\t\t  Invalid input. Please try again\n";
			goto educationdad;
		}
	}

	correct:
	char correct;
	cout << "\n\t\t\t  All the information you input will be recorded inside the birth certificate.\n\t\t\t  Are you sure all the information is correct?(Y/N): "; cin >> correct;
	if (correct == 'y' || correct == 'Y')
		goto yes;
	else if (correct == 'n' || correct == 'N')
	{
		cout << "\n\t\t\t  Please refill all the information to ensure the information are correct.\n\n\t\t\t\t\t\t";
		system("pause");
		birthRes();
	}
	else {
		cout << "\n\t\t\t  Invalid input. Please try again.\n";
		goto correct;
	}

yes:
	int qstate1, qstate2, qstate3;
	if (marital == "Married") {
		string insertFather_query = "insert into father(Father_name, Father_DocType, Father_DocNo, Father_Citizen, Father_Age, Father_Race, Father_Religion, Father_MobileNo, Father_Job, Father_edu) values('"
			+ father + "','" + dadDocType + "','" + dadNo + "','" + dadCitizen + "','" + dadAge + "','" + dadRace + "','" + dadReligion + "','" + dadMobile + "','" + dadJob + "','" + dadEdu + "')";

		string insertMother_query = "insert into mother(Mother_name, Mother_DocType, Mother_DocNo, Mother_Citizen, Mother_Age, Mother_Race, Mother_Religion, Mother_MobileNo, Mother_Job, Mother_edu, Mother_marital, Mother_Address, Mother_Postcode, Mother_City, Mother_State) values('"
			+ mother + "','" + mumDocType + "','" + mumNo + "','" + mumCitizen + "','" + mumAge + "','" + mumRace + "','" + mumReligion + "','" + mumMobile + +"','" + mumJob + "','"
			+ mumEdu + +"','" + marital + "','" + addr + "','" + postcode + "','" + city + +"','" + state + "')";

		string currmumNo = mumNo;
		string currdadNo = dadNo;
		string currNo = userNo;
		string status = "PENDING"; //set default status
		string insertBaby_query = "insert into certificate(Baby_name, Gender, dateOfBirth, timeOfBirth, PlaceOfBirth, User_DocNo, Mother_DocNo, Father_DocNo, Status) values('"
			+ baby + "','" + gender + "','" + dob + "','" + tob + "','" + place + "','" + currNo + "','" + currmumNo + "','" + currdadNo + "','" + status + "')";

		const char* q_father = insertFather_query.c_str();
		qstate2 = mysql_query(conn, q_father);

		const char* q_mother = insertMother_query.c_str();
		qstate3 = mysql_query(conn, q_mother);

		const char* q_baby = insertBaby_query.c_str();
		qstate1 = mysql_query(conn, q_baby);

		if (!qstate1 && !qstate2 && !qstate3)
		{
			cout << "\n\t\t\t  The registration for baby " << baby << " is success." << endl;
			cout << "\t\t\t  Press enter to back to the menu.\n\n\t\t\t     ";
			system("pause");
			UsrMain();
		}
		else {
			cout << "\t\t\t  Query Execution Probrem!" << mysql_errno(conn) << endl;
		}
	}
	else
	{
		string insertMother_query = "insert into mother(Mother_name, Mother_DocType, Mother_DocNo, Mother_Citizen, Mother_Age, Mother_Race, Mother_Religion, Mother_MobileNo, Mother_Job, Mother_edu, Mother_marital, Mother_Address, Mother_Postcode, Mother_City, Mother_State) values('"
			+ mother + "','" + mumDocType + "','" + mumNo + "','" + mumCitizen + "','" + mumAge + "','" + mumRace + "','" + mumReligion + "','" + mumMobile + +"','" + mumJob + "','"
			+ mumEdu + +"','" + marital + "','" + addr + "','" + postcode + "','" + city + +"','" + state + "')";

		string currmumNo = mumNo;
		string currNo = userNo;
		string status = "PENDING"; //set default status
		string insertBaby_query = "insert into certificate(Baby_name, Gender, dateOfBirth, timeOfBirth, PlaceOfBirth, User_DocNo, Mother_DocNo, Status) values('"
			+ baby + "','" + gender + "','" + dob + "','" + tob + "','" + place + "','" + currNo + "','" + currmumNo + "','" + status + "')";

		const char* q_mother = insertMother_query.c_str();
		qstate3 = mysql_query(conn, q_mother);

		const char* q_baby = insertBaby_query.c_str();
		qstate1 = mysql_query(conn, q_baby);

		if (!qstate1 && !qstate3)
		{
			cout << "\n\t\t\t  The registration for baby " << baby << " is success." << endl;
			cout << "\t\t\t  Press enter to back to the menu.\n\n\t\t\t     ";
			system("pause");
			UsrMain();
		}
		else {
			cout << "\t\t\t  Query Execution Probrem!" << mysql_errno(conn) << endl;
		}
	}
}

void checkStatus()
{
	system("cls");
	list ls;
	head();
	cout << "\t\t\t(Enter 0 to back to main menu)\n\n";

	string currNo = userNo;
	string seacrh_cert = "select * from certificate where User_DocNo = '" + currNo + "'";
	const char* q = seacrh_cert.c_str();
	qstate = mysql_query(conn, q);
	cout << "\n\t\t\t\t\t      " << "Check Application Status" << endl;

	ls.applicationlist();

	cout << "\n\t\t\t\t\t\t";
	system("pause");
	UsrMain();
}

void birthCert() {
	system("cls");
	list ls;

	head();
	cout << "\t\t\t(Enter 0 to back to main menu)\n\n";

	cout << "\t\t\t\t\t       View Birth Certificate" << endl;

	string currNo = userNo;
	string seacrh_cert = "select * from certificate where User_DocNo = '" + currNo + "'";
	qstate = mysql_query(conn, seacrh_cert.c_str());
	ls.applicationlist();

	cout << "\n\t\t\t\tEnter mother's ID Number to view the certificate: "; cin >> mumNo;
	if (mumNo == "0")
		UsrMain();

	string check = "select * from certificate where User_DocNo = '" + currNo + "' and Mother_DocNo = '" + mumNo + "'";
	const char* q = check.c_str();
	qstate = mysql_query(conn, q);
	res = mysql_store_result(conn);
	
	if (res->row_count == 1) {
		row = mysql_fetch_row(res);
		string status = row[10];
		if (status == "REJECTED") {
			cout << "\n\t\t\t\t\tThis application has been rejected because:" << "\n\t\t\t\t\t" << row[11];
			cout << "\n\n\t\t\t\t\tSo you can't view the birth certificate\n";
		}
		else if (status == "PENDING") {
			cout << "\n\t\t\t\t\t    This application status is still pending.\n";
			cout << "\n\t\t\t\t\t       Check the certificate once approved.";
		}
		else {
			system("cls");
			string name = row[1]; int x = name.size();
			cout << "\n\n\t\t\t " << setfill('=') << right << setw(x + 60) << "=" << endl;
			cout << "\t\t\t|                            BIRTH CERTIFICATE" << setfill(' ') << setw(x + 16) << '|' << endl;
			cout << "\t\t\t " << setfill('=') << setw(x + 60) << "=" << endl;
			cout << "\t\t\t|                                  CHILD" << setfill(' ') << setw(x + 22) << '|' << endl;
			cout << "\t\t\t " << setfill('-') << setw(x + 60) << "-" << endl;
			cout << "\t\t\t|Full Name               " << left << setfill(' ') << setw(x + 36) << row[1] << "|" << endl;
			cout << "\t\t\t|Date of Birth           " << left << setfill(' ') << setw(x + 36) << row[2] << "|" << endl;
			cout << "\t\t\t|Time of Birth           " << left << setfill(' ') << setw(x + 36) << row[3] << "|" << endl;
			cout << "\t\t\t|Place of Birth          " << left << setfill(' ') << setw(x + 36) << row[5] << "|" << endl;
			cout << "\t\t\t|Gender                  " << left << setfill(' ') << setw(x + 36) << row[4] << "|" << endl;
			string mumNo = row[7] ? row[7] : "NULL";
			string dadNo = row[8] ? row[8] : "NULL";

			cout << "\t\t\t " << setfill('-') << right << setw(x + 60) << "-" << endl;
			cout << "\t\t\t|                                 Father" << setfill(' ') << setw(x + 22) << '|' << endl;
			cout << "\t\t\t " << setfill('-') << setw(x + 60) << "-" << endl;

			string dad = "select * from father where Father_DocNo = '" + dadNo + "'";
			const char* q1 = dad.c_str();
			qstate1 = mysql_query(conn, q1);
			res = mysql_store_result(conn);
			if (res->row_count == 1) {
				row = mysql_fetch_row(res);
				cout << "\t\t\t|Full Name               " << left << setfill(' ') << setw(x + 36) << row[0] << "|" << endl;
				cout << "\t\t\t|Identification Card No  " << left << setfill(' ') << setw(x + 36) << row[2] << "|" << endl;
				cout << "\t\t\t|Age                     " << left << setfill(' ') << setw(x + 36) << row[4] << "|" << endl;
				cout << "\t\t\t|Citizenship             " << left << setfill(' ') << setw(x + 36) << row[3] << "|" << endl;
				cout << "\t\t\t|Race                    " << left << setfill(' ') << setw(x + 36) << row[5] << "|" << endl;
				cout << "\t\t\t|Religion                " << left << setfill(' ') << setw(x + 36) << row[6] << "|" << endl;
			}
			else {
				cout << "\t\t\t|Full Name" << setfill(' ') << setw(x + 52) << '|' << endl;
				cout << "\t\t\t|Identification Card No" << setfill(' ') << setw(x + 39) << '|' << endl;
				cout << "\t\t\t|Age" << setfill(' ') << setw(x + 58) << '|' << endl;
				cout << "\t\t\t|Citizenship" << setfill(' ') << setw(x + 50) << '|' << endl;
				cout << "\t\t\t|Race" << setfill(' ') << setw(x + 57) << '|' << endl;
				cout << "\t\t\t|Religion" << setfill(' ') << setw(x + 53) << '|' << endl;
			}

			string mum = "select * from mother where Mother_DocNo = '" + mumNo + "'";
			const char* q2 = mum.c_str();
			qstate2 = mysql_query(conn, q2);
			res = mysql_store_result(conn);
			if (res->row_count == 1) {
				row = mysql_fetch_row(res);
				cout << "\t\t\t " << setfill('-') << right << setw(x + 60) << "-" << endl;
				cout << "\t\t\t|                                 Mother" << setfill(' ') << setw(x + 22) << '|' << endl;
				cout << "\t\t\t " << setfill('-') << setw(x + 60) << "-" << endl;
				string add = row[11]; string post = row[12]; string city = row[13]; string state = row[14];
				string full = add + ", " + post + " " + city + ", " + state;
				cout << "\t\t\t|Full Name               " << left << setfill(' ') << setw(x + 36) << row[0] << '|' << endl;
				cout << "\t\t\t|Identification Card No  " << left << setfill(' ') << setw(x + 36) << row[2] << '|' << endl;
				cout << "\t\t\t|Age                     " << left << setfill(' ') << setw(x + 36) << row[4] << '|' << endl;
				cout << "\t\t\t|Citizenship             " << left << setfill(' ') << setw(x + 36) << row[3] << '|' << endl;
				cout << "\t\t\t|Race                    " << left << setfill(' ') << setw(x + 36) << row[5] << '|' << endl;
				cout << "\t\t\t|Religion                " << left << setfill(' ') << setw(x + 36) << row[6] << '|' << endl;
				cout << "\t\t\t|Address                 " << left << setfill(' ') << setw(x + 36) << full << '|' << endl;
				cout << "\t\t\t " << setfill('=') << setw(x + 60) << "=" << endl;
			}
		}
	}
	else
	{
		cout << "\n\t\t\t\tThe application does not exist. Please try again.\n\n\t\t\t\t\t";
		system("pause");
		birthCert();
	}
	cout << "\n\n\t\t\t\t\t\t";
	system("pause");
	UsrMain();
}

void deleteApplication() {
	system("cls");
	list ls;
	string currNo = userNo;
	string seacrh_cert = "select * from certificate where User_DocNo = '" + currNo + "'";
	const char* q1 = seacrh_cert.c_str();
	qstate = mysql_query(conn, q1);
	ls.applicationlist();
	cout << "\t\t(Enter 0 to back to main menu)\n";

id:
	cout << "\n\t\t   Enter the mother's ID Number for the application that you want to delete: "; cin >> mumNo;
	if (mumNo == "0")
		UsrMain();
	string seacrh_mum = "select * from certificate where User_DocNo  = '" + currNo + "' and Mother_DocNo = '" + mumNo + "'";

	const char* s = seacrh_mum.c_str();
	qstate = mysql_query(conn, s);
	res = mysql_store_result(conn);

	if (res->row_count == 1) {
		row = mysql_fetch_row(res);
	confirm:
		string certId = row[0];
		string mumNo = row[7] ? row[7] : "NULL";
		string dadNo = row[8] ? row[8] : "NULL";
		string status = row[10];
		char choice;

		if (status == "APPROVED") {
			cout << "\n\t\t   This application has been approved. You are not allowed to delete this application.\n\n\t\t\t\t\t\t";
		}
		else {
			cout << "\n\t\tAre you sure you want to delete all the data for mother with ID No of " + mumNo + " (Y/N): "; cin >> choice;

			if (choice == 'y' || choice == 'Y') {
				string delete_cert = "delete from certificate where Cert_ID = '" + certId + "'";
				const char* c = delete_cert.c_str();
				qstate1 = mysql_query(conn, c);

				string delete_mum = "delete from mother where Mother_DocNo = '" + mumNo + "'";
				const char* m = delete_mum.c_str();
				qstate2 = mysql_query(conn, m);

				string delete_dad = "delete from father where Father_DocNo = '" + dadNo + "'";
				const char* d = delete_dad.c_str();
				qstate3 = mysql_query(conn, d);

				if (!qstate1 && !qstate2 && !qstate3)
				{
					cout << endl << "\t\t\t\tSuccessfully DELETED! Press enter to back to main menu.\n\t\t\t\t\t\t";
					system("pause");
					UsrMain();
				}
				else
					cout << "Query Execution Probrem!" << mysql_errno(conn) << endl;
			}
			else if (choice == 'n' || choice == 'N')
			{
				cout << "\n\t\t   You decide not to delete this application. You will be redirected back to main menu.\n\n\t\t\t\t\t\t";
				system("pause");
				UsrMain();
			}
			else
			{
				cout << "\n\t\t\t\t\t    Wrong input. Please try again." << endl;
				goto confirm;
			}
		}
	}
	else {
		cout << "\n\t\t\t\tThe application does not exist. Please try again.\n\n\t\t\t\t\t";
	}
	system("pause");
	deleteApplication();
}

void editPsw() {
	PasswordStatus ps;
	system("cls");
	head();
	string currNo = userNo;
	char pw[15], pass[15]; int i = 0, j = 0; char ch;

	cout << "\n\t\t\t\t\tEnter Old Password: ";
	while ((ch = _getch()) != 13)
	{
		pw[i] = ch;
		i++;
		printf("*");
	}
	pw[i] = '\0';

	string search = "select * from users where User_DocNo = '" + currNo + "' and  User_Psw = '" + pw + "'";
	const char* s = search.c_str();
	qstate = mysql_query(conn, s);
	res = mysql_store_result(conn);
	if (res->row_count == 1) {
		row = mysql_fetch_row(res);
		cout << "\n\n\t\t\t\t\t\tEDIT PASSWORD" << "\n";

		cout << "\n\t\t\t\tIn order to protect your account, make sure your \n\t\t\t\t   password fulfill the following requirement\n";
		cout << "\n\t\t\t\t\t-More than 8 characters" << endl << "\t\t\t\t\t-A lowercase letter" << endl
			<< "\t\t\t\t\t-An uppercase letter" << endl << "\t\t\t\t\t-A number"
			<< endl << "\n\t\t\t\tPlease enter your new password: ";

		while ((ch = _getch()) != 13)
		{
			pass[j] = ch;
			j++;
			printf("*");
		}
		pass[j] = '\0';

		ps.checkValidity(pass);
		ps.ShowStatus();
		if (!ps.IsTotalyValid()) {
			cout << "\n\t\tYour password does not fulfill the requirement. Please try another password.\n\n\t\t\t\t";
			system("pause");
			editPsw();
		}
		string psw = pass;
		string update_query = "update users set User_Psw = '" + psw + "' where User_DocNo = '" + currNo + "'";
		const char* q = update_query.c_str();
		qstate = mysql_query(conn, q);

		if (!qstate) {
			cout << "\n\t\t\t\t\t  The password is updated.\n\t\t\t\t\t      Try to login now.\n\n\t\t\t\t\t";
			system("pause");
			UsrLog();
		}
		else
		{
			cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
		}
	}
	else
	{
		cout << "\n\n\t\t\t\t\tWrong password. Please try again.\n\n\t\t\t\t\t";
		system("pause");
		editPsw();
	}
}

//STAFF
void staffLog()
{
	system("cls");

	string staffPsw;
	cout << "\n\n\t\t\t\t+------------------------------------------+" << endl;
	cout << "\t\t\t\t|             STAFF LOGIN PAGE             |" << endl;
	cout << "\t\t\t\t+------------------------------------------+" << endl;
	cout << "\n\t\t\t\tHi! Please enter your IC and Password to login." << endl;
	cout << "\t\t\t\t(Enter 0 to back to main page)\n\n";
	cout << "\t\t\t\t\tIC No (without '-'): ";
	cin.ignore();
	getline(cin, staffIc);
	if (staffIc == "0")
		main();
	cout << "\t\t\t\t\tPassword: ";
	
	char ch;
	while ((ch = _getch()) != 13)
	{
		staffPsw += ch;
		cout << '*';
	}

	string checkstaff = "select * from staff where Staff_ICNo = '" + staffIc + "' and Staff_Psw = '" + staffPsw + "'";
	const char* qn = checkstaff.c_str();
	qstate = mysql_query(conn, qn);
	if (!qstate) {
		res = mysql_store_result(conn);
		if ((row = mysql_fetch_row(res)))
		{
			cout << "\n\n\t\t\t\t\t\tLOGIN Success!!\n\n\t\t\t\t\t";
			system("pause");
			staffMain();
		}
		else
		{
			cout << "\n\t\t\t       Fail to login. Please check your IC and password.\n\n\t\t\t\t\t";
			system("pause");
			staffLog();
		}
	}
}

void staffMain()
{
	system("cls");
	head();
	
	cout << "\t\t\t\t    Hi " << staffIc << "! Welcome to staff main page.\n\n";
	cout << "\t\t\t\t\t   1. View application" << endl;
	cout << "\t\t\t\t\t   2. BRS report" << endl;
	cout << "\t\t\t\t\t   3. Exit" << endl << endl;
	cout << "\t\t\t\t What you want to do here? Please enter your option: ";
	cin >> choose;

	switch (choose) {
	case 1:
		viewApplication();
		break;
	case 2:
		brsReport();
		break;
	case 3:
		main();
		break;
	default:
		cout << "\t\t\t\t\t   Wrong input. Please enter again." << endl;
		system("pause");
		staffMain();
		break;
	}
}

void viewApplication() {
	system("cls");
	list ls;
	ls.fullList();
	cout << "(Enter 0 to back to main menu)\n\n";
	cout << "\n\t\tEnter the Cert Id for the application that you want to view the details: "; 
	cin >> certId;
	if (certId == "0")
		staffMain();

	string search_cert = "select * from certificate where Cert_ID = '" + certId + "'";
	const char* c = search_cert.c_str();
	qstate = mysql_query(conn, c);
	res = mysql_store_result(conn);
	if (res->row_count == 1) {
		ls.detail();
	}
	else
	{
		cout << "\n\t\t\t\tThe Cert ID doesn't exits. Please retry.\n";
		system("pause");
		viewApplication();
	}
	
	string checkp = "select * from certificate where Cert_ID = '" + certId + "' and Status = 'PENDING'";
	const char* p = checkp.c_str();
	qstate1 = mysql_query(conn, p);
	if (!qstate1) {
		res = mysql_store_result(conn);
		if (row = mysql_fetch_row(res))
		{
		
			char choice;
			cout << "\n\t\t\t\t\tThe Status of this Application: PENDING\n";
		update:
			cout << "\n\t\t\tWould you like to update the details of this aplication? (y/n): ";
			cin >> choice;
			if (choice == 'y' || choice == 'Y')
			{
				char decision;
			approve:
				cout << "\n\t\t\tIf you approve this application, enter Y. If rejected, enter N. \n\t\t\t\t\t   Please enter your decision: ";
				cin >> decision;
				if (decision == 'y' || decision == 'Y') {
					string status = "APPROVED";
					string update_query = "update certificate set Status = '" + status + "', Staff_ICNo = '" + staffIc + "' where Cert_ID = '" + certId + "'";
					const char* q = update_query.c_str();
					qstate = mysql_query(conn, q);
					cout << "\t\t\t\t\tThis Application had been APPROVED!" << "\n\n\t\t\t\t\t";
					system("pause");
					staffMain();
				}
				else if (decision == 'n' || decision == 'N') {
					string reason;
					reason:
					cout << "\n\t\t\tPlease provide the reason why you reject this application: ";
					cin.ignore(); getline(cin, reason);
					if (reason == "") {
						cout << "\t\t\t\t\tYou should enter a reason. Please try again." << endl;
						goto reason;
					}
					string status = "REJECTED";
					string update_query = "update certificate set Status = '" + status + "', Staff_ICNo = '" + staffIc + "', Reason = '" + reason + "' where Cert_ID = '" + certId + "'";
					const char* q = update_query.c_str();
					qstate = mysql_query(conn, q);
					cout << "\t\t\t\t\tThis Application had been REJECTED!" << "\n\n\t\t\t\t\t";
					system("pause");
					staffMain();
				}
				else
				{
					cout << "\n\t\t\t\tYou should only enter 'Y' or 'N'. Please try again.\n";
					goto approve;
				}
			}
			else if (choice == 'n' || choice == 'N')
			{
				cout << "\n\t\t\t\tThank you. You will be redirected back to main menu.\n\t\t\t\t\t";
				system("pause");
				staffMain();
			}
			else
			{
				cout << "\n\t\t\t\tYou should only enter 'Y' or 'N'. Please try again.\n";
				goto update;
			}
		}
	}

	string checka = "select * from certificate where Cert_ID = '" + certId + "' and Status = 'APPROVED'";
	const char* a = checka.c_str();
	qstate2 = mysql_query(conn, a);
	if (!qstate2) {
		res = mysql_store_result(conn);
		if (row = mysql_fetch_row(res))
		{
			cout << "\t\t\t\t\tThe Status of this Application: " << row[10] << "\n\n\t\t\t\t\t";
		}
	}

	string checkr = "select * from certificate where Cert_ID = '" + certId + "' and Status = 'REJECTED'";
	const char* r = checkr.c_str();
	qstate3 = mysql_query(conn, r);
	if (!qstate3) {
		res = mysql_store_result(conn);
		if (row = mysql_fetch_row(res))
		{
			cout << "\t\t\t\t\tThe Status of this Application: " << row[10] << endl;
			cout << "\n\t\t\t\tThe Reason is: " << row[11] << "\n\n\t\t\t\t\t";
		}
	}

	cout << "\n\t\t\t\t\t";
	system("pause");
	staffMain();
}

void brsReport() {
	system("cls");
	cout << "\n\n\t\t\t+------------------------------------------------------------+" << endl;
	cout << "\t\t\t|                         BRS REPORT                         |" << endl;
	cout << "\t\t\t+------------------------------------------------------------+" << endl;
	cout << "\n\t\t\t\t  What type of report you want to choose here?\n\n";
	cout << "\t\t\t\t     1. Report of new born baby by month" << endl;
	cout << "\t\t\t\t     2. Report of approved application" << endl;
	cout << "\t\t\t\t     3. Report of new application" << endl;
	cout << "\t\t\t\t     4. Exit" << endl;
	cout << "\n\t\t\t\t\tPlease enter your option: "; cin >> choose;
	switch (choose) {
	case 1:
		rMonth();
		break;
	case 2:
		approved();
		break;
	case 3:
		pending();
		break;
	case 4:
		staffMain();
		break;
	default:
		cout << "Wrong input. Please enter again." << endl;
		system("pause");
		brsReport();
		break;
	}
}

void rMonth() {
	system("cls");
	cout << "\n\n\t\t\t+------------------------------------------------------------+" << endl;
	cout << "\t\t\t|                         BRS REPORT                         |" << endl;
	cout << "\t\t\t+------------------------------------------------------------+\n\n";
	string month, year;
	cout << "\t\t\t\t\tReport of New Born Baby by Month" << endl << endl;
	month:
	cout << "\t\t\t\t\t\tEnter Month (1-12): "; cin >> month;
	if (!check_digit(month)) {
		cout << "\n\t\t\t\t\t\tCheck your input. Please try again\n\n";
		goto month;
	}
	else if (month != "1" && month != "2" && month != "3" && month != "4" && month != "5" && month != "6" && month != "7" && month != "8" && month != "9" && month != "10" && month != "11" && month != "12") {
		cout << "\n\t\t\t\t\tCheck your input. Please try again\n\n";
		goto month;
	}
	year:
	cout << "\t\t\t\t\t\tEnter Year: "; cin >> year;
	string countM = "SELECT COUNT(*) FROM certificate where Status = 'APPROVED' AND YEAR(dateOfBirth) = " + year + " AND MONTH(dateOfBirth) = " + month;
	const char* c = countM.c_str();
	qstate = mysql_query(conn, c);
	if (month == "1") month = "January";
	else if (month == "2") month = "Febuary";
	else if (month == "3") month = "March";
	else if (month == "4") month = "April";
	else if (month == "5") month = "May";
	else if (month == "6") month = "June";
	else if (month == "7") month = "July";
	else if (month == "8") month = "August";
	else if (month == "9") month = "September";
	else if (month == "10") month = "October";
	else if (month == "11") month = "November";
	else if (month == "12") month = "December";

	if (!qstate) {
		res = mysql_store_result(conn);
		if (row = mysql_fetch_row(res)) {
			cout << "\n\t\t\t\tThe number of new born baby in " << month << " " << year << " is: " << row[0] << "\n\n\t\t\t\t\t";
		}
	}
	else {
		cout << "\n\t\t\t\t\t\tNo Record Found.\n\n";
	}
	system("pause");
	staffMain();
}

void approved(){
	system("cls");
	cout << "\n\n\t\t\t+------------------------------------------------------------+" << endl;
	cout << "\t\t\t|                         BRS REPORT                         |" << endl;
	cout << "\t\t\t+------------------------------------------------------------+\n";
	cout << "\n\n\t\t\t\t\tReport of approved application" << endl << endl;

	string countM = "SELECT count(Status) from certificate where Status = 'APPROVED'";
	const char* c = countM.c_str();
	qstate = mysql_query(conn, c);

	if (!qstate) {
		res = mysql_store_result(conn);
		if (row = mysql_fetch_row(res)) {
			cout << "\n\t\t\t\t    The number of approved application is: " << row[0] << "\n\n\n\t\t\t\t\t";
		}
	}
	else {
		cout << "\n\t\t\t\t\t\tNo Record Found.\n\n";
	}
	system("pause");
	staffMain();
}

void pending() {
	system("cls");
	cout << "\n\n\t\t\t+------------------------------------------------------------+" << endl;
	cout << "\t\t\t|                         BRS REPORT                         |" << endl;
	cout << "\t\t\t+------------------------------------------------------------+\n";
	cout << "\n\n\t\t\t\t\tReport of new application" << endl << endl;

	string countM = "SELECT count(Status) from certificate where Status = 'PENDING'";
	const char* c = countM.c_str();
	qstate = mysql_query(conn, c);

	if (!qstate) {
		res = mysql_store_result(conn);
		if (row = mysql_fetch_row(res)) {
			cout << "\n\t\t\t\t    The number of new application is: " << row[0] << "\n\n\n\t\t\t\t\t";
		}
	}
	else {
		cout << "\n\t\t\t\t\t\tNo Record Found.\n\n";
	}
	system("pause");
	staffMain();
}

//ADMIN
void adminLog()
{
	system("cls");
	int i = 0;
	string adminId, adminPsw;
	cout << "\n\n\t\t\t\t+------------------------------------------+" << endl;
	cout << "\t\t\t\t|             ADMIN LOGIN PAGE             |" << endl;
	cout << "\t\t\t\t+------------------------------------------+\n\n";
	cout << "\t\t\t    Hi admin! Please enter your ID and Password to login." << endl;
	cout << "\t\t\t    (Enter 0 to back to main page)\n\n";
	cout << "\t\t\t\t\tAdmin ID: ";
	cin.ignore();
	getline(cin, adminId);
	if (adminId == "0")
		main();
	
	cout << "\t\t\t\t\tPassword: ";
	char ch;
	while ((ch = _getch()) != 13)
	{
		adminPsw += ch;
		cout << '*';
	}

	string checkadmin = "SELECT * FROM admin WHERE Admin_ID = '" + adminId + "' AND Admin_Psw = '" + adminPsw + "'";
	const char* qn = checkadmin.c_str();
	qstate = mysql_query(conn, qn);
	if (!qstate) {
		res = mysql_store_result(conn);
		if (row = mysql_fetch_row(res))
		{
			cout << "\n\n\t\t\t\t\t       LOGIN Success!!\n\n\t\t\t\t\t";
			system("pause");

			adminMain();
		}
		else
		{
			cout << "\n\n\t\t\t      Fail to login. Please check your ID and password.\n\n\t\t\t\t\t";
			system("pause"); 
			adminLog();
		}
	}
}

void adminMain()
{
	system("cls");
	int choice;
	head(); 
	cout << "\n\t\t\t\t      Hi Admin! Welcome to Admin main page!" << endl;
	cout << "\t\t\t      As an admin, what would you wish to make change here?\n\n";
	cout << "\t\t\t\t\t 1. Register a staff" << endl;
	cout << "\t\t\t\t\t 2. Update a staff information" << endl;
	cout << "\t\t\t\t\t 3. Delete Staff" << endl;
	cout << "\t\t\t\t\t 4. Exit" << endl << endl;

	cout << "\t\t\t\t\tPlease enter your option: ";
	cin >> choice;

	switch (choice) {
	case 1:
		resStaff();
		break;
	case 2:
		updateStaff();
		break;
	case 3:
		deleteStaff();
		break;
	case 4:
		main();
		break;
	default:
		cout << "Wrong input. Please enter again." << endl;
		system("pause");
		adminMain();
		break;
	}
}

void resStaff() {
	system("cls");
	Email e; PasswordStatus ps;
	string icno, name, email, mobile, address, postcode, city, state;
	char psw[15];
	head();
	cout << "\n\t\t\t\t\t\tStaff Registration\n";
	cout << "\n\t\t\t\t      Please fill in the staff information.\n";
	cout << "\t\t\t\t      (Enter 0 to back to main menu)\n\n";

	cin.ignore();
ic:
	cout << "\t\t\t\t1. Identification Card Number (without '-'): ";
	getline(cin, icno);
	if (icno == "0")
		adminMain();
	else if (icno == "") {
		cout << "\t\t\t\tPlease input the IC.\n";
		goto ic;
	}
	else if (icno.size() != 12 || !check_digit(icno)) {
		cout << "\t\t\t\tThe IC number should be 12 digits only. Please enter again.\n\n";  //Malaysian IC No is fixed to 12 integer only
		goto ic;
	}
	name:
	cout << "\t\t\t\t2. Name: ";
	getline(cin, name);
	if (name == "") {
		cout << "\t\t\t\tPlease input the name.\n";
		goto name;
	}
	else if (name == "0")
		adminMain();
	email:
	cout << "\t\t\t\t3. E-mail: ";
	getline(cin, email);
	if (email == "") {
		cout << "\t\t\t\tPlease input the email.\n";
		goto email;
	}
	else if (email == "0")
		adminMain();
	else if (!e.is_valid(email)) {
		cout << "\t\t\t\tYour email is invalid. Please try again.\n";
		goto email;
	}
	mobile:
	cout << "\t\t\t\t4. Mobile: ";
	getline(cin, mobile);
	if (mobile == "") {
		cout << "\t\t\t\tPlease input the mobile.\n";
		goto mobile;
	}
	else if (mobile == "0")
		adminMain();
	address:
	cout << "\t\t\t\t5. Address: ";
	getline(cin, address);
	if (address == "") {
		cout << "\t\t\t\tPlease input the address.\n";
		goto address;
	}
	else if (address == "0")
		adminMain();
	postcode:
	cout << "\t\t\t\t6. Postcode: ";
	getline(cin, postcode);
	if(postcode == "") {
		cout << "\t\t\t\tPlease input the postcode.\n";
		goto postcode;
	}
	else if (postcode == "0")
		adminMain();
	else if (!check_digit(postcode)) {
		cout << "\t\t\t\tSomething wrong with your input. A postcode should be 5 digit.\n\t\t\t\tPlease try again.\n";
		goto postcode;
	}
	else if (postcode.size() != 5) {
		cout << "\t\t\t\tSomething wrong with your input. A postcode should be 5 digit.\n\t\t\t\tPlease try again.\n";
		goto postcode;
	}
	city:
	cout << "\t\t\t\t7. City: ";
	getline(cin, city);
	if (city == "") {
		cout << "\t\t\t\tPlease input the city.\n";
		goto city;
	}
	else if (city == "0")
		adminMain();
	state:
	cout << "\t\t\t\t8. State: ";
	getline(cin, state);
	if (state == "") {
		cout << "\t\t\t\tPlease input the state.\n";
		goto state;
	}
	else if (state == "0")
		adminMain();
	pass:
	int i = 0;
	char ch;
	cout << "\t\t\t\t9. Set a password for your staff\n" 
	<< "\t\t\t\tIn order to protect the staff account, make sure the \n\t\t\t\t     password fulfill the following requirement\n";
	cout << "\t\t\t\t\t-More than 8 characters" << endl << "\t\t\t\t\t-A lowercase letter" << endl
		<< "\t\t\t\t\t-An uppercase letter" << endl << "\t\t\t\t\t-A number"
		<< endl << "\t\t\t\tPlease enter you password: ";

	while ((ch = _getch()) != 13)
	{
		psw[i] = ch;
		i++;
		printf("*");
	}
	psw[i] = '\0';

	ps.checkValidity(psw);
	ps.ShowStatus();
	if (!ps.IsTotalyValid()) {
		cout << "\n\t\t\tThe password does not fulfill the requirement. Please try another password.\n\n";
		goto pass;
	}
	
	string checkStaff_query = "SELECT * from staff WHERE Staff_ICNo = '" + icno + "'";
	const char* cu = checkStaff_query.c_str();
	qstate = mysql_query(conn, cu);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		if (res->row_count == 1)
		{
			again:
			cout << "\n\t\t\t\tThis staff already have an account. \n\t\t\t\tDo you want to register another staff (y/n): ";
			char c;
			cin >> c;
			switch (c) {
			case 'y':
				resStaff();
				break;
			case 'Y':
				resStaff();
				break;
			case 'n':
				adminMain();
				break;
			case 'N':
				adminMain();
				break;
			default:
				cout << "\n\t\t\t\tWrong input. Please try again." << endl;
				goto again;
				break;
			}
		}
		else
		{
			string adminId = "B032010130";
			string insertStaff_query = "INSERT into staff (Staff_ICNo, Staff_Name, Staff_Email, Staff_MobileNo, Staff_Address, Staff_Postcode, Staff_City, Staff_State, Staff_Psw, Admin_ID) values ('"
				+ icno + "', '" + name + "', '" + email + "', '" + mobile +"', '" + address + "', '" + postcode + "', '" + city + "', '" + state + "','" + psw + "', '" + adminId + "')";
			const char* q = insertStaff_query.c_str();
			qstate = mysql_query(conn, q);
		}
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	} 
	
	cout << "\n\t\t\t\t\tSuccessfully register the staff!\n\n\t\t\t\t";
	system("pause");
	adminMain();
}

void updateStaff() {
	system("cls");
	Email e; list ls; PasswordStatus ps;
	string icno, name, email, mobile, address, postcode, city, state;
	char psw[15];
	int i = 0; char ch;

	string seacrh_query = "select * from staff";
	const char* q = seacrh_query.c_str();
	qstate = mysql_query(conn, q);

	ls.staffList();
	cout << "\t\t(Enter 0 to back to main menu)\n\n";
	cout << "\n\t\tPlease enter the IC No of staff that you want to update his/her information: "; cin >> icno;
	
	string searchStaff_query = "select * from staff where Staff_ICNo = '" + icno + "'";
	if (icno == "0")
		adminMain();
	const char* p = searchStaff_query.c_str();
	qstate = mysql_query(conn, p);
	res = mysql_store_result(conn);
	while (row = mysql_fetch_row(res))
	{
		cout << "\n\t\t\t\t\t1. Identification Card Number: " << row[0] << endl;
		cout << "\t\t\t\t\t2. Name: " << row[1] << endl;
		cout << "\t\t\t\t\t3. E-mail: " << row[2] << endl;
		cout << "\t\t\t\t\t4. Mobile: " << row[3] << endl;
		cout << "\t\t\t\t\t5. Address: " << row[4] << endl;
		cout << "\t\t\t\t\t6. Postcode: " << row[5] << endl;
		cout << "\t\t\t\t\t7. City: " << row[6] << endl;
		cout << "\t\t\t\t\t8. State: " << row[7] << endl;
		cout << "\t\t\t\t\t9. Password: " << row[8] << endl;
		cout << "\t\t\t\t\t10. Select another staff." << endl;
	}
	if (res->row_count == 1)
	{
		update:
		cout << "\n\t\t\t\tChoose the number of information that you want to make change: ";
		cin >> choose;
		cin.ignore(1, '\n');
		if (choose == 0)
			adminMain();
		if (choose == 1)
		{
			ic:
			string newic;cout << "\n\t\t\t\t\tNew IC No:"; getline(cin, newic);
			if (newic == "")
			{
				cout << "\t\t\t\t\tPlease input the new IC.\n";
				goto ic;
			}
			else if (newic.size() != 12 || !check_digit(newic)) {
				cout << "\n\t\t\t\t\tThe IC number should be 12 digits only. Please enter again.\n";  //Malaysian IC No is fixed to 12 integer only
				goto ic;
			}
			string update_query = "update staff set Staff_ICNo = '" + newic + "' where Staff_ICNo = '" + icno + "'";
			const char* q = update_query.c_str();
			qstate = mysql_query(conn, q);
			cout << "\n\t\t\t\t\tThe Ic No is updated.\n\n";
		}
		else if (choose == 2)
		{
			name:
			cout << "\n\t\t\t\t\tNew Name:"; getline(cin, name);
			if (name == "") {
				cout << "\t\t\t\t\tPlease input the new name.\n";
				goto name;
			}
			string update_query = "update staff set Staff_Name = '" + name + "' where Staff_ICNo = '" + icno + "'";
			const char* q = update_query.c_str();
			qstate = mysql_query(conn, q);
			cout << "\n\t\t\t\t\tThe name is updated.\n\n";
		}
		else if (choose == 3)
		{
			email:
			cout << "\n\t\t\t\t\tNew E-mail:"; getline(cin, email);
			if (email == "") {
				cout << "\t\t\t\t\tPlease input the new email.\n";
				goto email;
			}
			if (!e.is_valid(email)) {
				cout << "\t\t\t\t\tYour email is invalid. Please try again.\n";
				goto email;
			}
			string update_query = "update staff set Staff_Email = '" + email + "' where Staff_ICNo = '" + icno + "'";
			const char* q = update_query.c_str();
			qstate = mysql_query(conn, q);
			cout << "\n\t\t\t\t\tThe email is updated.\n\n";
		}
		else if (choose == 4)
		{
			mobile:
			cout << "\n\t\t\t\t\tNew Mobile No:"; getline(cin, mobile);
			if (mobile == "") {
				cout << "\t\t\t\t\tPlease input the new mobile.\n";
				goto mobile;
			}
			string update_query = "update staff set Staff_MobileNo = '" + mobile + "' where Staff_ICNo = '" + icno + "'";
			const char* q = update_query.c_str();
			qstate = mysql_query(conn, q);
			cout << "\n\t\t\t\t\tThe mobile no is updated.\n\n";
		}
		else if (choose == 5)
		{
			address:
			cout << "\n\t\t\t\t\tNew Address:"; getline(cin, address);
			if (address == "") {
				cout << "\t\t\t\t\tPlease input the new address.\n";
				goto address;
			}
			string update_query = "update staff set Staff_Address = '" + address + "' where Staff_ICNo = '" + icno + "'";
			const char* q = update_query.c_str();
			qstate = mysql_query(conn, q);
			cout << "\n\t\t\t\t\tThe address is updated.\n\n";
		}
		else if (choose == 6)
		{
			postcode:
			cout << "\n\t\t\t\t\tNew Postcode:"; getline(cin, postcode);
			if (postcode == "") {
				cout << "\t\t\t\tPlease input the postcode.\n";
				goto postcode;
			}
			else if (!check_digit(postcode)) {
				cout << "\t\t\t\tSomething wrong with your input. A postcode should be 5 digit.\n\t\t\t\tPlease try again.\n";
				goto postcode;
			}
			else if (postcode.size() != 5) {
				cout << "\t\t\t\tSomething wrong with your input. A postcode should be 5 digit.\n\t\t\t\tPlease try again.\n";
				goto postcode;
			}
			string update_query = "update staff set Staff_Postcode = '" + postcode + "' where Staff_ICNo = '" + icno + "'";
			const char* q = update_query.c_str();
			qstate = mysql_query(conn, q);
			cout << "\n\t\t\t\t\tThe postcode is updated.\n\n";
		}
		else if (choose == 7)
		{	
			city:
			cout << "\n\t\t\t\t\tNew City:"; getline(cin, city);
			if (city == "") {
				cout << "\t\t\t\t\tPlease input the new city.\n";
				goto city;
			}
			string update_query = "update staff set Staff_City = '" + city + "' where Staff_ICNo = '" + icno + "'";
			const char* q = update_query.c_str();
			qstate = mysql_query(conn, q);
			cout << "\n\t\t\t\t\tThe city is updated.\n\n";
		}
		else if (choose == 8)
		{
			state:
			cout << "\n\t\t\t\t\tNew State:"; getline(cin, state);
			if (state == "") {
				cout << "\t\t\t\t\tPlease input the new state.\n";
				goto state;
			}
			string update_query = "update staff set Staff_State = '" + state + "' where Staff_ICNo = '" + icno + "'";
			const char* q = update_query.c_str();
			qstate = mysql_query(conn, q);
			cout << "\n\t\t\t\t\tThe state is updated.\n\n";
		}
		else if (choose == 9)
		{
			pass:
			cout << "\n\t\t\t\t\tNew Password\n" << "\t\t\t\t\tIn order to protect the staff account, make sure the \n\t\t\t\t\tpassword fulfill the following requirement\n";
			cout << "\t\t\t\t\t\t-More than 8 characters" << endl << "\t\t\t\t\t\t-A lowercase letter" << endl
				<< "\t\t\t\t\t\t-An uppercase letter" << endl << "\t\t\t\t\t\t-A number"
				<< endl << "\t\t\t\t\tPlease enter you password: ";

			while ((ch = _getch()) != 13)
			{
				psw[i] = ch;
				i++;
				printf("*");
			}
			psw[i] = '\0';

			ps.checkValidity(psw);
			ps.ShowStatus();
			if (!ps.IsTotalyValid()) {
				cout << "\n\t\t\tYour password does not fulfill the requirement. Please try another password.\n";
				goto pass;
			}
			string pass = psw;
			string update_query = "update staff set Staff_Psw = '" + pass + "' where Staff_ICNo = '" + icno + "'";
			const char* q = update_query.c_str();
			qstate = mysql_query(conn, q);
			cout << "\n\t\t\t\t\tThe password is updated.\n\n";
		}
		else if (choose == 10)
		{
			updateStaff();
		}
		else
		{
			cout << "\t\t\t\t\t     Invalid Input. Please try again\n\n\n";
			goto update;
		}
		system("pause");
		adminMain();
	}
	else
	{
	cout << "\t\t\t\t\t\tThe staff does not exist...\n\t\t\t\t\t\t    Check your input...\n\n\n\t\t\t\t\t\t";
	system("pause");
		updateStaff();
	}
}

void deleteStaff() {
	system("cls");
	list ls;

	string icno;
	char choose, chooseback;

	string seacrh_query = "SELECT * from staff";
	const char* q = seacrh_query.c_str();
	qstate = mysql_query(conn, q);

	ls.staffList();
	cout << "\t\t(Enter 0 to back to main page)\n\n";

	cout << "\n\t\t\t\tEnter the IC No of staff that you want to delete: "; cin >> icno;
	if (icno == "0")
		adminMain();
	string check = "select * from staff where Staff_ICNo = '" + icno + "'";
	const char* c = check.c_str();
	qstate = mysql_query(conn, c);
	res = mysql_store_result(conn);
	if (res->row_count == 1)
	{
		confirm:
		cout << "\t\t\t\tAre you sure this staff is no longer working here? (y/n): "; cin >> choose;
		if (choose == 'y' || choose == 'Y')
		{
			string delete_query = "delete from staff where Staff_ICNo = '" + icno + "'";
			const char* q = delete_query.c_str();
			qstate = mysql_query(conn, q);

			if (!qstate)
			{
				cout << "\t\t\t\t\t\tThe staff has been deleted...\n\n\t\t\t\t\t  Do you want to delete another staff?(y/n): "; cin >> chooseback;
				if (chooseback == 'y' || chooseback == 'Y')
				{
					deleteStaff();
				}
				else if (chooseback == 'n' || chooseback == 'N')
				{
					cout << "\n\t\t\t\t\t  Thank You. Press enter to back to the main menu.\n\n\t\t\t\t\t     ";
					system("pause");
					adminMain();
				}
			}
			else
			{
				cout << "\t\t\t\t\tQuery Execution Problem!" << mysql_errno(conn) << endl;
			}
		}
		else if (choose == 'n' || choose == 'N')
		{
			cout << "\n\t\t\tYou decided not to delete the staff. Press enter to back to the main menu.\n\t\t\t\t\t";
			system("pause");
			adminMain();
		}
		else
		{
			cout << "\n\t\t\t\t\t\tInvalid input.Please try again\n";
			goto confirm;
		}
	
	}
	else
	{
		cout << "\t\t\t\t\t\tThe staff does not exist...\n\t\t\t\t\t\t    Check your input...\n\n\n\t\t\t\t\t\t";
		system("pause");
		deleteStaff();
	}
}