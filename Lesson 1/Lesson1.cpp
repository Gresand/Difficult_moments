#include <iostream>
#include <optional>
#include <fstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <iomanip>
#include <string>


//----------------1----------------
struct Person
{
	std::string name;
	std::string surname;
	std::optional<std::string> patronymic;
};

std::ostream& operator<<(std::ostream& out, Person person)
{
	out << person.surname << person.name << " " << person.patronymic.has_value() ? person.patronymic.value() : "";
	return out;
}

bool operator<(const Person& person1, const Person& person2)
{
	return std::tie(person1.surname, person1.name, person1.patronymic.value()) < 
			std::tie(person2.surname, person2.name, person2.patronymic.value());
}

bool operator==(const Person& person1, const Person& person2)
{
	return std::tie(person1.surname, person1.name, person1.patronymic.value()) ==
		std::tie(person2.surname, person2.name, person2.patronymic.value());
}

//----------------2----------------
struct PhoneNumber
{
	uint16_t countryCode;
	uint16_t cityCode;
	std::string number;
	std::optional<uint16_t> addNumber;
};

std::ostream& operator<<(std::ostream& out, PhoneNumber phoneNumber)
{
	out << "+" << phoneNumber.countryCode << "(" << phoneNumber.cityCode << ")" << phoneNumber.number << " " << phoneNumber.addNumber.has_value() ? phoneNumber.addNumber.value() : out.end;
	return out;
}

bool operator<(const PhoneNumber& phoneNumber1, const PhoneNumber& phoneNumber2)
{
	return std::tie(phoneNumber1.countryCode, phoneNumber1.cityCode, phoneNumber1.number, phoneNumber1.addNumber.value()) <
		std::tie(phoneNumber2.countryCode, phoneNumber2.cityCode, phoneNumber2.number, phoneNumber2.addNumber.value());
}

bool operator==(const PhoneNumber& phoneNumber1, const PhoneNumber& phoneNumber2)
{
	return std::tie(phoneNumber1.countryCode, phoneNumber1.cityCode, phoneNumber1.number, phoneNumber1.addNumber.value()) ==
		std::tie(phoneNumber2.countryCode, phoneNumber2.cityCode, phoneNumber2.number, phoneNumber2.addNumber.value());
}

//----------------3----------------
class PhoneBook
{
private:
	std::vector<std::pair<Person, PhoneNumber>> humansNumber;
public:
	PhoneBook(std::ifstream& file)
	{
		std::string tempString;
		if (file.is_open())
		{
			while (!file.eof())
			{
				std::getline(file, tempString);
				humansNumber.push_back(std::pair<Person, PhoneNumber>(std::string(tempString.substr(0, tempString.find('+') - 1)), std::string(tempString.substr(tempString.find('+') + 1, ((tempString.size() - 1) - tempString.find('+'))))));
			}
			auto i = tempString.substr(0, tempString.find('+') - 2);
		}
		file.close();
	}
	friend std::ostream& operator<<(std::ostream& os, const PhoneBook& pb);
	void SortByName()
	{
		std::sort(humansNumber.begin(), humansNumber.end(), [](const std::pair<Person, PhoneNumber>& a, const std::pair<Person, PhoneNumber>& b)
			{ return a.first < b.first; });
	}

	void SortByPhone()
	{
		std::sort(humansNumber.begin(), humansNumber.end(), [](const std::pair<Person, PhoneNumber>& a, const std::pair<Person, PhoneNumber>& b)
			{ return a.second < b.second; });
	}

	std::tuple<std::string, PhoneNumber> GetPhoneNumber(std::string targetSurname)
	{
		std::vector<std::vector<std::pair<Person, PhoneNumber>>::iterator> personsFound;
		for (auto iter = humansNumber.begin(); iter != humansNumber.end(); iter++)
		{
			if ((*iter).first == Person(targetSurname, "", std::nullopt))
			{
				personsFound.push_back(iter);
			}
		}
		if (!personsFound.empty())
		{
			if (personsFound.size() > 1)
			{
				return std::make_tuple("found more than 1", PhoneNumber(0, 0, "000", std::nullopt));
			}
			return std::tie("", (*personsFound[0]).second);
		}
		return std::make_tuple("not found", PhoneNumber(0, 0, "000", std::nullopt));
	}
	void ChangePhoneNumber(Person per, PhoneNumber pn)
	{
		for (auto iter = humansNumber.begin(); iter != humansNumber.end(); iter++)
		{
			if ((*iter).first == per)
			{
				(*iter).second = pn;
			}
		}
	}
};

std::ostream& operator<<(std::ostream& os, const PhoneBook& pb)
{
	for (auto iter : pb.humansNumber)
	{
		os << iter.first << iter.second << std::endl;
	}
	return os;
}

int main()
{
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	std::ifstream file("PhoneBook.txt"); // путь к файлу PhoneBook.txt PhoneBook book(file);
	PhoneBook book(file);
	std::cout << "------SortByPhone-------" << std::endl;
	book.SortByPhone();
	std::cout << book;
	std::cout << "------SortByName--------" << std::endl;
	book.SortByName();
	std::cout << book;
	std::cout << "-----GetPhoneNumber-----" << std::endl;
	// лямбда функция, которая принимает фамилию и выводит номер телефона
	auto print_phone_number = [&book](const std::string& surname) {
		std::cout << surname << "\t";
	auto answer = book.GetPhoneNumber(surname);
	if (get<0>(answer).empty())
		std::cout << get<1>(answer);
	else
		std::cout << get<0>(answer);
	std::cout << std::endl;
	};
	// вызовы лямбды
	print_phone_number("Ivanov");
	print_phone_number("Petrov");
	std::cout << "----ChangePhoneNumber----" << std::endl;
	book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" },
		PhoneNumber{ 7, 123, "15344458", std::nullopt });
	book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" },
		PhoneNumber{ 16, 465, "9155448", 13 });
	std::cout << book;

	SetConsoleCP(866);
	SetConsoleOutputCP(866);

	return 0;
}

/*
3. Создайте класс PhoneBook, который будет в контейнере хранить пары: Человек – Номер телефона. Конструктор этого класса должен принимать параметр типа ifstream – поток данных, полученных из файла. 
В теле конструктора происходит считывание данных из файла и заполнение контейнера. Класс PhoneBook должен содержать перегруженный оператор вывода, для вывода всех данных из контейнера в консоль.
В классе PhoneBook реализуйте метод SortByName, который должен сортировать элементы контейнера по фамилии людей в алфавитном порядке.
Если фамилии будут одинаковыми, то сортировка должна выполняться по именам, если имена будут одинаковы, то сортировка производится по отчествам. Используйте алгоритмическую функцию sort.
Реализуйте метод SortByPhone, который должен сортировать элементы контейнера по номерам телефонов. Используйте алгоритмическую функцию sort.
Реализуйте метод GetPhoneNumber, который принимает фамилию человека, а возвращает кортеж из строки и PhoneNumber.
Строка должна быть пустой, если найден ровно один человек с заданном фамилией в списке. 
Если не найден ни один человек с заданной фамилией, то в строке должна быть запись «not found», если было найдено больше одного человека, то в строке должно быть «found more than 1».
Реализуйте метод ChangePhoneNumber, который принимает человека и новый номер телефона и, если находит заданного человека в контейнере, то меняет его номер телефона на новый, иначе ничего не делает.
Функция main будет выглядеть так:

int main() {
этого
ifstream file("ХХХ"); // путь к файлу PhoneBook.txt PhoneBook book(file);
cout << book;
cout << "------SortByPhone-------" << endl;
book.SortByPhone();
cout << book;
cout << "------SortByName--------" << endl;
book.SortByName();
cout << book;
cout << "-----GetPhoneNumber-----" << endl;
// лямбда функция, которая принимает фамилию и выводит номер телефона
человека, либо строку с ошибкой
auto print_phone_number = [&book](const string& surname) {
cout << surname << "\t";
auto answer = book.GetPhoneNumber(surname);
if (get<0>(answer).empty())
cout << get<1>(answer);
else
cout << get<0>(answer);
cout << endl;
};
// вызовы лямбды
print_phone_number("Ivanov");
print_phone_number("Petrov");
cout << "----ChangePhoneNumber----" << endl;
book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" },
PhoneNumber{7, 123, "15344458", nullopt});
book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" },
PhoneNumber{ 16, 465, "9155448", 13 });
cout << book;
}
У вас должен получиться следующий вывод: (см. Методичка урока 1, стр. 28-29)*/