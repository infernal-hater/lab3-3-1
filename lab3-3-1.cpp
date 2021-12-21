#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
using namespace std;

/* Для работы необходим файл test.txt
Пример содержимого файла:

10 25
25 5
85 7

Последней строчкой не должна быть пустая строка. В строке только один пробел. Это важно.
Вначале программа запросит название файла. Вводить название файла с учетом расширения!
*/


// Временная точка для вычисления времени работы программы
const auto TIME_START = chrono::steady_clock::now();

// Класс, отвечающий за обработку ошибок
class file_exception {
private:
	// Строка ошибки
	string _error;

public:
	// Конструктор со строкой-параметром
	file_exception(const string& error) : _error(error)
	{
	}

	// Вывод ошибки и времени в консоль и в файл
	// По умолчанию 'filename' пустое, запись не в файл не будет производится
	void log(const string& filename = "") const noexcept {
		// фиксиурем время и вычисляем в секундах
		auto current = chrono::steady_clock::now();
		double exit_time = chrono::duration<double>(current - TIME_START).count();

		// Подготовка строки для вывода ошибки и времени
		string log_message = "EXCEPTION: " + _error + "; Time: " + to_string(exit_time) + " ms\n";

		// Вывод в консоль
		cout << log_message;

		// Вывод в файл, если передали имя файла
		if (!filename.empty()) {
			ofstream fout(filename);
			if (fout.is_open()) {
				fout << log_message;
			}
		}
	}

};

// Функция проверяет состоит ли строка только из цифр
bool isOnlyNumbers(const string& string) noexcept {
	for (size_t i = 0; i < string.size(); i++) {
		if (!isdigit(string.at(i))) {
			return false;
		}
	}

	return true;
}

// Функция принимает файловый указаетель и две переменные
// Считывает строку с файла и выделяет оттуда два числа, если в строке не два числа, выбрасывается исключение
void getNumbersFromLine(ifstream& fin, size_t& first, size_t& second) {
	string line;
	size_t space_pos;

	// Считываем строку, находим пробел
	getline(fin, line);
	space_pos = line.find_first_of(" ");

	// Проверка чисел слева и справа от пробела
	if (!isOnlyNumbers(line.substr(0, space_pos)) || !isOnlyNumbers(line.substr(space_pos + 1))) {
		throw file_exception("The line contains characters other than integers");
	}

	// Получаем числа
	first = stoi(line.substr(0, space_pos));
	second = stoi(line.substr(space_pos + 1));
}

int main() {

	string filename;

	cout << "Enter the filename: ";
	cin >> filename;

	try // Блок поиска исключений
	{
		ifstream fin(filename);

		if (!fin.is_open()) {
			// Файл не открылся
			throw file_exception("File cannot be opened");
		}

		size_t first, second;
		while (!fin.eof()) {
			// Очередная строка файла некорректна
			getNumbersFromLine(fin, first, second);

			if (second == 0) {
				// Второе число - 0
				throw file_exception("The divisor number is zero");
			}

			cout << (double)first / second << '\n';
		}
	}
	// Ловим исключения типа созданного класса и вызываем в нём метод, выводящий ошибки
	catch (const file_exception& except) {
		except.log("test.txt");
	}

	return 0;
}