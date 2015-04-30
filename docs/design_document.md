# CINQ: Design Document

- Kevin Chen kxc2103
- Jonathan Barrios jeb2239
- Jonathan Wong jmw2245

## Summary

C++ Integrated Query (CINQ, pronounced "sink") is a C++ implementation of Microsoft's Language Integrated Query (LINQ). We implemented most of LINQ's method syntax, using concepts to ensure that the user passes the correct types. Additionally, our syntax is much cleaner than other C++ LINQ implementations, since we use the `.` operator instead of `>>` to chain methods.

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

### Lots of tests

We have over 60 unit tests covering the correctness and performance of all methods in the library. This allows us to verify that changes to the code continue to return the correct results, and gives us a way to verify that performance fixes actually improved the library's performance.

## Interface design

combining orderby and thenby to reduce confusion

## Error handling

throw exceptions - returning magic numbers doesnt make sense for data processing lib

## Resource management

based on what we learned from class, avoid using "new"

our library's interface is designed so that enumerable objects will be destroyed going out of scope

## Optimizations

determining when we should cache in "data"

certain operations can work without the cache.

count overloads for linked list vs vector iterators, etc

take advantage of inlining

## Features for the convenience of users

using concepts improves the compiler errors

overrides that exist only for error checking size_t values

## Ideas for release 1.2

take from github issues
