#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <optional>
#include <tuple>

//------------1------------
template <typename T>
void Swap(T&& t1, T&& t2)
{
    auto temp = t1;
    t1 = t2;
    t2 = temp;
}

//------------2------------
template <typename S>
void SortPointers(std::vector<S>& vecPtr)
{
    std::sort(vecPtr.begin(), vecPtr.end(), [](const S& t1, const S& t2) { return *t1 < *t2; });
}

//------------3------------
class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::string m_name;
	std::chrono::time_point<clock_t> m_beg;
	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}

public:
	Timer() : m_beg(clock_t::now()) { }
	Timer(std::string name) : m_name(name), m_beg(clock_t::now()) { }

	void start(std::string name) {
		m_name = name;
		m_beg = clock_t::now();
	}
	void print() const {
		std::cout << m_name << ":\t" << elapsed() * 1000 << " ms" << '\n';
	}
};

int main()
{
    //------------1------------
    int a = 7;
    int b = 10;
    int* ptr1 = new int;
    int* ptr2 = new int;
    *ptr1 = a;
    *ptr2 = b;
    std::cout << ptr1 << " " << *ptr1 << " " << ptr2  << " " << *ptr2 << " " << &a << " " << &b <<  std::endl;
    Swap(ptr1, ptr2);
    std::cout << ptr1 << " " << *ptr1 << " " << ptr2 << " " << *ptr2 << " " << &a << " " << &b << std::endl;
    /*000001A7B0696710 7 000001A7B0696850 10 0000007ABBDFF3D4 0000007ABBDFF3F4
      000001A7B0696850 10 000001A7B0696710 7 0000007ABBDFF3D4 0000007ABBDFF3F4*/

    //------------2------------
	std::vector<int> vec = { 5, 1, 3, 4, 8, 6, 7, 2, 9 };
	std::vector<int*> vecPtr;
	for (auto iter = 0; iter < vec.size(); iter++)
	{
		vecPtr.push_back(&vec[iter]);
	}

	for (auto iter : vecPtr)
	{
		std::cout << *iter << " ";
	}

	SortPointers(vecPtr);
	std::cout << "\n After sort:" << std::endl;
	for (auto iter : vecPtr)
	{
		std::cout << *iter << " ";
	}
	std::cout << std::endl;

	//------------3------------

	std::string buffer, book, vowelLetters = { "АаУуОоЫыИиЭэЯяЮюЁёЕеAaàEeèéêIiOoôUuûYy" };

	int count = 0;
	std::fstream file;
	file.open("voina-i-mir.txt");

	if (file.is_open())
	{
		std::cout << "\nFile is open." << std::endl;
		for (std::getline(file, buffer); !file.eof(); std::getline(file, buffer))
		{
			book += buffer;
		}
		buffer = "";
		buffer.shrink_to_fit();
	}
	else
	{
		std::cout << "File isn'n open!" << std::endl;
	}
	file.close();

	//------------3.1------------

	auto start = std::chrono::steady_clock::now();
	count = std::count_if(book.begin(), book.end(), [&vowelLetters](const auto ch)
		{ return std::find(vowelLetters.begin(), vowelLetters.end(), ch) != vowelLetters.end(); });
	std::cout << std::endl << "Number of vowels = " << count << std::endl;
	count = 0;

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> seconds = end - start;
	std::cout << "count_if и find = " << seconds.count() << "s" << std::endl;

	//------------3.2------------

	start = std::chrono::steady_clock::now();
	count = std::count_if(book.begin(), book.end(), [&vowelLetters](const auto ch)
		{ for (auto iter : vowelLetters)
	{
		if (iter == ch)
		{
			return true;
		}
	}
	return false;
		});
	std::cout << std::endl << "Number of vowels = " << count << std::endl;
	count = 0;

	end = std::chrono::steady_clock::now();
	seconds = end - start;
	std::cout << "count_if and for = " << seconds.count() << "s" << std::endl;

	//------------3.3------------

	start = std::chrono::steady_clock::now();
	for (auto iter : book)
	{
		if (std::find(vowelLetters.begin(), vowelLetters.end(), iter) != vowelLetters.end())
		{
			count++;
		}
	}
	count = std::count_if(book.begin(), book.end(), [&vowelLetters](const auto ch)
		{ return std::find(vowelLetters.begin(), vowelLetters.end(), ch) != vowelLetters.end(); });
	std::cout << std::endl << "Number of vowels = " << count << std::endl;
	count = 0;

	end = std::chrono::steady_clock::now();
	seconds = end - start;
	std::cout << "for and find = " << seconds.count() << "s" << std::endl;

	//------------3.4------------

	start = std::chrono::steady_clock::now();
	for (auto iter1 : book)
	{
		for (auto iter2 : vowelLetters)
		{
			if (iter1 == iter2)
			{
				count++;
				break;
			}
		}
	}
	std::cout << std::endl << "Number of vowels = " << count << std::endl;

	end = std::chrono::steady_clock::now();
	seconds = end - start;
	std::cout << "a couple of for = " << seconds.count() << "s" << std::endl;

	book = "";
	book.shrink_to_fit();

	return 0;
}