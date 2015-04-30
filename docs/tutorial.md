#__Cinq Tutorial__

###__Getting Setup__ <sub><sup>These instructions have only been tested on Ubuntu 14.04</sup></sub>
####Step 1: compile and install the GCC 5 concepts branch You need a version of gcc with concepts
support. See [concepts_setup.md](concepts_setup.md) for instructions on how to get this.

####Step 2: Obtain needed libraries.  As of the publishing of this document there are no concepts
defined in the included STL even though the compiler supports the necessary syntax. For this reason
the CINQ library depends on Andrew Sutton's [Origin Libraries](https://github.com/asutton/origin) .
These libraries include many useful concepts which are used in the CINQ implementation. These are
also the concepts that will most likely be included in the STL if concepts are ever included in the
standard. These concepts are included with the download of CINQ. CINQ otherwise relies exclusively
on the STL.

To use CINQ clone the git repository using this command: ```shell $ git clone
https://github.com/jeb2239/Linq4Cpp.git ```
//we need to include details about how people can include this in their normal projects

-----
###__Basic CINQ usage__ Lets take a look at a basic CINQ query:

```cpp 
std::array<int, 8> my_array = { 1, 4, 6, 3, -6, 0, -3, 2 }; 
	auto result = cinq::from(my_array) 
						.where([](int x){ return x > 0;})
						.to_vector(); 
	std::vector<int> answer { 1, 4, 6, 3, 2 }; //the output

``` 

First we construct an `std::array` object. This could be any sequence container such as an
`std::vector` or `std::list`.

First of all notice the initial call to `cinq::from()` this function serves as the entry point of
all CINQ queries. It constructs an `enumerable` object from the passed in sequence container. This
enumerable is then returned and is passed as the implicit argument to `where()`. 

Where is a method that filters a sequence of values based on a input function. That function is
constrained to those only satisfying the `Predicate` concept. For convenience we use a lambda
function which returns a type convertible to bool. This `Predicate` is applied to each item in the
sequence. Only the items for which this predicate evaluates true will be included in the output
sequence. 

`Where` now returns the modified sequence as an enumerable. At this point the user could
chain more queries together to further modify the data set. However, in this example, we are now
done changing our set so we proceed to call `to_vector()` on our `enumerable`, which stores the returned
`std::vector` in `result`.  At the end of this query the original source object passed into from()
is not changed. 

###__CINQ methods__

The CINQ library allows for type safe, generic and arbitrarily complex queries via the chaining of
query methods which operate on an enumerable object. 

####[_select()_]() 

Consider the following example using `select()`: ```cpp vector<int> nums { 1,2,3,4,5 }; auto result
= cinq::from(nums) .select([](int x) { return x * x; }) .to_vector(); vector<int>
answer{1,4,9,16,25}; ``` Lets use a `vector<int>` this time. As before `from()` returns an
enumerable which we can query with `select()` this time. Select transforms every element in the
sequence according to a mapper function which is taken as an argument. This mapper must satisfy the
constraints required by the `Invokeable` concept (something that can be called). This method is an
easy way to apply a certain operation to every object with out changing the original source
container. Select instead returns an `enumerable` which contains the original elements but mapped.

####[_where()_]()

The following shows a different way of using `where`: ```cpp std::list<int> my_list { 1, 30, 2, 5, 4
}; auto result = cinq::from(my_list) .where([](int x, int index) {return index == x;}) .to_vector();
std::vector<int> answer { 2, 4 }; return (result == answer); ```

The overload of `where` in this example still requires a `Predicate` but this time the predicate has
two arguments the first being the object which is contained by the source container and the second
being the index of the item. This means we can use the index of the index of the object when making
our decision on what elements to keep. The example above filters out any elements which are not
equal to their index.  ####[_any()_]()

When `any()` is called with no arguments it will return true or false based on if there are elements
in the container or not for example: ```cpp std::vector<int> my_vector1 {0, 1, 2, 3, 4}; bool
result1 = cinq::from(my_vector1) .any(); std::vector<int> my_vector2{};						bool
result2 =cinq::from(my_vector2) .any(); ``` `result1` is `true` while `result2` would be `false`
	
As you will notice with many CINQ methods, there is often an additional overload which takes in a
predicate to allow for a more specific question. If you wanted to know if the vector contained at
least one value greater than 2 you would simply write: ```cpp std::vector<int> my_vector1 {0, 1, 2,
3, 4}; bool result1 = cinq::from(my_vector1) .any([](int x){return x>2;}); ``` This would of course
evaluate to `true`.

####[_count()_]() Count as its name would suggest, returns the number of elements in an enumerable.
The simplest example would look like: ```cpp std::vector<string>
my_vector{"hi","how","are","you","today"}; int count= from(my_vector).count(); ``` `count` would
equal 5. `Count()` also has an overload which takes a `Predicate` and counts the number of elements
which satisfy that predicate.  An example: ```cpp int count = from(my_vector) .count([](string
x){return x.length()>3;}); ``` `count` equals 1 as the only `string` whose length is greater than 3
is `"today"`

####[_all()_]()

All returns a bool which is true if all the elements in a sequence satisfy a certain condition and
false otherwise.  An example using the same vector from before: ```cpp int r1 = from(my_vector)
.all([](string x){return x.length()>3;}); int r2 = from(my_vector) .all([](string x){return
x.length()<6;}); ``` In the above example `r1` would be false while `r2` would be true because every
string has a length less than 6

We have already learned quite a bit about C-plus-plus integrated query. Lets just practice combining
some of the techniques we have learned so far. 

Imagine I have the data set below: ```cpp vector<string>
strVec{"apple","banana","peaches","melon","pineapple"}; ``` Say I want to only include the fruits
who's last character is an `'e'`.  This is easy to do with CINQ! Just build a simple query.

```cpp cinq::from(strVec) .where([](string x){ return x[x.length()-1]=='e'];); ``` in just a line of
code we have filtered our data set.  What if we wanted only the lengths of the of the words which
ended in `'e'`? We just need to add the extra step of mapping the the enumerable returned from where
to the lengths of the included strings as so: ```cpp cinq::from(strVec) .where([](string x){ return
x[x.length()-1]=='e'];) .select([](string x){return x.length();}); ``` this would give you an array
of the lengths of the strings which end in `'e'`. We will be visiting more complex examples of CINQ
usage later in the tutorial. Lets continue going over our toolkit.

####[_reverse()_]()

Can you guess what this method does? It just reverses the sequence which is passed to it. This is
nothing new but it makes it easy to include this operation  in query chains as it returns an
`enumerable`.

```cpp std::list<int> my_list{ 1,2,3,4,5,6}; auto result=my_list.reverse().to_vector();
//result contains {6,5,4,3,2,1}
```

####[_concat()_]()

Another sequence operation which comes in handy is `concat`. This enables the user to Concatenate an
enumerable to another enumerable. We shall give a basic example below:

```cpp std::list<int> my_list1{1,2,3}; std::list<int> my_list2{4,5,6}; auto
result=my_list1.concat(my_list2).to_vector(); ```

One of the best features of CINQ is its ability to conduct various math operations on lists such as
max, min ,sum and average in a fully generic manner.

####[_min()_]() and [_max()_]()

The `max()` method finds the maximum element in an enumerable. It has the option of taking a mapper
function in order to select the max element based on a certain field with in the object. `min()`
operates in the same manner but finds the minimum element. Consider the `Person` structure: ```cpp
struct Person{ string name; int age; int weight; Person(string n, int a, int w) { name=n; age=a;
weight=w; } } tim("Tim",14,120), rich("Rich",20,150),kevin("Kevin",19,145); ``` what if I have a
list of People and I want to find the oldest or youngest person in the set. Using CINQ we would do:

```cpp std::vector<Person> my_people= {tim,rich,kevin}; Person
result_old=from(my_people).max([](Person x){return x.age;});
//result = rich
Person result_young=from(my_people).min([](Person x){return x.age;});
//result =tim
``` Keep in mind that the return value of these lambdas are values which can be ordered and have the
needed operators to do so. CINQ will not allow the code to compile without this constraint being
satisfied. 

If the object itself has the needed operators for comparison then leaving `max()` with no arguments
will result in the selection of the max element based on the default comparison operators.

####[_sum()_]() You can take the sum over a set of elements or field from those sets, again the
options you have depend on what concepts are satisfied by the object being used for the calculation.
Say I want to know the combined weigh of all the people from the example above.  ```cpp int total=
from(my_people).sum([](Person x){return x.weight;});
//total = 315
``` Unless you specifiy a lambda sum will just sum up the objects un mapper assuming they satisy the
correct constraints.

####[_average()_]() This is similar to sum except that it also divides the sum by the number of
elements in the set. This necesitates that the items of interest satisfy the concepts `Number` and
`is_integral`.  Say I wanted to find the average weight, or age of the people listed above. I would
just do ```cpp auto seq = from(my_people); int avg_age=seq.average([](Person x){return x.age;}); int
avg_weight=seq.average([](Person x){return x.weight;}); ``` as with other math operations average
doesn't need to have an augment if the elements in the sequence satisfy the concept `Number` .

####[_take()_]()

Take takes a single integer $n$ as an argument and returns the first $n$ items of the sequence. Here
is a basic example:

```cpp std::vector<string> my_vector{"hi","how","are","you","today"}; auto
result=from(my_vector).take(2).to_vector();
//returns {"hi","how"}
``` ####[_skip()_]() the functional complement of `take` is `skip`. Like `take`, `skip` takes in  a
single integer $n$ and removes the first $n$ elements from the sequence Here is a basic example
using the same vector as before: ```cpp auto result = from(my_vector).skip(2).to_vector();
//returns {"are","you","today"}
``` ####[_element_at()_]() `element_at()` takes in an integer and returns the element at that index.
```cpp string result = from(my_vector).element_at(3);
//returns "you"
``` ####[_first()_]() and [_last()_]() Takes up to one `Predicate` and returns the first or last
element for which the condition is true. If you pass nothing it will return the first or last
element.  ```cpp string result1 = from(my_vector).first();
				//return "hi"
string result2 = from(my_vector) .first([](string x){return x[0]=='h';});
				//return "hi"
string result3 = from(my_vector) .last([](string x){return x[0]=='h';});
				//returns "how"
string result4 = from(my_vector).last();
				//returns "today"
``` ####[_single()_]() single takes in a `Predicate` and returns the only element of the sequence
that satisfies that condition. If more than one element satisfies the condition an exception is
thrown. If no `Predicate` is passed then single will return the only element in the sequence.
```cpp string result = from(my_vector) .single([](string x){return x[0]=='y';}); ```
####[_order_by()_]() Arguably one of the more powerful features of the CINQ library, `order_by`
takes a variable number of mappers and sorts the elements based on the order in which the mappers
are passed. Its best to demonstrate with an example:

```cpp vector<string> my_vector = {"hello","cello","fellow","cat"}; ``` say we want to sort this
vector of strings in order based on the length of the string. Notice two strings have the same
length. If we want to specify a lower priority rule to further order the strings of matching length
we can do so as follows: ```cpp auto result = from(my_vector).order_by( [](string x){return
x.length();}, [](string x){return x[0];}).to_vector();
// returns {"cat","cello","hello","fellow"};
``` In this case the lower priority rule is based on the comparison of the first letter.


