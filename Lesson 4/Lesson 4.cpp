#include <iostream>
#include <vector>
#include <list>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <optional>
#include <tuple>
#include <algorithm>

//----------------1----------------
template <typename C, typename V>
void insert_sorted(C& container, V&& value)
{
	auto it = std::find_if(std::begin(container), std::end(container), [value](auto containerValue) { return containerValue > value; });
	container.emplace(it, value);
}

template <typename T>
void printContainer(const T& cont)
{
	for (auto iter : cont)
		std::cout << iter << ' ';
	std::cout << std::endl;
}

int main()
{
	//----------------1----------------
	std::vector<int> vec1 = { 1, 2, 4, 5, 6, 7, 9 };
	printContainer(vec1);

	insert_sorted(vec1, 3);
	printContainer(vec1);

	std::list<int> list1 = { 1, 2, 3, 5, 7, 8, 9 };
	printContainer(list1);

	insert_sorted(list1, 6);
	printContainer(list1);
	std::cout << std::endl;

	//----------------2----------------
	std::cout << std::endl;

	std::srand(std::time(nullptr));

	std::vector<double> a;
	a.resize(100);
	std::generate(a.begin(), a.end(), []()
		{
			double temp = std::rand(); 
			return temp / 1000;
		});
	printContainer(a);
	std::cout << std::endl;

	std::vector<int> b;
	b.resize(100);
	std::copy(a.begin(), a.end(), b.begin());
	printContainer(b);
	std::cout << std::endl;

	double errorRange = 0;
	std::for_each(a.begin(), a.end(), [&b, &errorRange](auto& value)
	{	
		static size_t iter = 0;
		errorRange += (value - b.at(iter)) * (value - b.at(iter));
		iter++;
	});
	std::cout << "Error rate: " << errorRange;
	std::cout << std::endl;

	return 0;
}
