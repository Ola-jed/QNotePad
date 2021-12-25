#ifndef QNOTEPAD_LAZY_HPP
#define QNOTEPAD_LAZY_HPP

#include <optional>
#include <functional>

template<class T>
class Lazy
{
private:
    std::function<T()> generatorFunction;
    std::optional<T> valueHolder = std::nullopt;
public:
    Lazy() = default;
    explicit Lazy(const std::function<T()>& generatorFunction):
            generatorFunction(generatorFunction)
    {
    }

    T get()
    {
        if (!valueHolder.has_value())
        {
            valueHolder = generatorFunction();
        }
        return valueHolder.value();
    }
};

#endif //QNOTEPAD_LAZY_HPP
