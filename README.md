# A Matching Engine
A c++ matching engine with limit order book (LOB)

# Features
* For two major operations in LOB, O(1) to match, O(1) to add if already have price level or O(logM) otherwise. Assume M is the average number of quotes in the LOB 
* No double or float comparison
* Memory efficient 
* Wall clock time is ~19s under mac air Intel(R) Core(TM) i5-3427U CPU @ 1.80GHz

# Install
`$ make`

# Run Test
`$ make test`

# Run the matching engine
Need to modify the path to orders.csv in `run.sh`!

`$ ./run.sh`

# Dependencies Required to Run the Test
boost
