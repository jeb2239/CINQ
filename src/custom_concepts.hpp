#ifndef __custom_concepts_hpp__
#define __custom_concepts_hpp__

#include "all_concepts.hpp"
#include <type_traits>

using namespace origin;

template<typename _Iter>
    constexpr bool Sortable()
    {
      return Forward_iterator<_Iter>()
          && Permutable<_Iter>()
          && Weakly_ordered<Value_type<_Iter>>();
    }

#endif