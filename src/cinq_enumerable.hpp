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
        //TODO: fix all
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

        enumerable<TSource> skip(int count)
        {
            if (count >= 0) return skip((size_t)count);
            else throw invalid_argument("cinq: skip() was called with negative count");
        }

        enumerable<TSource> skip(size_t count)
        {

                //the could be a much better way to this
               // ensure_data();
            if(is_data_copied)
            {
                if(data.size()<count) throw invalid_argument("cinq: wtf"); 
                data.erase(data.cbegin(),data.cbegin()+count);
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
                begin = iter; //there will probably be a memory leak, still reachable
                
            }
            return *this;
        }


        
        
        //some weird shit is going on here with std::string
        //using Equality_comparable concept
        //weird syntax but it works
        template <TElement&>
        requires Equality_comparable<TElement&>()
        bool contains(TElement& elem)
        {
            ensure_data();

            for(TElement& i : data)
            {

                if(elem==i) return true;
            }

            return false;
        }

        template <TElement&>
        TElement& element_at(int index)
        {
            if(is_data_copied)
            {

                return data.at(index);
            }
            else
            {

                auto iter = begin;
                // This loop looks wrong, but the ending iterator should be 1 beyond the last element.
                while (index > 0 && end != iter)
                {
                    ++iter;
                    index--;
                }

               return *iter;
            }


        }

        
        TElement first()
        {

            if(is_data_copied) return data[0];
            else return *begin;

        }

        template <typename TFunc>
        requires Predicate<TFunc,TElement>()
        TElement first(TFunc predicate)
        {
            if(is_data_copied)
            {
            for(TElement& i: data)
                if(predicate(i)) return i;                       
                throw invalid_argument("cinq: no element satisfies the condition in predicate ");
            }

            auto iter = begin;
            while (!predicate(*iter) && end != iter) ++iter;

            if(!predicate(*iter))
                throw invalid_argument("cinq: no element satisfies the condition in predicate ");

            return *iter;

        }

        TElement last()
        {

           if(is_data_copied) return data[data.size()-1];
           else
            {
           auto iter=end;
            --iter;
            return *iter;
            }
        }

        template <typename TFunc>
        requires Predicate<TFunc,TElement>()
        TElement last(TFunc predicate)
        {

            if(is_data_copied)
            {
                for(size_t i=data.size()-1;i>=0;--i) if(predicate(data[i])) return data[i];

                throw invalid_argument("cinq: no element satisfies the condition in predicate ");
            }

            auto iter = end;
            --iter;
             //because end points to one past the end
            while(!predicate(*iter) && begin != iter) --iter;

            if(!predicate(*iter))
                throw invalid_argument("cinq: no element satisfies the condition in predicate ");

            return *iter;


        }

        TElement single()
        {
            if(is_data_copied )
            {
                if(data.size()>1) throw invalid_argument("cinq: structure has more than one element");
                else return data[0];
            }
            if(std::distance(begin,end)>1) //n time if container has no Random Access, constant otherwise
                throw invalid_argument("cinq: structure has more than one element");

            return *begin;

        }

        template <typename TFunc>
        requires Predicate<TFunc,TElement>()
        TElement single(TFunc predicate)
        {
                bool found = false;
                
                if(is_data_copied)
                {
                    size_t return_idx;
                for(size_t i=0;i<data.size();i++)
                {
                    if(found && predicate(data[i]))
                        throw invalid_argument("cinq: more than one element satisfies the predicate");
                    if(predicate(data[i]))
                    {
                     found = true;
                     return_idx=i;
                    }
                }
                return data[return_idx];
                }
                

                
                TElement rv;
                for(auto iter=begin;iter!=end;++iter)
                {
                    if(found && predicate(*iter))
                     throw invalid_argument("cinq: more than one element satisfies the predicate");
                    if(predicate(*iter))
                    {
                        found=true;
                        rv=*iter;

                    }
                }
                return rv;
        }




        vector<TElement> to_vector()
        {
            ensure_data();
            return data;
        }


        
        
        enumerable<TSource> order_by() requires Weakly_ordered<TElement&>()
        {
            ensure_data();

            std::sort(data.begin(),data.end());

            return *this;


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
