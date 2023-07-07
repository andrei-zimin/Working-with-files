#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <conio.h>
#include <locale>
#include <cmath> 
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <iostream>

#define MAXLEN 256

/*
���� ������� �������������� ������ � �������� �������.
��������� �� ������:
1) ��������������� ��������� ������ �� ����� ������ ���������
2) ������� ������� enter ������ ����� ���, ��� ���� ������ ����
3) ������������ ��������� ������ ����� �� ������ ������� ��������� �����
������ ����� �� ������ � write_data
� ��������� ������� ����� ��������� ���� char * filename, ���� filename[MAXLEN] � �������� ����� �����.
������� ���, ��� ������ ����������� ���������, �� ���� �� ������� ��������� ���������������� ������: �������� -- �� ������
*/


using namespace std;

void submit(char* filename);
void menu_create();
void menu_start();
void menu_open();
void menu_open_main(char filename[MAXLEN]);
void menu_edit(char* filename);
void menu_edit_member(char* filename, int n);
void menu_search(char* filename);
void show(char* filename, int shift);


struct member
{
	// ����������, ���������, ���������� �� ������ � �� ���������� �������
	unsigned long long int phone; // ����� ���������� ��� ������ ������������ ��� ����, ����� ����� ���� ��������������� ����� � ���������� �������, ����� �������� � ���� ���������� �����
	string FIO;
	string address;
};

/*
������������ ������� � ���������. ������ ������������ ������������� � ��� ������, ���� �� ����� ���� ������ ��������� ������� �����
� ��� ���������� ������ ����� ��������� ������, ������� ����� ���������
*/
vector<int> indices;

void write_data(char* filename, vector<member>v) {
	/*
	���������� ������ �������� v � ���� filename.
	������ ������ -- ����� ������, �������, ��� � ����� � ������ ��������� ;
	*/
	ofstream file(filename); //��������� ���� �� ������
	for (vector<member>::iterator i = v.begin(); i != v.end(); ++i) // ����, ������������ ��� ������� ������ � �������
		file << i->phone << ";" << i->FIO << ";" << i->address << endl; //� ������� ������������ �� � �����
	file.close(); //�� � ������� ���� �� �������� ��
}

vector<member> read_data(char* filename) {
	/*
	���������� ������ �������� �� ����� filename
	*/
	vector<member>v; //������ -- ������������ ������. ����� ������, ��������� ������������� ��������� ������ � ��������� ���������� � ������� ��� ���������.
	//vector.at(������) -- ���������� ������� ������� �� �������
	//vector.push_back(�������) -- �������� ������ � ����� �������
	//vector.erase(������) -- ������� ������� �� �������
	ifstream file(filename); //�������� ����� �� ������
	if (file.is_open())
	{
		member new_m;
		string line;
		char FIO_buf[MAXLEN];
		char address_buf[MAXLEN];
		unsigned long long int phone;
		while (getline(file, line))//���������� ��� ������ � �����
		{
			//sscanf -- �� �� �����, ��� � scanf, ������ ������ ����� �� ������
			//line -- string, ��� ������� � ������� ��� �������� ������������ ����� c_str
			sscanf(line.c_str(), "%llu;%[^;];%[^\n\r]", &phone, FIO_buf, address_buf);
			new_m.phone = phone;
			new_m.FIO = FIO_buf;
			new_m.address = address_buf;
			v.push_back(new_m);
		}
	}
	else
		cout << "������ ������ �����\n";
	file.close();
	return v;
}
member new_member() {
	/*
	���������� ��������� � ����������� � ��� ���������, ��� � �������. ��� ������ �������� � ���������� � ������ �������.
	� �������� ������ �������� ����� ��������� ������ ����� ��������������� �����.
	���� ��������� �������� �� ������������� ����� ���������� -- ��� ����� ���������� ������ ��� ��� ���.
	���� ��� ������� ���� �������, ��������� ���� ��� ���.
	*/
	system("cls");//������� ���� �������
	unsigned long long int phone;
	char phone_buf[MAXLEN], * p = phone_buf;
	int digit;
	bool isd = true;
	member m;
	printf("����� �������� (������ �����): ");
	cin >> phone_buf;
	cin.ignore();//��� ���� ����� getline ����� cin �� ������ ��������� ��������. ������� ����� �����
	while (*p)// ���� �������� �� ��, �������� �� ��������� ������ ������
		if (!isdigit(*p++))
		{
			isd = false;// �� �������� :(
			break;
		}
	if (isd) {// � ��� ���� ��������
		printf("���: ");
		getline(cin, m.FIO);
		printf("�����: ");
		getline(cin, m.address);
		m.phone = atoll(phone_buf);
		return m;// ���������� ����� ������ member
	}
	else {// ���� �� ��������, �� ������� �� ���� ��������� � ������ �����
		system("cls");
		printf("������������ ����� ��������.");
		_getch();// ��������� �����, ��������� � ����������. ����������� �����, ��� ������� ������� enter
		return new_member();
	}
}

int fileexists(char* filename)
{
	/*
	��������� ������� ����� filename.
	���������� 1, ���� ��������� ����� ����, 0 -- ���� ���.
	*/
	FILE* f = fopen(filename, "r");
	if (f == NULL)
		return 0;
	else
		return 1;
}

FILE* newfile(char* filename)
{
	/*
	������ ������� ���� filename.
	*/
	FILE* f = fopen(filename, "w");
	return f;
}


void menu_fileexists(char filename[MAXLEN]) {
	/*
	���� ���������� � ���, ������, ���� ����, ������� �� ������ �������, ��� ����������.
	���� �������� "��", �� ��� ���������� ���������.
	*/
	system("cls");
	printf(
		"���� \"%s\" ��� ����������. ����������?\n"
		"1. ��\n"
		"2. ���\n"
		"0. ������\n", filename
	);
	char choice = _getch();
	switch (choice - '0') {
	case 1: {
		newfile(filename);
		system("cls");
		printf("���� ������� ������");
		_getch();
		menu_start();
		break;
	}
	case 2: {
		menu_create();
		break;
	}
	case 0: {
		menu_start();
		break; }
	default: {
		menu_fileexists(filename);
		break;
	}
	}
}
void menu_create()
{
	/*
	�������
	������� ����� ����
	*/
	system("cls");
	char filename[MAXLEN];
	printf(
		"������� �������� �����: "
	);
	fgets(filename, MAXLEN, stdin);
	filename[strlen(filename) - 1] = 0;// ������� ���������� ����-������ ������ ������� �������� ������
	if (*filename == 0) //���� ������ ������ ������ -- ����-������, ������, ������ ������
		menu_start();
	else if (fileexists(filename)) {
		menu_fileexists(filename);
	}
	else {
		newfile(filename);
		system("cls");
		printf("���� ������� ������");
		_getch();
		menu_start();
	}
}

void menu_read(char filename[MAXLEN]) {
	/*
	�������
	������� ���� -> ������
	*/
	system("cls");
	printf(
		"������ \"%s\"\n"
		"1. ������\n"
		"2. �����\n"
		"0. �����\n", filename
	);
	char choice = _getch();
	switch (choice - '0')
	{
	case 1: {
		show(filename, 0);
		menu_read(filename);
		break;
	}
	case 2: {
		menu_search(filename);
		break;
	}
	case 0: {
		menu_open_main(filename);
	}
	default: {
		menu_read(filename);
		break;
	}
	}

}


void show(char* filename, int shift) {
	/*
	�������
	������� ���� -> ������ -> ������
	���
	������� ���� -> ������ -> �����
	*/
	system("cls");
	vector<member>v = read_data(filename);
	member m;
	//counter -- ����� ������� �� ������ �������� ������, ����� ������������� �������� � ��������� �� 1 �� 7
	//shift -- �����. �� ���� -- ����� ��������, �� ������� �� ������ ���������, ���� ������� �� 0.
	// size -- ����� ������� � �����
	int start, i, counter = 5, size = v.size();
	if (size != 0) {
		int maxpage = size / counter;
		if (size % counter) maxpage++;
		//maxpage -- ������ ����� ������� ������ 
		if (maxpage < shift + 1)//���� ������� ������, �� ����� ������� ����� ����������� � ����� �������, ��� ����� ������� ��������
			shift = maxpage - 1;
		start = counter * shift; // ����� � ����� ������ � ������ ������
		printf("\"%s\" %d\/%d\n�������� ����� ������, ����� �� ��������.\n\n", filename, shift + 1, maxpage);
		for (i = start; (i < size) && (start + counter > i); i++) {
			m = v.at(i);
			cout << i - start + 1 << ". " << m.phone << ": " << m.FIO << ", " << m.address << endl;
		}
		cout << endl;
		if (shift != 0)
			cout << "8. �����" << endl;
		if (i < size)
			cout << "9. ������" << endl;
		cout << "0. ������";
		char choice = _getch();
		switch (choice - '0')
		{
		case 8: {
			if (shift > 0)
				show(filename, shift - 1);
			else
				show(filename, shift);
			break;
		}
		case 9: {
			if (i < size)
				show(filename, shift + 1);
			else
				show(filename, shift);
			break;
		}
		case 0: {
			break;
		}
		default: {
			menu_edit_member(filename, choice - '0' + shift * counter - 1);
			show(filename, shift);
			break;
		}
		}
	}
	else //���� � ����� ��� �� ����� ������
	{
		printf("\"%s\"\n�� ������� �� ����� ������.\n\n", filename);
		cout << "0. ������";
		char choice = _getch();
		switch (choice - '0')
		{
		case 0: {
			break;
		}
		default: {
			menu_edit_member(filename, choice - '0' + shift * counter - 1);
			show(filename, shift);
			break;
		}
		}

	}



}
void menu_open_main(char filename[MAXLEN]) {
	/*
	�������
	������� ����
	*/
	system("cls");
	printf(
		"\"%s\"\n"
		"1. ������ �����\n"
		"2. �������������� �����\n"
		"0. ������\n", filename
	);
	char choice = _getch();
	switch (choice - '0')
	{
	case 1: {
		menu_read(filename);
		break;
	}
	case 2: {
		menu_edit(filename);
		break;
	}
	case 0: {
		menu_start();
	}
	default: {
		menu_open_main(filename);
		break;
	}
	}
}

void search_results(char* filename, unsigned long long int phone) {
	/*
	��������������� �������, ���������� ����� ����, ��� �� ������� ���������� ����� �������� ��� ������
	������� ���� -> ������ -> �����
	������� ������� show ��� ����������� ������
	����� ������ �� ������� ��������� ����������� ������, ����������� ������������� ��������� ���������.
	*/
	system("cls");
	char fname[] = "buffer";// �������� ���������� �����
	vector<member> results, v = read_data(filename);// ���� ��� ����������� ������ -- ��� �������� � �����
	vector<int> local_indices; // �� ������
	member m;
	int i, j = 0;
	for (i = 0; i < v.size(); i++) { // ������ ������ �������, � ������� ����� �������� ��������� � ��������� �������
		m = v.at(i);
		local_indices.push_back(j); //������ �������. �������� ������������� � ���, ����� ���-�� ������� ���������� � ���, ����� ������ ������ ���� �������
		j++;
		if (m.phone == phone)
			results.push_back(m);
	}
	write_data(fname, results);// ������ ������� ��������� ������ ����������� �� ��������� ����
	::indices = local_indices;// ������� �������� � ���������� ����������
	show(fname, 0);
	system("cls");
	printf(
		"��������� ���������?\n"
		"1. ��\n"
		"2. ���\n", m.phone);
	char choice;
	do
	{
		choice = _getch();

	} while (!((choice == '1') || (choice == '2')));
	switch (choice - '0')
	{
	case 1: {
		/*
		����� ���������� ������� ��������� �� ���������� ����� � ���� ��
		*/
		j = 0;
		results = read_data(fname);
		for (i = 0; (i < v.size()); i++) {
			m = v.at(i);
			if (m.phone == phone) {
				if (find(indices.begin(), indices.end(), j) != indices.end())
					v.at(i) = results.at(j); //������
				else {
					v.erase(v.begin() + i);// ������� �� ��
					i--;
				}
				j++;
			}
		}
		write_data(filename, v);
		break;
	}
	case 2:
	{
		break;
	}
	}
	remove(fname);//������� ��������� ����
	::indices = {};//������� �������
	menu_read(filename);

}

void menu_search(char* filename) {
	/*
	�������, ������������� ����� �������� ��� ������
	������� ���� -> ������-> �����
	� ������, ���� ����� ���������, �������� ��� � ������� search_results

	��� ��������� ������  menu_create (��� ����� ����������)
	*/
	system("cls");
	unsigned long long int phone;
	char phone_buf[MAXLEN], * end, * p = phone_buf;
	int digit;
	bool isd = true;
	printf(
		"������� ����� �������� (������ �����): "
	);
	fgets(phone_buf, MAXLEN, stdin);
	phone_buf[strlen(phone_buf) - 1] = 0;
	if (*phone_buf == 0)
		menu_read(filename);
	else {
		while (*p)
			if (!isdigit(*p++))
			{
				isd = false;
				break;
			}
		if (isd) {
			phone = atoll(phone_buf); // ����������� ������ � �����
			search_results(filename, phone);
			exit(0);
		}
		else {
			system("cls");
			printf("������������ ����� ��������.");
			_getch();
			menu_search(filename);
		}
	}
}

void submit_member(char* filename, int n) {
	/*
	�������, ������������� ������������� �������� ������
	*/
	system("cls");
	vector<member>v = read_data(filename);
	member m = v.at(n);
	printf(
		"�� �������, ��� ������ ������� \"%d\"?\n"
		"1. ��\n"
		"2. ���\n", m.phone);
	char choice = _getch();
	switch (choice - '0')
	{
	case 1: {
		if (indices.size())
			indices.erase(indices.begin() + n);
		v.erase(v.begin() + n);
		write_data(filename, v);
		system("cls");
		printf("������ �������.");
		_getch();
		break;
	}
	case 2: {
		menu_edit_member(filename, n);
		break;
	}
	default: {
		submit_member(filename, n);
		break;
	}
	}
}
void edit_member(char* filename, int n) {
	/*
	�������, ������������� ������������� ��������� ������
	*/
	vector<member>v = read_data(filename);
	member m = new_member();
	system("cls");
	printf(
		"��������� ��������� ���������?\n"
		"1. ��\n"
		"2. ���\n");
	char choice;
	do
	{
		choice = _getch();

	} while (!((choice == '1') || (choice == '2'))); //��� ���� ���� ��������� ��� ���� ����
	switch (choice - '0')
	{
	case 1: {
		v.at(n) = m;
		write_data(filename, v);
		system("cls");
		printf("������ ������� ��������.");
		_getch();
		break;
	}
	case 2: {
		menu_edit_member(filename, n);
		break;
	}
	}
}
void menu_edit_member(char* filename, int n) {
	/*
	������� ��������� ������. ���������� ������� ������ ���� ����� �������� ���� �������, ���� ����� �����
	*/
	vector<member>v = read_data(filename);
	member m = v.at(n);
	system("cls");
	printf(
		"��������� \"%d\"\n"
		"1. ��������\n"
		"2. �������\n"
		"0. �����\n", m.phone);
	char choice = _getch();
	switch (choice - '0')
	{
	case 1: {
		edit_member(filename, n);
		break;
	}
	case 2: {
		submit_member(filename, n);
		break;
	}
	case 0: {
		break;
	}
	default: {
		menu_edit_member(filename, n);
		break;
	}
	}
}

void menu_edit(char* filename) {
	/*
	�������
	������� ���� -> ��������
	*/
	system("cls");
	printf(
		"�������������� \"%s\"\n"
		"1. �������� ������\n"
		"2. �������� ����\n"
		"0. �����\n", filename);
	char choice = _getch();
	switch (choice - '0')
	{
	case 1: {
		vector<member>v = read_data(filename);
		v.push_back(new_member());
		write_data(filename, v);
		system("cls");
		printf("������ ������� ���������.");
		_getch();
		menu_edit(filename);
		break;
	}
	case 2: {
		submit(filename);
		break;
	}
	case 0: {
		menu_open_main(filename);
		exit(0); }
	default: {
		menu_edit(filename);
		break;
	}
	}

}
void submit(char* filename) {
	/*
	�������
	������� ���� -> �������� -> �������� ����
	����������� ������������� �������� �����
	*/
	system("cls");
	printf(
		"�� �������, ��� ������ ������� \"%s\"?\n"
		"1. ��\n"
		"2. ���\n", filename);
	char choice = _getch();
	switch (choice - '0')
	{
	case 1: {
		newfile(filename);
		system("cls");
		printf("���� ������� ������.");
		_getch();
		menu_edit(filename);
		break;
	}
	case 2: {
		menu_edit(filename);
		break;
	}
	default: {
		submit(filename);
		break;
	}
	}


}

void menu_open() {
	/*
	�������
	������� ����
	����������� ��� �����, ���� ���� �� ������, �� ������� �� ����

	�� ����������� ������ menu_create
	*/
	system("cls");
	char filename[MAXLEN];
	printf(
		"������� �������� �����: "
	);
	fgets(filename, MAXLEN, stdin);
	filename[strlen(filename) - 1] = 0;
	if (*filename == 0)
		menu_start();
	else if (fileexists(filename)) {
		menu_open_main(filename);

	}
	else {
		system("cls");
		printf("���� \"%s\" �� ������", filename);
		_getch();
		menu_open();
	}
}
void menu_start() {
	/*
	������� ����
	*/
	system("cls");
	printf(
		"�������� �������� ��������\n"
		"1. ������� ���� �������� ������\n"
		"2. ������� ���� �������� ������\n"
		"0. �����\n"
	);
	char choice = _getch();
	switch (choice - '0')
	{
	case 1: {
		menu_create();
		break;
	}
	case 2: {
		menu_open();
		break;
	}
	case 0: {
		printf("��������� ��������� ������");
		exit(0); }
	default: {
		menu_start();
		break;
	}
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
	menu_start();

}

