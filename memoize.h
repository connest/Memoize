#ifndef MEMOIZE_H
#define MEMOIZE_H

#include <map>
#include <tuple>
#include <functional>

namespace connest {
/**
 * @brief The Memoize class
 *
 * Класс мемоизации (сохранения результата функции: вместо очередного вызова
 * функции поиск ответа для параметров)
 *
 * @template Func       Тип вызываемой фукции
 * @template Args...    Параметры фукнции
 *
 * Требования к типу:
 *  - возможность вызова через ()
 *  - чистота вызова: результат функции зависит только от входных параметров
 *  - сравниваемость или перегруженный оператор < у аргументов
 *
 * Гарантии контейнера:
 *  - проброс исключений вызывающему коду
 *  - сохранение копий параметров
 */

template <typename Unused>
class Memoize{};

template <typename ReturnType,
          typename ... Args>
class Memoize<ReturnType(Args...)> {
    protected:

    std::function<ReturnType(Args...)> function;
    std::map<std::tuple<Args...>, ReturnType> cache;

    public:
    /**
     * @brief Memoize Конструктор
     * @param func std::function интерфейс
     */
    explicit Memoize(ReturnType(*func)(Args...))
        : function {std::move(std::function<ReturnType(Args...)>(func))}
    {}

    /**
     * @brief Memoize Конструктор
     * @param Functor функтор
     */
    template <typename Functor>
    explicit Memoize(Functor functor)
        : function(std::move(functor))
    {}

    Memoize(const Memoize&) = default;
    Memoize(Memoize&&) = default;

    /**
     * @brief operator () Поиск ответа и/или вызов функции
     * @param args Аргументы функции
     * @return результат работы функции
     */
    ReturnType operator()(Args&&... args)
    {
        std::tuple<Args...> tupled_args { std::make_tuple(args...) };

        auto iter = cache.lower_bound(tupled_args);
        if((iter != cache.end()) && (iter->first == tupled_args)) {
            return iter->second;

        } else {
            ReturnType return_value = function(std::forward<Args>(args)...);
            cache.emplace_hint(iter, tupled_args, return_value);

            return return_value;
        }

    }

    /**
     * @brief clear Очистить кеш
     */
    void clear()
    {
        cache.clear();
    }
};

}

#endif // MEMOIZE_H
