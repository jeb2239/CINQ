#ifndef __cinq_enumerable_hpp__
#define __cinq_enumerable_hpp__

#include <iostream>
#include <vector>

#include "all_concepts.hpp"

namespace cinq
{
    using namespace std;
    using namespace origin;
    
    template <typename TSource, typename TElement = typename TSource::value_type, typename TIter = typename TSource::const_iterator>
    class enumerable
    {
    public:
        //template <typename TSource>
        //requires Container<TSource>()
        enumerable(TSource& source)
        {
            is_data_copied = false;
            begin = source.cbegin();
            end = source.cend();
        }
        
        template <typename TFunc>
        requires Predicate<TFunc, TElement>()
        enumerable<TSource> where(TFunc predicate)
        {
            ensure_data();
            
            for (size_t i = 0; i < data.size(); i++)
            {
                if (!predicate(data[i]))
                {
                    data.erase(data.cbegin() + i);
                    i--;
                }
            }
            
            return *this;
        }
        
        template <typename TFunc>
        requires Predicate<TFunc, TElement, size_t>()
        enumerable<TSource> where(TFunc predicate)
        {
            ensure_data();
            
            size_t original_index = 0;
            for (size_t i = 0; i < data.size(); i++)
            {
                if (!predicate(data[i], original_index))
                {
                    data.erase(data.cbegin() + i);
                    i--;
                }
                original_index++;
            }
            
            return *this;
        }

        template <typename TFunc>
        requires Predicate<TFunc,TElement>()
        int count(TFunc predicate)
        {

            int count=0;
            for(auto iter=begin; iter != end; ++iter) 
              {
               // printf("%u\n", *iter );
                if(predicate(*iter)) ++count;
            }
            return count;

        }
        //over load which takes no arguments
        //if we do some type introspection this one could be faster
        int count()
        {
            int count=0;
            for(auto iter=begin; iter != end; ++iter) ++count;

            return count;
        }

        template <typename TFunc>
        requires Predicate<TFunc,TElement>()
        bool all(TFunc predicate)
        {
            for(auto iter=begin; iter !=end; ++iter){
                if(!predicate(*iter)) return false;
            }

            return true;
        }

    

        auto take(int index){
            ensure_data();
            for(size_t i=index; i<(size_t)index-data.size();i++){
                data.pop_back();
            }
            return data;

        }



        vector<TElement> to_vector()
        {
            return data;
        }

        //using Equality_comparable concept
        //weird syntax but it works
        template <TElement>
        requires Equality_comparable<TElement>()
        bool contains(TElement elem)
        {
            for(auto iter=begin; iter !=end; ++iter)
            {
                if(elem == *iter) return true;
            }

            return false;
        }
        
    private:
        TIter begin, end;
        vector<TElement> data;
        bool is_data_copied;
        
        void ensure_data()
        {
            if (is_data_copied) return;
            
            vector<TElement> copy;
            for (auto iter = begin; iter != end; ++iter) copy.push_back(*iter);
            
            is_data_copied=true; // should be set to true after copy right
            data = copy;
        }
    };
    
    template <typename T>
    //requires Container<T>()
    auto from(T& source)
    {
        //enumerable<decltype(source.cbegin())> e(source);
        enumerable<T> e(source);
        return e;
    }


}

#endif
