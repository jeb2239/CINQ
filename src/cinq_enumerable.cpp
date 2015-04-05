#include "cinq_enumerable.hpp"

namespace cinq
{
    template <typename T>
    //requires Collection<T>()
    enumerable<T> from(T& source)
    {
        enumerable<decltype(source.cbegin())> e(source);
        return e;
    }
}
