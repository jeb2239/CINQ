#ifndef __custom_concepts_hpp__
#define __custom_concepts_hpp__

#include "all_concepts.hpp"
#include <type_traits>

using namespace origin;

template<typename _Iter>
    concept bool Sortable()
    {
      return Forward_iterator<_Iter>()
          && Permutable<_Iter>()
          && Weakly_ordered<Value_type<_Iter>>();
    }

template<typename _Iter, typename _Compare>
    concept bool Sortable()
    {
      return Forward_iterator<_Iter>()
          && Permutable<_Iter>()
          && Relation<_Compare, Value_type<_Iter>>();
    }

template<typename _Iter1, typename _Iter2, typename _Out>
    concept bool Mergeable()
    {
      return Indirectly_ordered<_Iter1, _Iter2>()
          && Indirectly_copyable<_Iter1, _Out>()
          && Indirectly_copyable<_Iter2, _Out>();
    }

template<typename _Iter1, typename _Iter2, typename _Out, typename _Comp>
    concept bool Mergeable()
    {
      return Indirectly_comparable<_Iter1, _Iter2, _Comp>()
          && Indirectly_copyable<_Iter1, _Out>()
          && Indirectly_copyable<_Iter2, _Out>();
    }
 






#endif