#include <iostream>
#include <list>
#include <vector>

//------------1------------
void insertValue(std::list<float>& list)
{
    float temp = 0.0f;
    for (auto it : list)
    {
        temp += it;
    }
    temp /= list.size();
    list.push_back(temp);
}

void printList(const std::list<float>& list)
{
    for (auto it : list)
        std::cout << it << " ";
}

//------------2------------
class Matrix
{
public:
    Matrix(int n)
    {
        this->n = n;
        fillVector();
        printMatrix();
        getDet();
    }

    void fillVector()
    {
        std::vector<int> tempVec;
        for (int i = 0; i < this->n; i++)
        {
            for (int j = 0; j < this->n; j++)
            {
                int tempValue = 0;
                std::cout << "Enter value " << i + 1 << " " << j + 1 << " element: " << std::endl;
                std::cin >> tempValue;
                tempVec.push_back(tempValue);
            }
            this->matrix.push_back(tempVec);
            tempVec.clear();
        }
    }

    void getDet()
    {
        if (this->n == 1)
            std::cout << "Determimant = " << matrix[0].at(0) << std::endl;

        if (this->n == 2)
            std::cout << "Determimant = " << matrix[0].at(0) * matrix[1].at(1) - matrix[0].at(1) * matrix[1].at(0) << std::endl;

        if (this->n >= 3) //РЎРґРµР»Р°Р» С‚РѕР»СЊРєРѕ РґРѕ С‚СЂРµС‚СЊРµР№, С‚Р°Рє РєР°Рє СЃРёР¶Сѓ РЅР° СЂР°Р±РѕС‚Рµ Рё РґРµР»Р°СЋ РґРѕРјР°С€РЅРµРµ Р·Р°РґР°РЅРёРµ, РІСЂРµРјРµРЅРё РЅРµ С…РІР°С‚Р°РµС‚
        {
            std::cout << "Determimant = " << matrix[0].at(0) * (matrix[1].at(1) * matrix[2].at(2) - matrix[1].at(2) * matrix[2].at(1)) -
                matrix[0].at(1) * (matrix[1].at(0) * matrix[2].at(2) - matrix[2].at(0) * matrix[1].at(2)) +
                matrix[0].at(2) * (matrix[1].at(0) * matrix[2].at(1) - matrix[2].at(0) * matrix[1].at(1)) << std::endl;
        }
    }

    void printMatrix()
    {
        for (int i = 0; i < this->n; i++)
        {
            for (int j = 0; j < this->n; j++)
            {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
private:
    int n;
    std::vector<std::vector<int>> matrix;
};


//------------3------------
template <typename T>
class charArray {
public:
    class iterator {
    public:
        iterator(T* ptr) : ptr(ptr) {}
        iterator operator++() { ++ptr; return *this; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
        const T& operator*() const { return *ptr; }
    private:
        T* ptr;
    };
private:
    unsigned len;
    T* val;
public:
    iterator begin() const { return iterator(val); }
    iterator end() const { return iterator(val + len); }

};

int main()
{
    //------------1------------
    std::list<float> myGlist{ 1,2,3,4,5,6 };
    printList(myGlist);
    insertValue(myGlist);
    std::cout << std::endl;
    printList(myGlist);
    std::cout << std::endl;

    //------------2------------
    int n = 0;
    std::cout << "Enter N: " << std::endl;
    std::cin >> n;
    while (n < 0)
    {
        std::cout << "Error, invalid value" << std::endl;
        std::cin >> n;
    }
    Matrix a(n);

    //------------3------------
    charArray<char> array;
    for (auto& c : array)
        std::cout << "char: " << c << std::endl;

} 