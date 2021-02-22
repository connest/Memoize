# Head-only реализация мемоизации

Класс мемоизации (сохранения результата функции: вместо очередного вызова
функции поиск ответа для параметров

Требования к типу:
- возможность вызова через ()
- чистота вызова: результат функции зависит только от входных параметров

Гарантии контейнера:
- проброс исключений вызывающему коду
- сохранение копий параметров

Поддерживается c++11 и выше.

### Работа с функторами

```
auto f = [](int a, double b) -> double {
    return a + b;
};

Memoize<double(int, double)> m(f);

cout << m(1, 4.9) << endl; // new
cout << m(1, 4.9) << endl; // from cache
cout << m(1, 4.9) << endl; // from cache
cout << m(2, 4.9) << endl; // new
cout << m(2, 4.9) << endl; // from cache
```

### Очищение кеша при необходимости

```
m.clear();                 // clear cache
cout << m(2, 4.9) << endl; // new
```

Работа со статическими фунциями - указатель на функцию

```
// статическия функция
string static_func(string a, string b) {
    return a + b;
}

// перегруженная статическая функция
int static_func(int a, int b) {
    return a - b;
}

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
```

### Работа в членами класса

```
class MaxClass{
public:
    // чистый метод класса
    double member_func(double a, double b) {
        return (a > b)? a : b;
    }
};

MaxClass obj;

// Преобразование члена-функции класса в функтор, принимающий указатель
// на объект класса
// ВАЖНО: метод является чистым
auto callable = std::mem_fn(&MaxClass::member_func);
Memoize<decltype(callable), MaxClass*, double, double> m_class(callable);

cout << m_class(&obj, 10.0, 20.0) << endl; // new
cout << m_class(&obj, 10.0, 20.0) << endl; // from cache
cout << m_class(&obj, 10.0, 20.0) << endl; // from cache
cout << m_class(&obj, 20.0, 21.0) << endl; // new
cout << m_class(&obj, 20.0, 21.0) << endl; // from cache
```
