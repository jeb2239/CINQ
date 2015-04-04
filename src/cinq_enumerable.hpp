#ifndef __cinq_enumerable_hpp__
#define __cinq_enumerable_hpp__

#include <iostream>
#include <vector>

#include "all_concepts.hpp"

namespace cinq
{
    using namespace std;
    using namespace origin;
    
    template <typename TIter, typename TElement = typename iterator_traits<TIter>::value_type>
    class enumerable
    {
    public:
        template <typename TSource>
        //requires Container<TSource>()
        enumerable(TSource& source);
        
        template <typename TFunc>
        requires Predicate<TFunc>()
        enumerable<TIter> where(TFunc& f);
        
    private:
        TIter begin, end;
        vector<TElement> data;
        
        void ensure_data();
    };
    
    template <typename T>
    //requires Container<T>()
    enumerable<T> from(T& source);
}

#endif
