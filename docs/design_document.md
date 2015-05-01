# CINQ: Design Document

- Kevin Chen kxc2103
- Jonathan Barrios jeb2239
- Jonathan Wong jmw2245

## Summary

C++ Integrated Query (CINQ, pronounced "sink") is a C++ implementation of Microsoft's Language Integrated Query (LINQ). We implemented most of LINQ's method syntax, using concepts to ensure that the user passes the correct types. Additionally, our syntax is much cleaner than other C++ LINQ implementations, since we use the `.` operator instead of `>>` to chain methods.

We chose this project because it would allow us to use templates and concepts. These C++ features were introduced in class, but having never used them, we felt that we did not have enough familiarity. Through this project, we have become scarily good at weird templating tricks.

## Feature set

### LINQ methods implemented

- **Select.** Creates a new sequence by mapping the source sequence with a user-supplied mapping lambda.
- **Where.** Filters the sequence to remove items not matched by a user-supplied predicate.
- **Single.** Filters the sequence. If there is only one element left, return the item. Otherwise, throw an exception.
- **Any, All.** Checks whether any or all of the elements match a user-supplied predicate.
- **Min, Max, Sum, Average.** If the source sequence is a `Number`, these methods will compute the min, max, sum, or average. Otherwise, it can compute those values on the results of a user-provided mapping lambda.
- **Take.** Includes only the first _N_ elements. (Convenience method.)
- **Skip.** Includes everything after and including the Nth element. (Convenience method.)
- **ElementAt, First, Last.** Get the element at the specified index.
- **Concat.** Concatenates two sequences.
- **OrderBy.** Sorts the sequence. If a mapping lambda is provided, the sequences will be sorted based on the return value of the lambda. If multiple lambdas are provided, the other lambdas will be used to specify subsequent ordering for the sort.
- **Reverse.** Reverses the order of the sequence.

### Concepts improve template error messages

The nature of our project requires a lot of templating, especially since C++ lambdas each have a unique type. Prior to C++ concepts, this would have meant incomprehensible error messages with type names long enough to fill an entire terminal window.

With concepts, the compiler also tells the user which constraint could not be satisfied, making it easy to see which template argument had the wrong type. In our testing, we found that most of these errors are simple mistakes --- returning an `int` from a lambda instead of a `bool`, for example. Concepts allows the user to spend less time tracking down template problems and more time writing their application.

Prior to this project, it was not easy to get up and running with a C++ concepts toolchain. GCC, Clang, and Microsoft do not have concepts on their feature roadmaps. There are many experimental compiler and standard library implementations, each one implementing a different draft TS. Some compilers did not come with a standard library, and vice versa. We have documented the steps we took to set up a C++ concepts toolchain to save others some trouble.

(Using concepts in our library has also been a great learning experience for us. We were able to apply our knowledge from class, as well as try out a C++ feature that almost nobody else is using --- currently, there are only 55 questions [tagged `c++-concepts` on Stack Overflow](http://stackoverflow.com/questions/tagged/c%2b%2b-concepts).)

### Lots of tests

We have over 60 unit tests covering the correctness and performance of all methods in the library. This allows us to verify that changes to the code continue to return the correct results, and gives us a way to verify that performance fixes actually improved the library's performance.

## Interface design

One might imagine that implementing an existing library in a new programming language would remove the need to think about design. That is not true. The existing library provides a feature list of things that should be implemented, but the interface still needs to be adapted to match the target language's conventions.

### Combining `order_by` and `then_by` to reduce confusion

The C# version of LINQ allows specifying a primary sort order with `OrderBy()` and subsequent orderings with `ThenBy()`. For example:

    myList.OrderBy(Person p => p.Age)
          .ThenBy(p => p.LastName)
          .ThenBy(p => p.FirstName);

However, this syntax makes it possible to write nonsensical queries, such as:

    myList.OrderBy(Person p => p.Age)
          .ThenBy(p => p.LastName)
          .Where(p => p.Age >= 30)
          .ThenBy(p => p.FirstName);

The C# compiler catches these, but unfortunately, outputs a difficult-to-understand error:

> error CS0411: The type arguments for method `System.Linq.Enumerable.ThenBy<TSource,TKey>(this System.Linq.IOrderedEnumerable<TSource>, System.Func<TSource,TKey>)` cannot be inferred from the usage. Try specifying the type arguments explicitly

To eliminate the possibility of the user making this error, we have combined `OrderBy` and `ThenBy` into a single `order_by()` method that takes multiple mapping lambdas for specifying the orderings:

    cinq::from(my_vector)
         .order_by([](person p) { return p.age; },
                   [](person p) { return p.last_name; },
                   [](person p) { return p.last_name; });

Behind the scenes, we recursively construct a comparison lambda that we pass into `std::sort`.

### Using `.` instead of `>>` for method chaining

The "C++ way" of passing the result of one method to another would be overloading `operator>>`. `istream` and `ostream` are implemented this way. However, we know from using `cout` that the angle brackets are annoying to type. They are also confusing to C++ novices who think they are bit shift operators --- especially bad for a data processing library.

## Error handling

In general, returning a magic number to indicate an error is a bad idea. For a data processing library, it makes even less sense --- there is a high risk that the error number is misinterpreted as the result of a computation.

Each CINQ method checks its preconditions to catch possible runtime errors by the user, and throws an exception with a helpful message if the precondition is not met. For example, it does not make sense to `average()` an empty sequence, so the method will throw a `length_error`. Should the user catch the exception, the `enumerable` object's state is still valid and other calls can be made.

## Resource management

Based on what we learned in class, we decided to avoid bare pointers and explicit heap memory allocation (with `new`) wherever possible. The result is that CINQ only allocates heap memory through STL classes such as `std::vector`, and does not use pointers. We were surprised at how easy it was to write error-free C++ code after adopting these rules.

Consider this CINQ call:

    vector<int> result = cinq::from(my_vector)
                              .where([](const int& x) { return x > 0; })
                              .to_vector();

Here is what happens to memory during that call:

1. `from()` constructs the `enumerable` object, which holds information related to the sequence being queried.
2. The `enumerable` object retrieves the iterators of `my_vector`. (This assumes that `my_vector` is not destroyed or modified during the CINQ call.)
3. `from()` returns the `enumerable` instance by value --- no pointers involved. This will use the move constructor on compilers that support it.
4. `where()` creates a new vector to store the result of the filtering. The new vector is assigned to the `data` member of the `enumerable`, which uses the copy constructor.
5. `where()` returns the `enumerable` instance by value.
6. `to_vector()` returns a copy of the `data` vector.
7. The `enumerable` object goes out of scope and its destructor is automatically called.

Our testing strategy includes running the program in [Valgrind](http://valgrind.org) to ensure we didn't miss anything.

CINQ does not open files or listen on the network.

## Optimizations

### Template specializations improve performance in specific cases

A wide variety of containers and iterators can be used as inputs to CINQ. This could be a problem when implementing methods like `count()`:

- Incrementing the iterator until we hit the end works for all containers, but is inefficient for random-access containers like vector.
- On the other hand, not all iterators can get the distance as an O(1) operation.

We overloaded `count()` using concepts. One requires `Random_access_iterator` and the other requires only `Forward_iterator`. If the constraints on the more specific and more efficient specialization cannot be satisfied, the compiler will fall back to the other one. 

### GCC lambda inlining reduces mapper & predicate overhead

Many of our library's functions involve calling small, one-line lambdas, whether they are predicates or mappers. At first, this seems extremely inefficient. However, lambdas are more easily optimized by the compiler than function pointers. When compiling with the `-S` flag to output assembly, we have never observed a situation where the lambdas are not inlined.

### What happened to caching?

In our project proposal, we wrote:

> A good compromise is to use both solutions: use the original vector until a function involving deletion is called. Then, we can silently copy the vector's contents into our lazy deletion vector before continuing to execute the query. We will need to measure this to know whether it is faster.

We implemented something like this in CINQ before mostly throwing it away --- you can still see the remnants in the `ensure_data()` method. The idea was to keep a cache of the data in a vector to avoid accessing the original container, which might be a linked list.

As it turns out, this approach was too heavy-handed. For example, in `where()`, it would cause the method to:

1. Copy all the elements into the cache.
2. Read the items out of the cache one by one.
3. Pass them to the predicate lambda. If it returns true, copy it to the result vector.

In the case of `where()`, step 1 was unnecessary because the items could just be read out of the source vector. Writing the method without `ensure_data()` decreased the average running time from 212 ms to 11 ms (95 percent faster) and putting us on par with writing the same code by hand. This experience showed that knowing the exact needs of each method allows us to optimize better.

## Features for the convenience of users

### Overrides that exist for preventing unsigned to `size_t` conversion errors

The convention for C++ indices is to store them in `size_t`, which is a typedef for an unsigned integer. However, this leads to some nasty problems where the user passes in a signed value that happens to be negative --- the signed value is interpreted as a very large unsigned value:

    cinq::from(my_vector).take(-1);

It will most likely cause the program to crash immediately, but depending on the value, it can also create more subtle errors: an invalid result from CINQ or even memory corruption that causes problems later.

Critically, there will be no compile-time warnings. Neither Clang nor GCC gives warnings for the code above --- and even if they wanted to, they could not prevent all cases: an end user may enter "-1" into the application at run time.

To solve this problem, any CINQ method that takes a `size_t` argument also has an `int` version that throws an exception if the argument is negative, exposing the problem at the source:

    enumerable<TSource> take(int count)
    {
        if (count >= 0) return take((size_t)count);
        else throw invalid_argument("cinq: take() was called with negative count");
    }

### Default versions of methods

Many CINQ methods take mapper lambdas --- for example, `average()` will map the sequence using user-provided lambda, and average the result. But if the user already has a sequence of numbers, it would be very annoying to write:

    cinq::from(class_grades).average([](float grade) { return grade; });

So we provide a default version that does the same thing as the code above:

    cinq::from(class_grades).average();

Thanks to concepts, we can make the no-lambda overload of `average()` available only when used with a sequence of numeric type. This is also a much cleaner and less error-prone than the [C# implementation of average](https://github.com/mono/mono/blob/effa4c0/mcs/class/System.Core/System.Linq/Enumerable.cs#L163), which has a manually templated copy of the method for each numeric primitive and object wrapper.

## Ideas for release 1.2

### Parallel queries

No other C++ LINQ implementation allows the user to parallelize a query automatically. For example, the following code would detect the number of CPUs on your computer (using `std::thread::hardware_concurrency()`) and distribute the filtering so that each core is doing something:

    cinq::from(my_vector).as_parallel().where([](person p) { return some_very_expensive_call(p); });

By making it so easy to parallelize queries, CINQ in practice might even be _faster_ than writing the query out by hand. More programmers will be inclined to parallelize if it's as easy as writing "as_parallel."

### Full implementation of LINQ method syntax

We did not get around to implementing some of the LINQ method syntax, mostly the set operations. It would be good to have a complete implementation, so that we can meet the expectations of programmers already familiar with LINQ.

- select_many
- join
- group_join
- distinct
- except
- intersect
- union
- group_by
- aggregate

### Query Syntax

In our opinion, query syntax is harder to use and less flexible than method syntax. So it's not a high priority. However, it looks cooler --- especially if implemented in a language that is not supposed to have an SQL-like syntax:

    auto result = ( <from> my_contacts
                    <where> [](person &p) { return p.age >= 21 }
                    <order_by> ascending ).to_vector();
