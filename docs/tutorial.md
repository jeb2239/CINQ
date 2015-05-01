#__CINQ Tutorial__

##__Introduction__
*What is CINQ?*

C++ Integrated Query (CINQ, pronounced "sink") is a play on words on Microsoft's [Language-Integrated Query
(LINQ)](https://msdn.microsoft.com/en-us/library/bb397926.aspx).  LINQ allows both C# and Visual Basic to perform various queries on potentially any data store. It standardizes patterns for querying and updating data. 

CINQ is our C++ implementation of LINQ. It contains most of LINQ's method syntax, utilizing C++'s templating for great robustness while adding C++14's concepts for type security.




##__Getting Setup__ 

Before we jump into the gritty details, we first need to get set up to use CINQ.

<sub><sup>These instructions have only been tested on Ubuntu 14.04</sup></sub>

###Step 1: Compile and install the GCC 5 concepts branch 

You need a version of gcc with concepts support. See [concepts_setup.md](concepts_setup.md) for instructions on how to get this.

###Step 2: Obtain needed libraries  

As of the publishing of this document there are no concepts defined in the included STL even though the compiler supports the necessary syntax. For this reason the CINQ library depends on Andrew Sutton's [Origin Libraries](https://github.com/asutton/origin). These libraries include many useful concepts which are used in the CINQ implementation. These are also the concepts that will most likely be included in the STL if concepts are ever included in the standard. These concepts are included with the download of CINQ. CINQ otherwise relies exclusively on the STL.

To use CINQ clone the git repository using this command: 
```shell 
$ git clone https://github.com/jeb2239/CINQ.git 

```
###Step 3: Include CINQ header files

For convenience CINQ has been implemented entirely in the header file "cinq_enumerables.hpp". All you need to do is make sure your compiler includes the directory src directory and that your C++ file has the includes statement

```cpp
#includes "cinq_enumerables.hpp"

```




##__Basic CINQ usage__ 

Now that we have gotten all set up, let's take a look at a basic CINQ query:

```cpp 
std::array<int, 8> my_array = { 1, 4, 6, 3, -6, 0, -3, 2 }; 
auto result = cinq::from(my_array) 
					.where([](int x){ return x > 0;})
					.to_vector(); 
std::vector<int> answer { 1, 4, 6, 3, 2 }; //the output

``` 

In this example, we construct an `std::array` object. This could be any sequence container such as an
`std::vector` or `std::list`.

First of all, notice the initial call to `cinq::from()` this function serves as the entry point of all CINQ queries. It constructs an `enumerable` object from the passed in sequence container. This enumerable is then returned and is passed as the implicit argument to `where()`.  `where()` is a method that filters a sequence of values based on a input function. That function is constrained to those only satisfying the `Predicate` concept. This `Predicate`is applied to each item in the sequence. Only the items for which this predicate evaluates true will be included in the output sequence. `where()` now returns the modified sequence as an enumerable.

At this point the user could chain more queries together to further modify the data set. However, in this example, we are now done changing our set so we proceed to call `to_vector()` on our `enumerable`, which stores the returned `std::vector` in `result`.  At the end of this query the original source object passed into `from()` is not changed. 

Notice how this example uses a lambda as the input to the `Predicate` parameter of `where()`. Throughout this tutorial, we will be using lambdas for convenience and legibility, but function objects can also be easily used instead. Also, unlike other C++ implementations of LINQ, we use the `.` operator to chain our queries, rather than `>>`. This also makes the code more readable. 


##__CINQ methods__

Now that we have had a taste of CINQ, we can take a broader look at the various tools CINQ provides.

The CINQ library allows for type safe, generic and arbitrarily complex queries via the chaining of query methods which operate on an enumerable object. Most of these methods can be separated by their functionality into a few major categories: Boolean, Filter, Math and Sorting.

###__Boolean__

The Boolean family of methods are the simplest to understand. In general, they test the sequence of values contained in the `enumerable` for whether or not a certain condition is satisfied. This condition is usually  defined by a input predicate.

Consider the following example using `all()` and `empty()`:

```cpp
std::vector<int> my_vector { 1, 2, 3, 4, 5 };
auto my_enum = cinq::from(my_vector);
					
//check if all values in my_enum are greater than 0
if( my_enum.all([](int x) { return x>0; }) )
	cout << "All values are greater than 0.\n";

//check if my_enum is empty
if( my_enum.empty() ) 
	cout << "This is not empty!\n";

```

As before, `from()` returns an enumerable from which we can use `all()` and `empty()` to query. Each of these functions return a boolean value that states whether or not their condition has been satisfied.

This family of methods provides helpful methods that are easily readable and obvious in their meaning.

###__Math__

The Mathematics set of methods adds additional functionality to enumerables that contain numbers. Like the Boolean methods, they are mostly self-explanatory, but are useful when performing calculations on a set of data.

```cpp
std::vector<int> my_ints { 1, 2, 3, 4, 5 };

// sums the values in my_ints
auto sum_results = cinq::from(my_ints).sum();
// returns 15

```

These methods are also able to operate on the values obtained by invoking a mapping function on each element in the input sequence. One of the best features of CINQ is its ability to conduct math operations on sequences in a fully generic manner.

```cpp
vector<string> authors { "kevin chen", "jonathan barrios", "jonathan wong" };

//calculates the average length of the strings in the vector<string> authors
auto result = cinq::from(authors)
					.average([](string x) { return x.length(); });
// returns 13

```

`average()` takes in a function that transforms each string in the authors vector to some useful value, and then calculates the average of these values. In the above example, `average()` converts the strings to their lengths and then finds the average.

###__Filter__

Filtering functions allow us to filter the elements of a sequence such that the remaining elements are the only ones that we want. These functions range from methods such as `element_at()` that retrieves only the value at the input index, to `where()` which uses a predicate and applying it to each element to see if it satisfies the condition.

```cpp
std::array<int, 8> my_array = { 1, 4, 6, 3, -6, 0, -3, 2 }; 
auto my_enum = cinq::from(my_array);

// finds the element at index 2, with the first element being at index 0
auto e = my_enum.element_at(2); 
// returns 6

// finds the elements whose value is greater than 0
auto result = my_enum.where([](int x) { return x > 0; })
                     .to_vector();
// returns { 1, 4, 6, 3, 2 }

```

By using filtering queries, we are able to retain only the elements that we wish to keep. This would speed up further queries on this enumerable since unnecessary elements are removed.

###__Sorting__

Sorting is one of the most useful features of CINQ. Currently, `order_by()` is the only method implemented, but is arguably the most powerful function in the library due to its broad scope of applications. `order_by()` does its sorting by utilizing an input function to determine the output order. Since 'order_by()' is a variadic function, it is able to take in a variable number of mapping functions and sorts the elements based on the order in which the mappers are passed. This is best to demonstrate with an example:

```cpp 
vector<string> my_strings = { "hello", "cello", "fellow", "cat" }; 

``` 

Say we want to sort this vector of strings in order based on the length of the string. Notice both `"hello"` and `"cello"` have the same length. If we want to specify a lower priority rule to further order the strings of matching length we can do so as follows: 

```cpp 
// sorts the strings go from shortest to longest, with conflicts resolved by
// the value of the first character
auto result = cinq::from(my_vector)
				    .order_by( [](string x) { return x.length(); }, 
							   [](string x) { return x[0]; } )
					.to_vector();
// returns { "cat", "cello", "hello", "fellow" }

``` 

In this case the lower priority rule is based on the comparison of the first letter.

You may have noticed in the above example, the return strings go from shortest to longest, with the character 'c' in "cello" being taken over 'h' in "hello". This is because `order_by()` sorts from lowest value to highest as returned by its input mapping functions.

Since `order_by()` is able to use any number of user-defined mapping function, it is essentially your go to function to perform any sorting on your data set. 


###__Miscellaneous__

Though most methods have functionalities that fit into at least one of the above categories, there are a few methods that do not exactly belong in one. The most common query in this group would likely be `select()`.

```cpp 
vector<int> my_ints { 1, 2, 3, 4, 5 }; 

// projects the values in my_ints into a new form
auto result = cinq::from(my_ints)
					.select([](int x) { return x * x; }) 
					.to_vector(); 
// returns { 1, 4, 9, 16, 25 }

``` 

`select()` transforms every element in the sequence according to a mapper function which is taken as an argument. This mapper must satisfy the constraints required by the `Invokeable` concept (something that can be called). This method is an easy way to apply a certain operation to every object with out changing the original source container. Select instead returns an `enumerable` which contains the original elements but mapped. These mapped elements need not even be the same type as they were originally.

```cpp
// appends each integer to a set string, returning each element as a string
// rather than an integer
auto result = cinq::from(my_ints)
					.select( [](int x) { string s {"s"}; return s + std::to_string(x); })
					.to_vector();
// returns { "s0", "s1", "s2", "s3", "s4" }

```

Though not part of the actual query `to_vector()` is an essential part of any query that seeks to output its data to some useable form. As the name states, `to_vector()` takes the enumerable and converts it to a vector containing the appropriate type. 

###__List of implmented methods__			

Now that we have seen some of the power of CINQ, it might be time to have a quick overview of all the tools at our disposal.

Below is a list of all currently implemented methods:

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


###__Overloaded functions__

As described in the list of implemented methods, some of the functions have been overloaded to provided additional functionality in certain cases. This added functionality is made possible by allowing those methods access to either a predicate, mapping function, or element index.

####__1. Predicate__

Predicates return a value that is convertible to bool. As such they allow the function to test the elements for a condition so that they can return only those elements, or determine if those elements exist.

####__2. Mapping function__

The mapping functions allow the data contained by the enumerable to be changed into another form. This allows non-numerical data be transformed to useable values that can be analyzed using some of the Mathematics functions. 

####__3. Element index__

By having access to the element index, it is possible to perform actions or comparisons only on the desired indices.




 


##__Putting it all together__

The examples in the previous section seem to be fairly trivial, and you may be wondering what is the benefit of using CINQ over just implementing these seemingly simple functions yourself and applying them to the actual container.

One of the main benefits is that CINQ functions are able to be applied one after the other to form a much more complicated query.

Consider the `Person` structure containing information on a person's name, age, and weight:

```cpp
struct Person{ 
	string name; 
	int age; 
	int weight; 
	int id;
	Person(string n, int a, int w, int i) 
	{ 
		name=n; 
		age=a;
		weight=w; 
		id=i;
	} 
} tim("Tim",14,120,2), rich("Rich",20,145,5), kevin("Kevin",19,150,6),
emily("Emily",30,110,4), cassy("Cassy",21,125,1), laura("Laura",18,105,3); 

```

One such chain of queries consists of determining the number of people whose index is equal to their id after sorting from least to greatest weight. 

```cpp
vector<Person> my_people { tim, rich, kevin, emily, cassy, laura };

// By applying each of these commands in succession, we are able to form a much
// more complicated query.
auto result = cinq::from(my_people)
					.order_by([](const Person& p) { return p.weight; }
					.where([](Person p, int index) { return index == id; }
					.count();
//returns 2

```
As you can see, we are using an overloaded version of `where()` in this instance.  `where()` still requires a `Predicate` but this time the predicate has two arguments. The first is the object which is contained by the source container and the second is the index of the item. This means we can use the index of the object when making our decision on what elements to keep. The example above filters out any elements whose id's are not equal to their index. This concept of accessing element index is utilized in other overloaded methods. 

Let us consider another, more involved query. Using the same `Person` structure, we want to find the weights of the 3 youngest people, with sorting conflicts being determined by name length, who are 18 or older, However, we only want to apply this query if there there is at least one person whose age is an even number. This is a somewhat complicated query. If written manually, this query would require several lines of additional code, increasing the chance of introducing bugs into the code. 

```cpp
vector<Person> my_people { tim, rich, kevin, emily, cassy, laura };

auto my_enum = cinq::from(my_people);
if( my_enum.any([](Person p) { return (p.age%2) == 0; }) ) 
{
	auto result = my_enum.where([](Person p) { return p.age>=18; })
						 .order_by([](const Person& p) { return p.age; },
						 		   [](const Person& p) { return p.name.length(); }) 
						 .take(3)
						 .select([](Person p) { return p.weight; })
						 .to_vector();
	//returns { 105, 150, 145 }
}

```

Rather than having a large block of handwritten code, the query is reduced to 7 easily understood CINQ method calls.

Just like the first example in which `where()` was overloaded to access an index, this block of code gives us an example of how a `Predicate` is used to give Boolean functions the ability to test for additional conditions. In this case, `any()`, rather than test for whether or not the enumerable contains an element, checks whether or not a `Person` has an even numbered age.



