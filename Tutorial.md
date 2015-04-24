#__Cinq Tutorial__

###__Getting Setup__
<sub><sup>These instructions have only been tested on Ubuntu 14.04</sup></sub>
####Step 1: compile and install the GCC 5 concepts branch
Because we are using a development branch of gcc we will need to compile our own gcc toolchain which supports concepts syntax such as the `concept` and `requires` keywords. Obtain the branch you will need to have Subversion installed on you machine.
Go to your home directory and enter the following commands.
```shell
$ svn checkout svn://gcc.gnu.org/svn/gcc/branches/c++-concepts
```
You will now have to configure GCC by running, adjusting the paths accordingly:
```shell
$ ../configure --disable-checking --enable-languages=c,c++   --enable-multiarch --enable-shared --enable-threads=posix   --program-suffix=concepts-latest  --without-included-gettext  --with-tune=generic   --prefix=$HOME/install/gcc-concepts-latest --disable-multilib
```
This will produce a minimal install of GCC because we don't need support for Ada, Fortran etc..

Other than the modifications provided above this will be like a normal GCC install for which detailed instructions are available [here](https://gcc.gnu.org/install/). 

Lastly based on the where you installed gcc you will need to go to the directory where the binaries are located and rename them as so:
```shell
$ for i in *; do   mv "$i" "`echo $i | sed 's/concepts-latest//'`"; done
```
This will rename all of the executables to the correct names ( gcc, g++ ,etc..).

Lastly you will need to edit your `.bashrc`   file inorder to ensure the correct libraries are used. For example something like:

```shell
export PATH="/home/barriosj/install/gcc-concepts-latest/bin:$PATH"
export LD_LIBRARY_PATH="$HOME/install/gcc-concepts-latest/lib64/"
```
should be appended to your `.bashrc`


You also will have the option of downloading a virtual machine with all of this installed.

####Step 2: Obtain needed libraries.
As of the the publishing of this document there are no concepts defined in the the included STL even though the compiler supports the necessary syntax. For this reason the CINQ library depends on Andrew Sutton's [Origin Libraries](https://github.com/asutton/origin) . These libraries include many useful concepts which are used in the CINQ implementation, these also are the concepts that will most likely be included in the STL if concepts ever are included in the standard. CINQ otherwise relies exclusively on the STL. These are included with the download of CINQ. To use CINQ clone the git repository using this command:
```shell
$ git clone https://github.com/jeb2239/Linq4Cpp.git
```
//we need to include details about how people can include this in their normal projects

-----
###__Basic CINQ usage__
Lets take a look at a basic CINQ query:

```c++
std::array<int, 8> my_array = { 1, 4, 6, 3, -6, 0, -3, 2 };
        auto result = cinq::from(my_array)
                      .where([](int x){ return x > 0;})
                      .to_vector();
        std::vector<int> answer { 1, 4, 6, 3, 2 }; //the output
        
``` 
First we construct an `std::array` object. This could be any sequence container such as an `std::vector` or `std::list`.

First of all notice the initial call to `cinq::from()` this function serves as the entry point of all CINQ queries. It constructs an `enumerable` object from the passed in sequence container. This enumerable is then returned and is passed as the implicit argument to `where()`. Where constrains the input function to those only satisfying the `Predicate` concept. For convenience we use a lambda function which returns a type convertible to bool. This `Predicate` is applied to each item in the sequence, only the items for which this predicate evaluates true will be included in the output sequence. `Where` now returns the modified sequence as an enumerable. At this point the user could chain more queries together to further modify the data set. We are now done changing our set so we proceed to call `to_vector()` on our `enumerable` and store the returned `std::vector` in `result`.  At the end of this query the original source object passed into from() is not changed. 

###__CINQ methods__

The CINQ library allows for type safe, generic and arbitrarily complex queries via the chaining of query methods which operate on an enumerable object. 

####[_select()_]() 

Consider the following example using `select()`:
```c++
vector<int> nums { 1,2,3,4,5 };
auto result = cinq::from(nums)
.select([](int x) { return x * x; })
.to_vector();
vector<int> answer{1,4,9,16,25};
```
Lets use a `vector<int>` this time. As before `from()` returns an enumerable which we can query with `select()` this time. Select transforms every element in the sequence according to a mapper function which is taken as an argument. This mapper must satisfy the constraints required by the `Invokeable` concept (something that can be called). This method is an easy way to apply a certain operation to every object with out changing the original source container. Select instead returns an `enumerable` which contains the original elements but mapped.

####[_where()_]()

The following shows a different way of using `where`:
```c++
std::list<int> my_list { 1, 30, 2, 5, 4 };
auto result = cinq::from(my_list)
        .where([](int x, int index) {return index == x;})
        .to_vector();
        std::vector<int> answer { 2, 4 };
        return (result == answer);
```

The overload of `where` in this example still requires a `Predicate` but this
time the predicate has two arguments the first being the object which is contained
by the source container and the second being the index of the item. This means we can use the index of the index of the object when making our decision on what elements to keep. The example above filters out any elements which are not equal to their index.
####[_any()_]()

When `any()` is called with no arguments it will return true or false based on if there are elements in the container or not for example:
```c++
std::vector<int> my_vector1 {0, 1, 2, 3, 4};
        bool result1 = cinq::from(my_vector1)
						.any();
std::vector<int> my_vector2{};						
		bool result2 =cinq::from(my_vector2)
						.any();
```
`result1` is `true` while result2 would be `false`
	
As you will notice with many CINQ methods, there is often an additional overload which takes in a predicate to allow for a more specific question. If you wanted to know if the vector contained at least one value greater than 2 you would simply write:
```c++
std::vector<int> my_vector1 {0, 1, 2, 3, 4};
        bool result1 = cinq::from(my_vector1)
						 .any([](int x){return x>2;});
```
This would of course evaluate to `true`.

####[_count()_]()



