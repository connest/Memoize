#include "memoize.h"

#include <iostream>
#include <string>
#include <functional>

using namespace std;


// статическия функция
string static_func(string a, string b) {
    return a + b;
}

// перегруженная статическая функция
int static_func(int a, int b) {
    return a - b;
}

class MaxClass{
public:
    // чистый метод класса
    double member_func(double a, double b) {
        return (a > b)? a : b;
    }
};

int main()
{
    using namespace connest;
    // Работа с функторами

    auto f = [](int a, double b) -> double {
        return a + b;
    };

    Memoize<double(int, double)> m(f);

    cout << m(1, 4.9) << endl; // new
    cout << m(1, 4.9) << endl; // from cache
    cout << m(1, 4.9) << endl; // from cache
    cout << m(2, 4.9) << endl; // new
    cout << m(2, 4.9) << endl; // from cache

    m.clear();                 // clear cache
    cout << m(2, 4.9) << endl; // new


    // Работа со статическими фунциями - указатель на функцию
    // использование decltype() невозможно, так как функция перезгружена - нужно
    // уточнение параметров
    Memoize<int(int, int)> msf(&static_func);
    Memoize<string(string, string)> overloaded(&static_func);

    cout << msf(4,3)<<endl; // new
    cout << msf(4,3)<<endl; // from cache
    cout << msf(4,3)<<endl; // from cache
    cout << msf(1,3)<<endl; // new
    cout << msf(1,3)<<endl; // from cache


    cout << overloaded("z ", "y") << endl; // new
    cout << overloaded("z ", "y") << endl; // from cache
    cout << overloaded("z ", "y") << endl; // from cache
    cout << overloaded("c ", "s") << endl; // new
    cout << overloaded("c ", "s") << endl; // from cache

    // Работа с методами класса
    // Преобразование члена-функции класса в функтор, принимающий указатель
    // на объект класса
    // ВАЖНО: метод является чистым
    Memoize<double(MaxClass*, double, double)> m_class(&MaxClass::member_func);

    MaxClass obj;

    cout << m_class(&obj, 10.0, 20.0) << endl; // new
    cout << m_class(&obj, 10.0, 20.0) << endl; // from cache
    cout << m_class(&obj, 10.0, 20.0) << endl; // from cache
    cout << m_class(&obj, 20.0, 21.0) << endl; // new
    cout << m_class(&obj, 20.0, 21.0) << endl; // from cache


    // std::function interface
    std::function<double(int, double)> functor{f};
    Memoize<double(int, double)> m_functor(functor);


    cout << m(1, 4.9) << endl; // new
    cout << m(1, 4.9) << endl; // from cache
    cout << m(1, 4.9) << endl; // from cache
    cout << m(2, 4.9) << endl; // new
    cout << m(2, 4.9) << endl; // from cache

    return 0;
}
