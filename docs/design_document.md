# CINQ: Design Document

- Kevin Chen kxc2103
- Jonathan Barrios jeb2239
- Jonathan Wong jmw2245

## Feature set



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
