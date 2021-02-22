#ifndef MEMOIZE_H
#define MEMOIZE_H

#include <map>
#include <tuple>
#include <functional>

/**
 * @brief The Memoize class
 *
 * Класс мемоизации (сохранения результата функции: вместо очередного вызова
 * функции поиск ответа для параметров
 *
 * @template Func       Тип вызываемой фукции
 * @template Args...    Параметры фукнции
 *
 * Требования к типу:
 *  - возможность вызова через ()
 *  - чистота вызова: результат функции зависит только от входных параметров
 *
 * Гарантии контейнера:
 *  - проброс исключений вызывающему коду
 *  - сохранение копий параметров
 */


template <typename Func,
          typename ...Args>
class Memoize {
protected:
    using return_type = typename std::result_of<Func(Args&&...)>::type;

    Func function;
    std::map<std::tuple<Args...>, return_type> cache;

public:
    /**
     * @brief Memoize Конструктор
     * @param function функтор, указатель на фукнцию
     */
    Memoize(Func function)
        : function(std::move(function))
    {}

    Memoize(const Memoize&) = default;
    Memoize(Memoize&&) = default;

    /**
     * @brief operator () Поиск ответа и/или вызов функции
     * @param args Аргументы функции
     * @return результат работы функции
     */
    return_type operator()(Args&&... args)
    {
        std::tuple<Args...> tupled_args { std::make_tuple(args...) };

        auto iter = cache.lower_bound(tupled_args);
        if((iter != cache.end()) && (iter->first == tupled_args)) {
            return iter->second;

        } else {
            return_type return_value = function(std::forward<Args>(args)...);
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

template <typename ReturnType,
          typename ...Args>
class Memoize<ReturnType(Args...)> {
protected:
    ReturnType (*function)(Args...);
    std::map<std::tuple<Args...>, ReturnType> cache;

public:
    /**
     * @brief Memoize Конструктор
     * @param указатель на фукнцию
     */
    Memoize(ReturnType (*function)(Args...))
        : function(std::move(function))
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

#endif // MEMOIZE_H
