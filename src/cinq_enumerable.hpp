#ifndef __cinq_enumerable_hpp__
#define __cinq_enumerable_hpp__

#include <iostream>
#include <vector>
#include <stdexcept>

#include "all_concepts.hpp"
#include "cinq_test.hpp"

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
        
        // ARITHMETIC
        
        // Count

        template <typename TFunc>
        requires Predicate<TFunc,TElement>()
        size_t count(TFunc predicate)
        {
            ensure_data();
            size_t count=0;

            for(TElement i : data) 
              {
               // printf("%u\n", *iter );
                if(predicate(i)) ++count;
            }
            return count;

        }
        
        size_t count() requires Random_access_iterator<TIter>()
        {   
            if (is_data_copied) return data.size();
            else return end - begin;
        }
        
        size_t count() requires Forward_iterator<TIter>()
        {
            if (is_data_copied) return data.size();
            else
            {
                size_t count = 0;
                for (auto iter = begin; iter != end; ++iter) count++;
                return count;
            }
        }
        
        // Max
        
        // This is where the power of C++ templates & concepts really shines. You can't do this
        // in any other mainstream language. Check out the C# implementation and cry...
        // https://github.com/mono/mono/blob/effa4c0/mcs/class/System.Core/System.Linq/Enumerable.cs
        
        template <typename TFunc, typename TReturn = typename result_of<TFunc(TElement)>::type>
        requires Invokable<TFunc, TElement>() && Number<TReturn>()
        TReturn max(TFunc predicate)
        {
            if (count() == 0) throw length_error("cinq: sequence is empty");
            
            auto seq_begin = (is_data_copied ? data.cbegin() : begin);
            auto seq_end   = (is_data_copied ? data.cend()   : end  );
            
            TReturn max = numeric_limits<TReturn>::min();
            for (auto iter = seq_begin; iter != seq_end; ++iter)
            {
                TReturn val = predicate(*iter);
                if (val > max) max = val;
            }
            
            return max;
        }
        
        // TODO: This could call the other max override w/ a lambda that returns itself, if we
        // figure out lambda inlining.
        TElement max() requires Number<TElement>()
        {
            if (count() == 0) throw length_error("cinq: sequence is empty");
            
            auto seq_begin = (is_data_copied ? data.cbegin() : begin);
            auto seq_end   = (is_data_copied ? data.cend()   : end  );
            
            TElement max = numeric_limits<TElement>::min();
            for (auto iter = seq_begin; iter != seq_end; ++iter)
            {
                TElement val = *iter;
                if (val > max) max = val;
            }
            
            return max;
        }
        
        template <typename TFunc>
        requires Predicate<TFunc,TElement>()
        bool all(TFunc predicate)
        {
            ensure_data();
            for(TElement i : data){
                if(!predicate(i)) return false;
            }

            return true;
        }
        
        enumerable<TSource> take(size_t count)
        {
            if (is_data_copied)
            {
                // TODO: resize() is O(N) operation. See if there is a better way.
                if (data.size() > count) data.resize(count);
            }
            else
            {
                auto iter = begin;
                // This loop looks wrong, but the ending iterator should be 1 beyond the last element.
                while (count > 0 && end != iter)
                {
                    ++iter;
                    count--;
                }
                end = iter;
            }
            
            return *this;
        }
        
        // Try to catch a negative count before it gets casted into a huge size_t.
        enumerable<TSource> take(int count)
        {
            if (count >= 0) return take((size_t)count);
            else throw invalid_argument("cinq: take() was called with negative count");
        }
        
        //some weird shit is going on here with std::string
        //using Equality_comparable concept
        //weird syntax but it works
        template <TElement&>
        requires Equality_comparable<TElement&>()
        bool contains(TElement& elem)
        {
            ensure_data();

            for(TElement& i : data){

                if(elem==i) return true;
            }

            return false;
        }
        
        vector<TElement> to_vector()
        {
            ensure_data();
            return data;
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
    
    // Allow automated tests to access private stuff.
    friend class test;
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
