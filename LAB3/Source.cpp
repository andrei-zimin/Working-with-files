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
Ввод номеров осуществляется только в числовом формате.
Программа не терпит:
1) Самостоятельное изменение файлов во время работы программы
2) Нажатия клавиши enter вместо ввода там, где есть пункты меню
3) Подсовывания программе вместо файла БД любого другого непустого файла
Формат файла БД смотри в write_data
В программе функции часто принимают либо char * filename, либо filename[MAXLEN] в качестве имени файла.
Разницы нет, так просто исторически сложилось, но один из главных принципов программирования гласит: работает -- не трогай
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
	// Собственно, структура, отвечающая за записи в БД телефонных номеров
	unsigned long long int phone; // Такой диковинный тип данных используется для того, чтобы число было неотрицательным целым и достаточно длинным, чтобы записать в него телефонный номер
	string FIO;
	string address;
};

/*
Единственный костыль в программе. Вектор используется исключительно в том случае, если вы через меню поиска пытаетесь удалить файлы
В нем содержатся номера строк поисковой выдачи, которые НУЖНО сохранить
*/
vector<int> indices;

void write_data(char* filename, vector<member>v) {
	/*
	Записывает вектор структур v в файл filename.
	Каждая запись -- новая строка, телефон, ФИО и адрес в строке разделены ;
	*/
	ofstream file(filename); //Открывает файл на запись
	for (vector<member>::iterator i = v.begin(); i != v.end(); ++i) // Цикл, перебирающий все сячейки памяти в векторе
		file << i->phone << ";" << i->FIO << ";" << i->address << endl; //и красиво записывающий их в файле
	file.close(); //Ну и закрыть тоже не помешало бы
}

vector<member> read_data(char* filename) {
	/*
	Возвращает вектор структур из файла filename
	*/
	vector<member>v; //Вектор -- динамический массив. Очень удобен, поскольку предоставляет свободный доступ к изменению количества и состава его элементов.
	//vector.at(индекс) -- возвращает элемент вектора по индексу
	//vector.push_back(элемент) -- помещает объект в конец вектора
	//vector.erase(индекс) -- удаляет элемент по индексу
	ifstream file(filename); //Открытие файла на чтение
	if (file.is_open())
	{
		member new_m;
		string line;
		char FIO_buf[MAXLEN];
		char address_buf[MAXLEN];
		unsigned long long int phone;
		while (getline(file, line))//перебирает все строки в файле
		{
			//sscanf -- то же самое, что и scanf, только данные берет из строки
			//line -- string, для доступа к массиву его символов используется метод c_str
			sscanf(line.c_str(), "%llu;%[^;];%[^\n\r]", &phone, FIO_buf, address_buf);
			new_m.phone = phone;
			new_m.FIO = FIO_buf;
			new_m.address = address_buf;
			v.push_back(new_m);
		}
	}
	else
		cout << "Ошибка чтения файла\n";
	file.close();
	return v;
}
member new_member() {
	/*
	Возвращает структуру с записанными в нее телефоном, ФИО и адресом. Все данные вводятся с клавиатуры в режиме диалога.
	В качестве номера телефона может выступать только целое неотрицательное число.
	Если введенное значение не соответствует этому требованию -- вам будет предложено ввести его еще раз.
	Если эта функция была вызвана, обратного пути уже нет.
	*/
	system("cls");//Очистка окна консоли
	unsigned long long int phone;
	char phone_buf[MAXLEN], * p = phone_buf;
	int digit;
	bool isd = true;
	member m;
	printf("Номер телефона (только цифры): ");
	cin >> phone_buf;
	cin.ignore();//Без этой штуки getline после cin не всегда корректно работает. Очищает поток ввода
	while (*p)// цикл проверки на то, является ли введенная строка числом
		if (!isdigit(*p++))
		{
			isd = false;// Не является :(
			break;
		}
	if (isd) {// А вот если является
		printf("ФИО: ");
		getline(cin, m.FIO);
		printf("Адрес: ");
		getline(cin, m.address);
		m.phone = atoll(phone_buf);
		return m;// Возвращаем новую запись member
	}
	else {// Если не является, то выводим об этом сообщение и просим снова
		system("cls");
		printf("Некорректный номер телефона.");
		_getch();// Считывает сивол, введенный с клавиатуры. Срабатывает сразу, без нажатия клавиши enter
		return new_member();
	}
}

int fileexists(char* filename)
{
	/*
	Проверяет наличие файла filename.
	Возвращает 1, если сущестует такой файл, 0 -- если нет.
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
	Просто создает файл filename.
	*/
	FILE* f = fopen(filename, "w");
	return f;
}


void menu_fileexists(char filename[MAXLEN]) {
	/*
	Меню вызываемое в том, случае, если файл, который вы хотите создать, уже существует.
	Если выберите "Да", то его содержимое очистится.
	*/
	system("cls");
	printf(
		"Файл \"%s\" уже существует. Продолжить?\n"
		"1. Да\n"
		"2. Нет\n"
		"0. Отмена\n", filename
	);
	char choice = _getch();
	switch (choice - '0') {
	case 1: {
		newfile(filename);
		system("cls");
		printf("Файл успешно создан");
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
	Подменю
	Создать новый файл
	*/
	system("cls");
	char filename[MAXLEN];
	printf(
		"Введите название файла: "
	);
	fgets(filename, MAXLEN, stdin);
	filename[strlen(filename) - 1] = 0;// Вручную проставляю нуль-символ вместо символа переноса строки
	if (*filename == 0) //Если первый символ строки -- нуль-символ, значит, строка пустая
		menu_start();
	else if (fileexists(filename)) {
		menu_fileexists(filename);
	}
	else {
		newfile(filename);
		system("cls");
		printf("Файл успешно создан");
		_getch();
		menu_start();
	}
}

void menu_read(char filename[MAXLEN]) {
	/*
	Подменю
	Открыть файл -> Чтение
	*/
	system("cls");
	printf(
		"Чтение \"%s\"\n"
		"1. Чтение\n"
		"2. Поиск\n"
		"0. Назад\n", filename
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
	Подменю
	Открыть файл -> Чтение -> Чтение
	ИЛИ
	Открыть файл -> Чтение -> Поиск
	*/
	system("cls");
	vector<member>v = read_data(filename);
	member m;
	//counter -- число записей на каждой странице выдачи, можно безболезненно изменить в диапазоне от 1 до 7
	//shift -- сдвиг. По сути -- номер страницы, на которой мы сейчас находимся, если считать от 0.
	// size -- число записей в файле
	int start, i, counter = 5, size = v.size();
	if (size != 0) {
		int maxpage = size / counter;
		if (size % counter) maxpage++;
		//maxpage -- просто число страниц выдачи 
		if (maxpage < shift + 1)//Если удалить записи, то число страниц может уменьшиться и стать меньшим, чем номер текущей страницы
			shift = maxpage - 1;
		start = counter * shift; // Номер в файле первой в выдаче записи
		printf("\"%s\" %d\/%d\nВыберите номер записи, чтобы ее изменить.\n\n", filename, shift + 1, maxpage);
		for (i = start; (i < size) && (start + counter > i); i++) {
			m = v.at(i);
			cout << i - start + 1 << ". " << m.phone << ": " << m.FIO << ", " << m.address << endl;
		}
		cout << endl;
		if (shift != 0)
			cout << "8. Назад" << endl;
		if (i < size)
			cout << "9. Вперед" << endl;
		cout << "0. Отмена";
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
	else //Если в файле нет ни одной записи
	{
		printf("\"%s\"\nНе найдено ни одной записи.\n\n", filename);
		cout << "0. Отмена";
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
	Подменю
	Открыть файл
	*/
	system("cls");
	printf(
		"\"%s\"\n"
		"1. Чтение файла\n"
		"2. Редактирование файла\n"
		"0. Отмена\n", filename
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
	Вспомогательная функция, вызываемая после того, как вы введете корректный номер телефона для поиска
	Открыть файл -> Чтение -> Поиск
	Сначала вызвает show для результатов поиска
	После выхода из подменю просмотра результатов поиска, запрашивает подтверждения внесенных изменений.
	*/
	system("cls");
	char fname[] = "buffer";// название временного файла
	vector<member> results, v = read_data(filename);// один для результатов поиска -- для векторов в файле
	vector<int> local_indices; // Не вникай
	member m;
	int i, j = 0;
	for (i = 0; i < v.size(); i++) { // Задает вектор записей, в которых номер телефона совпадает с введенным номером
		m = v.at(i);
		local_indices.push_back(j); //Просто костыль. Возникла необходимость в том, чтобы где-то хранить информацию о том, какие записи должны быть удалены
		j++;
		if (m.phone == phone)
			results.push_back(m);
	}
	write_data(fname, results);// Вектор записей поисковой выдачи сохраняется во временный файл
	::indices = local_indices;// Костыль хранится в глобальной переменной
	show(fname, 0);
	system("cls");
	printf(
		"Сохранить изменения?\n"
		"1. Да\n"
		"2. Нет\n", m.phone);
	char choice;
	do
	{
		choice = _getch();

	} while (!((choice == '1') || (choice == '2')));
	switch (choice - '0')
	{
	case 1: {
		/*
		Здесь происходит перенос изменений из временного файла в файл БД
		*/
		j = 0;
		results = read_data(fname);
		for (i = 0; (i < v.size()); i++) {
			m = v.at(i);
			if (m.phone == phone) {
				if (find(indices.begin(), indices.end(), j) != indices.end())
					v.at(i) = results.at(j); //замена
				else {
					v.erase(v.begin() + i);// Удаение из БД
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
	remove(fname);//Удаляет временный файл
	::indices = {};//Очищает костыль
	menu_read(filename);

}

void menu_search(char* filename) {
	/*
	Подменю, запрашивающее номер телефона для поиска
	Открыть файл -> Чтение-> Поиск
	В случае, если номер корректен, передает его в функцию search_results

	Для пояснений смтори  menu_create (Они почти одинаковые)
	*/
	system("cls");
	unsigned long long int phone;
	char phone_buf[MAXLEN], * end, * p = phone_buf;
	int digit;
	bool isd = true;
	printf(
		"Введите номер телефона (только цифры): "
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
			phone = atoll(phone_buf); // првевращает строку в число
			search_results(filename, phone);
			exit(0);
		}
		else {
			system("cls");
			printf("Некорректный номер телефона.");
			_getch();
			menu_search(filename);
		}
	}
}

void submit_member(char* filename, int n) {
	/*
	Подменю, запрашивающее подтверждение удаления записи
	*/
	system("cls");
	vector<member>v = read_data(filename);
	member m = v.at(n);
	printf(
		"Вы уверены, что хотите удалить \"%d\"?\n"
		"1. Да\n"
		"2. Нет\n", m.phone);
	char choice = _getch();
	switch (choice - '0')
	{
	case 1: {
		if (indices.size())
			indices.erase(indices.begin() + n);
		v.erase(v.begin() + n);
		write_data(filename, v);
		system("cls");
		printf("Запись удалена.");
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
	Подменю, запрашивающее подтверждение изменения записи
	*/
	vector<member>v = read_data(filename);
	member m = new_member();
	system("cls");
	printf(
		"Применить введенные изменения?\n"
		"1. Да\n"
		"2. Нет\n");
	char choice;
	do
	{
		choice = _getch();

	} while (!((choice == '1') || (choice == '2'))); //Мне было лень добавлять еще одно меню
	switch (choice - '0')
	{
	case 1: {
		v.at(n) = m;
		write_data(filename, v);
		system("cls");
		printf("Запись успешно изменена.");
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
	Подменю изменения записи. Вызывается выбором записи либо через просмотр всех записей, либо через поиск
	*/
	vector<member>v = read_data(filename);
	member m = v.at(n);
	system("cls");
	printf(
		"Изменение \"%d\"\n"
		"1. Изменить\n"
		"2. Удалить\n"
		"0. Назад\n", m.phone);
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
	Подменю
	Открыть файл -> Изменить
	*/
	system("cls");
	printf(
		"Редактирование \"%s\"\n"
		"1. Добавить запись\n"
		"2. Очистить файл\n"
		"0. Назад\n", filename);
	char choice = _getch();
	switch (choice - '0')
	{
	case 1: {
		vector<member>v = read_data(filename);
		v.push_back(new_member());
		write_data(filename, v);
		system("cls");
		printf("Запись успешно добавлена.");
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
	Подменю
	Открыть файл -> Изменить -> Очистить файл
	Запрашивает подтверждение удаления файла
	*/
	system("cls");
	printf(
		"Вы уверены, что хотите удалить \"%s\"?\n"
		"1. Да\n"
		"2. Нет\n", filename);
	char choice = _getch();
	switch (choice - '0')
	{
	case 1: {
		newfile(filename);
		system("cls");
		printf("Файл успешно очищен.");
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
	Подменю
	Открыть файл
	Запрашивает имя файла, если файл не найден, то ссобщит об этом

	За пояснениями смотри menu_create
	*/
	system("cls");
	char filename[MAXLEN];
	printf(
		"Введите название файла: "
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
		printf("Файл \"%s\" не найден", filename);
		_getch();
		menu_open();
	}
}
void menu_start() {
	/*
	Главное меню
	*/
	system("cls");
	printf(
		"Выберите желаемое действие\n"
		"1. Создать файл исходных данных\n"
		"2. Открыть файл исходных данных\n"
		"0. Выход\n"
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
		printf("Программа заврешила работу");
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

