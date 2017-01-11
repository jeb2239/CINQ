#ifndef __cinq_enumerable_hpp__
#define __cinq_enumerable_hpp__

#include <iostream>
#include <vector>
#include <stdexcept>
#include <tuple>
#include <typeinfo>

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
        /**
         * @brief A wrapper which allows cinq to operate on various containers
         *
         * @param source the source container which
         * provides the data cinq operates on
         */
        enumerable(TSource& source) //requires Range<TSource>()
        {
            is_data_copied = false;
            begin = source.cbegin();
            end = source.cend();
        }

    private:
        
        enumerable()
        {
           is_data_copied=false;
        }
        
    public:
        
        /**
         * @brief Filters a sequence of values based on a predicate.
         * Each element's index may be used in the logic of the predicate function.
         *
         * @param predicate A function to test each element for a condition.
         * @return An enumerable that contains elements from the input sequence that satisfy the condition.
         */
        template <typename TFunc>
        requires Predicate<TFunc, TElement>() || Predicate<TFunc, TElement, size_t>()
        enumerable<TSource> where(TFunc predicate)
        {
            if (is_data_copied) where(predicate, data.cbegin(), data.cend());
            else where(predicate, begin, end);

            return *this;
        }

    private:

        template <typename TFunc, typename TIterator>
        requires Predicate<TFunc, TElement>()
        void where(TFunc predicate, TIterator begin, TIterator end)
        {
            vector<TElement> updated;
            for (auto iter = begin; iter != end; ++iter)
            {
                if (predicate(*iter)) updated.push_back(*iter);
            }
            data = updated;
            is_data_copied = true;
        }

        template <typename TFunc, typename TIterator>
        requires Predicate<TFunc, TElement, size_t>()
        void where(TFunc predicate, TIterator begin, TIterator end)
        {
            vector<TElement> updated;
            size_t i = 0;
            for (auto iter = begin; iter != end; ++iter)
            {
                if (predicate(*iter, i)) updated.push_back(*iter);
                i++;
            }
            data = updated;
            is_data_copied = true;
        }

    public:

        /**
         * @brief Determines whether a sequence contains any elements.
         *
         * @return true if the source sequence contains any elements; otherwise, false.
         */
        bool any()
        {
            return !empty();
        }

        /**
         * @brief Determines whether any element of a sequence satisfies a condition.
         *
         * @param predicate A function to test each element for a condition.
         * @return true if any elements in the source sequence pass the test
         * in the specified predicate; otherwise, false.
         */
        template <typename TFunc>
        requires Predicate<TFunc,TElement>()
        bool any(TFunc predicate)
        {
            ensure_data();
            for(TElement i : data){
                if(predicate(i)) return true;
            }

            return false;
        }

        /**
         * @brief Concatenate another enumerable to this enumerable.
         *
         * @param other the enumerable to append
         * @return this enumerable with the other enumerable appended
         */
        enumerable<TSource> concat(enumerable<TSource> other)
        {
            ensure_data();

            if (other.is_data_copied) data.insert(data.end(), other.data.cbegin(), other.data.cend());
            else data.insert(data.end(), other.begin, other.end);

            return *this;
        }

        // Workaround which allows access to other enumerable instantiations' private members.
        template <typename TSourceFriend, typename TElementFriend, typename TIterFriend>
        friend class enumerable;

        template <typename TFunc>
        requires Function<TFunc, TElement>() || Function<TFunc, TElement, size_t>()
        auto select(TFunc fun)
        {
            if(is_data_copied) return select(fun,data.cbegin(),data.cend());
            else return select(fun,begin,end);
        }

    private:
        
        template <typename TFunc, typename TReturn = typename std::result_of<TFunc(const TElement&)>::type,typename TIterator>
        requires Function<TFunc, const TElement&>() && Copy_constructible<TReturn>()
        enumerable<vector<TReturn>> select(TFunc fun,TIterator begin, TIterator end )
        {
            
            enumerable<vector<TReturn>> updated;
            for (auto iter = begin; iter != end; ++iter) updated.data.push_back(fun(*iter));
            updated.is_data_copied = true;

            return updated;
        }

        template <typename TFunc, typename TReturn = typename std::result_of<TFunc(const TElement&, size_t)>::type,typename TIterator>
        requires Function<TFunc, const TElement&, size_t>() && Copy_constructible<TReturn>()
        enumerable<vector<TReturn>> select(TFunc fun,TIterator begin, TIterator end)
        {
            enumerable<vector<TReturn>> updated;

            size_t index = 0;
            for (auto iter = begin; iter != end; ++iter)
            {
                updated.data.push_back(fun(*iter, index));
                index++;
            }
            
            updated.is_data_copied = true;
            
            return updated;
        }
        
    public:
        
        /**
         * @brief Inverts the order of the elements in a sequence.
         *
         * @return A sequence whose elements correspond to those of the input sequence in reverse order
         */
        enumerable<TSource> reverse()
        {
            //actually swaps data so actual data is reversed
            //might be better to reverse direction of the iterators
            //though I'm not sure if this is possible
            ensure_data();
            std::reverse(data.begin(), data.end());
            return *this;
        }

        /**
         * @brief Returns a number that represents how many elements in the specified sequence satisfy a condition.
         *
         * @param predicate A function to test each element for a condition.
         * @return size_t equal to the number of elements for which the predicate is true
         */
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
        /**
         * @brief Returns the number of elements in a sequence for
         * a Random_access_iterator container
         *
         * @return number of elements in a sequence
         */
        size_t count() requires Random_access_iterator<TIter>()
        {
            if (is_data_copied) return data.size();
            else return end - begin;
        }

        /**
         * @brief Returns the number of elements in a sequence for
         * a Forward_iterator container
         *
         * @return number of elements in a sequence
         */
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
        /**
         * @brief the maximum value in a sequence of mapped values.
         *
         * @param mapper function to expose the field of interest
         * @return max value element based on the field of interest
         */
        template <typename TFunc, typename TReturn = typename result_of<TFunc(TElement)>::type>
        requires Invokable<TFunc, TElement>() //&& Number<TReturn>()
        TReturn max(TFunc mapper)
        {
            ensure_nonempty();
            if (is_data_copied) return max(mapper, data.cbegin(), data.cend());
            else return max(mapper, begin, end);
        }

        // TODO: This could call the other max override w/ a lambda that returns itself, if we
        // figure out lambda inlining.
        /**
         * @brief returns max value
         *
         * @return max value
         * @requires requires Number<TElement>()
         */
        TElement max() //requires Number<TElement>()
        {
            ensure_nonempty();
            if (is_data_copied) return max(data.cbegin(), data.cend());
            else return max(begin, end);
        }
        
    private:
        
        template <typename TFunc, typename TReturn = typename result_of<TFunc(TElement)>::type, typename TIterator>
        TReturn max(TFunc mapper, TIterator seq_begin, TIterator seq_end)
        {
            TReturn max = numeric_limits<TReturn>::min();
            for (auto iter = seq_begin; iter != seq_end; ++iter)
            {
                TReturn val = mapper(*iter);
                if (val > max) max = val;
            }
            return max;
        }
        
        template <typename TIterator>
        TElement max(TIterator seq_begin, TIterator seq_end)
        {
            TElement max = numeric_limits<TElement>::min();
            for (auto iter = seq_begin; iter != seq_end; ++iter)
            {
                TElement val = *iter;
                if (val > max) max = val;
            }
            return max;
        }
        
    public:

        /**
         * @brief the minimum value in a sequence of mapped values.
         *
         * @param mapper function to expose the field of interest
         * @return min value element based on the field of interest
         */
        template <typename TFunc, typename TReturn = typename result_of<TFunc(TElement)>::type>
        requires Invokable<TFunc, TElement>() //&& Number<TReturn>()
        TReturn min(TFunc mapper)
        {
            ensure_nonempty();
            if (is_data_copied) return min(mapper, data.cbegin(), data.cend());
            else return min(mapper, begin, end);
        }

        /**
         * @brief returns min value
         *
         * @param mapper function to expose the field of interest
         * @return min value element based on the field of interest
         */
        TElement min() //requires Number<TElement>()
        {
            ensure_nonempty();
            if (is_data_copied) return min(data.cbegin(), data.cend());
            else return min(begin, end);
        }
        
    private:
        
        template <typename TFunc, typename TReturn = typename result_of<TFunc(TElement)>::type, typename TIterator>
        TReturn min(TFunc mapper, TIterator seq_begin, TIterator seq_end)
        {
            TReturn min = numeric_limits<TReturn>::max();
            for (auto iter = seq_begin; iter != seq_end; ++iter)
            {
                TReturn val = mapper(*iter);
                if (val < min) min = val;
            }
            return min;
        }
        
        template <typename TIterator>
        TElement min(TIterator seq_begin, TIterator seq_end)
        {
            TElement min = numeric_limits<TElement>::max();
            for (auto iter = seq_begin; iter != seq_end; ++iter)
            {
                TElement val = *iter;
                if (val < min) min = val;
            }
            return min;
        }
        
    public:

        /**
         * @brief computes the sum of a sequence
         *
         * @param mapper function to expose the field of interest
         * @return sum of the values once mapped
         */
        template <typename TFunc, typename TReturn = typename result_of<TFunc(TElement)>::type>
        requires Invokable<TFunc, TElement>() //&& Number<TReturn>()
        TReturn sum(TFunc mapper)
        {
            ensure_nonempty();
            if (is_data_copied) return sum(mapper, data.cbegin(), data.cend());
            else return sum(mapper, begin, end);
        }

        /**
         * @brief computes the sum of a sequence
         *
         * @return sum of the values
         * @requires requires Number<TElement>()
         */
        TElement sum() //requires Number<TElement>()
        {
            ensure_nonempty();
            if (is_data_copied) return sum(data.cbegin(), data.cend());
            else return sum(begin, end);
        }
        
    private:
        
        template <typename TFunc, typename TReturn = typename result_of<TFunc(TElement)>::type, typename TIterator>
        TReturn sum(TFunc mapper, TIterator seq_begin, TIterator seq_end)
        {
            TReturn sum = 0;
            for (auto iter = seq_begin; iter != seq_end; ++iter) sum += mapper(*iter);
            return sum;
        }
        
        template <typename TIterator>
        TElement sum(TIterator seq_begin, TIterator seq_end)
        {
            TElement sum = 0;
            for (auto iter = seq_begin; iter != seq_end; ++iter) sum += *iter;
            return sum;
        }
        
    public:
        
        /**
         * @brief calculates the average of the terms in a sequence
         *
         * @param mapper function to expose the field of interest
         * @return average of fields of interest
         */
        template <typename TFunc, typename TValue = typename result_of<TFunc(TElement)>::type>
        requires Invokable<TFunc, TElement>() //&& Number<TValue>()
        auto average(TFunc mapper)
        {
            ensure_nonempty();
            if (is_data_copied) return average(mapper, data.cbegin(), data.cend());
            else return average(mapper, begin, end);
        }

        /**
         * @brief calculates the average of the terms in a sequence
         *
         * @return average
         */
        auto average() //requires Number<TElement>()
        {
            ensure_nonempty();
            if (is_data_copied) return average(data.cbegin(), data.cend());
            else return average(begin, end);
        }
        
    private:
        
        // We have a ton of overloads here because we want to provide this behavior:
        // - For floating point types, average should return the same type (float for float, double for double).
        // - Otherwise, for integral types, average should return double.
        
        template <typename TFunc, typename TValue = typename result_of<TFunc(TElement)>::type, typename TIterator>
        requires is_integral<TValue>::value
        double average(TFunc mapper, TIterator seq_begin, TIterator seq_end)
        {
            TValue sum = 0;
            size_t count = 0;
            for (auto iter = seq_begin; iter != seq_end; ++iter)
            {
                sum += mapper(*iter);
                count++;
            }
            return sum / (double)count;
        }
        
        template <typename TIterator>
        requires is_integral<TElement>::value
        double average(TIterator seq_begin, TIterator seq_end)
        {
            TElement sum = 0;
            size_t count = 0;
            for (auto iter = seq_begin; iter != seq_end; ++iter)
            {
                sum += *iter;
                count++;
            }
            return sum / (double)count;
        }

        template <typename TFunc, typename TValue = typename result_of<TFunc(TElement)>::type, typename TIterator>
        TElement average(TFunc mapper, TIterator seq_begin, TIterator seq_end)
        {
            TValue sum = 0;
            size_t count = 0;
            for (auto iter = seq_begin; iter != seq_end; ++iter)
            {
                sum += mapper(*iter);
                count++;
            }
            return sum / count;
        }
        
        template <typename TIterator>
        TElement average(TIterator seq_begin, TIterator seq_end)
        {
            TElement sum = 0;
            size_t count = 0;
            for (auto iter = seq_begin; iter != seq_end; ++iter)
            {
                sum += *iter;
                count++;
            }
            return sum / count;
        }
        
    public:

        /**
         * @brief Tests whether the enumerable contains no elements.
         *
         * @return true if the enumerable contains no elements. false otherwise.
         */
        inline bool empty()
        {
           if (is_data_copied) return (data.size() == 0);
           else return (begin == end);
        }

        //TODO: fix all
        /**
         * @brief Determines whether all elements of a sequence satisfy a condition.
         *
         * @param predicate A function to test each element for a condition.
         * @return bool which is true if the predicate is true for all elements
         */
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

        /**
         * @brief Returns a specified number of contiguous elements from the start of a sequence.
         *
         * @param count number of elements
         * @return specified number of contiguous elements
         */
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
        /**
         * @brief Returns a specified number of contiguous elements from the start of a sequence.
         *
         * @param count number of elements
         * @return specified number of contiguous elements
         */
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

        /**
         * @brief Bypasses a specified number of elements in a sequence
         * and then returns the remaining elements.
         *
         * @param count number of elements to bypass
         * @return the sequence of the remaining elements
         */
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

        /**
         * @brief Determines whether a sequence contains a specified element by using the default equality comparer.
         *
         * @param elem element whose presence we are checking for
         * @return bool which is true iff the sequence contains elem
         */
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

        /**
         * @brief Returns the element at a specified index in a sequence, with range checking
         *
         * @param index returns the element at this number
         * @return The element at the specified position in the source sequence.
         */
        const TElement& element_at(size_t index)
        {
            if(is_data_copied)
            {
                return data.at(index);
            }
            else
            {
                auto iter = begin;
                // This loop looks wrong, but the ending iterator should be 1 beyond the last element.
                while (index > 0)
                {
                    if (end != iter) throw out_of_range("index out of range");
                    ++iter;
                    index--;
                }
                return *iter;
            }
        }

        /**
         * @brief Returns the element at a specified index in a sequence, with range checking
         *
         * @param index returns the element at this number
         * @return The element at the specified position in the source sequence.
         */
        const TElement& element_at(int index)
        {
            if (index >= 0) return element_at((size_t)index);
            else throw out_of_range("cinq: element_at() was called with negative index");
        }

        /**
         * @brief Returns the first element of a sequence.
         *
         * @return first element
         */
        const TElement& first()
        {
            return element_at((size_t)0);
        }

        /**
         * @brief Returns the first element in a sequence that satisfies a specified condition.
         *
         * @param  predicate A function to test each element for a condition.
         * @return first element in a sequence that satisfies a specified condition.
         */
        template <typename TFunc>
        requires Predicate<TFunc, TElement>()
        TElement first(TFunc predicate)
        {
            if (is_data_copied)
            {
                for (TElement& i : data)
                {
                    if (predicate(i)) return i;
                }
            }
            else
            {
                for (auto iter = begin; iter != end; ++iter)
                {
                    if (predicate(*iter)) return *iter;
                }
            }
            throw invalid_argument("cinq: no element satisfies the condition in predicate");
        }

        /**
         * @brief Returns the last element of a sequence.
         *
         * @return last element of a sequence
         */
        TElement last()
        {
            if (empty()) throw out_of_range("cinq: cannot get last element of empty enumerable");

            if (is_data_copied) return data[data.size() - 1];
            else
            {
                auto iter = end;
                --iter;
                return *iter;
            }
        }

        /**
         * @brief Returns the last element of a sequence that satisfies a specified condition.
         *
         * @param predicate A function to test each element for a condition.
         * @return last element that satisfies a specified condition
         */
        template <typename TFunc>
        requires Predicate<TFunc,TElement>()
        TElement last(TFunc predicate)
        {
            if (empty()) throw out_of_range("cinq: cannot get last element of empty enumerable");

            if(is_data_copied)
            {
                for (size_t i = data.size() - 1; i >= 0; --i)
                {
                    if(predicate(data[i])) return data[i];
                }
            }
            else
            {
                auto iter = end;
                --iter; //because end points to one past the end
                while (!predicate(*iter) && begin != iter) --iter;

                do
                {
                    --iter;
                    if (predicate(*iter)) return *iter;
                } while (iter != begin);
            }

            throw invalid_argument("cinq: no element satisfies the condition in predicate ");
        }

        /**
         * @brief Returns the only element of a sequence, and throws an exception if there
         * is not exactly one element in the sequence.
         *
         * @return only element in the sequence
         */
        TElement single()
        {
            if (empty()) throw out_of_range("cinq: structure has no elements");

            if (is_data_copied)
            {
                if(data.size() > 1) throw out_of_range("cinq: structure has more than one element");
                else return data[0];
            }
            else
            {
                if(std::distance(begin,end) > 1) throw invalid_argument("cinq: structure has more than one element");
                return *begin;
            }
        }

        /**
         * @brief Returns the only element of a sequence that satisfies a
         * specified condition, and throws an exception if more than one such element exists.
         *
         * @param predicate A function to test each element for a condition.
         * @return only element of a sequence that satisfies a
         * specified condition
         */
        template <typename TFunc>
        requires Predicate<TFunc, TElement>()
        TElement single(TFunc predicate)
        {
            bool found = false;

            if (is_data_copied)
            {
                size_t return_idx;
                for (size_t i = 0; i < data.size(); i++)
                {
                    bool matches = predicate(data[i]);
                    if (found && matches) throw invalid_argument("cinq: more than one element satisfies the predicate");
                    if (matches)
                    {
                        found = true;
                        return_idx = i;
                    }
                }
                return data[return_idx];
            }
            else
            {
                TElement rv;
                for (auto iter = begin; iter != end; ++iter)
                {
                    bool matches = predicate(*iter);
                    if (found && matches) throw invalid_argument("cinq: more than one element satisfies the predicate");
                    if (matches)
                    {
                        found = true;
                        rv = *iter;
                    }
                }
                return rv;
            }
        }

        /**
         * @brief returns vector of data contained in enumerable
         *
         * @return data
         */
        vector<TElement> to_vector()
        {
            ensure_data();
            return data;
        }

        template<typename ... TFunc>
        enumerable<TSource> order_by(TFunc... rest)
        {
            ensure_data();
            std::stable_sort(data.begin(), data.end(), multicmp(rest...));

            return *this;
        }

        enumerable<TSource> order_by()
        {
            ensure_data();
            std::stable_sort(data.begin(), data.end());

            return *this;
        }

    private:

        /**
         * @brief Constructs a comparison function suitable for std::sort from the given mappers
         * @param first The first mapper to use for comparison
         * @param rest The other mappers
         * @return Lambda that compares first by the return value of the first mapper, then the rest.
         */
        template<typename ... TFunc,
                 typename TFirst = typename std::tuple_element<0, std::tuple<TFunc...>>::type,
                 typename TReturn = typename result_of<TFirst(TElement)>::type>
        requires Invokable<TFirst, TElement>() && Ordered<TReturn>()
        auto multicmp(TFirst first, TFunc... rest)
        {
            return [=](const TElement& a, const TElement& b) -> bool
            {
                auto a_map = first(a);
                auto b_map = first(b);
                if (a_map == b_map) return multicmp(rest...)(a, b);
                else return a_map < b_map;
            };
        }

        template<typename TFirst, typename TReturn = typename result_of<TFirst(TElement)>::type>
        requires Invokable<TFirst, TElement>() && Totally_ordered<TReturn>()
        auto multicmp(TFirst first)
        {
            return [=](const TElement& a, const TElement& b) -> bool
            {
                return first(a) < first(b);
            };
        }

        /**
         * @brief begin iterator, only relevent if is_data_copied
         * is false
         *
         */
        TIter begin;

        /**
          * @brief end iterator, only relevent if is_data_copied
          * is false
          *
          */
        TIter end;

        /**
         * @brief stores the original container's
         * data for processing
         */
        vector<TElement> data;
        /**
         * @brief is set to true when our data has been copied.
         * In most cases this will done by ensure_data()
         */

        bool is_data_copied;

        /**
         * @brief this will check to see if the
         * passed in container has been copied to data.
         * this optimization reduces usless copying.
         */
        void ensure_data()
        {
            if (is_data_copied) return;

            data = vector<TElement>(begin, end);
            is_data_copied = true;
        }

        inline void ensure_nonempty()
        {
            if (empty()) throw length_error("cinq: sequence is empty");
        }

    // Allow automated tests to access private stuff.
    friend class test;

    };

    /**
     * @brief Convenience method for constructing enumerable objects.
     *
     * @param source the container passed in by the user for processing
     * @requires requires Container<T>()
     * @return constructs a type enumerable from the container
     */
    template <typename T>
    requires Range<T>()
    auto from(T& source)
    {
        enumerable<T> e(source);
        return e;
    }

}

#endif
