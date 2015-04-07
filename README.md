B+ Tree implementation in C++
======================================

To run the program do the following:
`make fresh M=val`
`make`
`./bpt.o queryfile outputfile`

where val is the number of max keys you want to take i a single node.

To update the tree do the following:
`./bpt.o queryfile outputfile`

The details for the tree are stored in bplustree.config file so it should not be tampered with.

The queryfile should contain at least one query of each type as it gives memory error otherwise.
Supported query formats are 0 for insert, 1 for point query, and 2 for range query.

Nodes of B+tree are created in `./data` folder along with the data files.
Naming scheme taken is f\* for file names and d\* for data file names.

High-resolution clock has been used to calculate the time statistics for the query operations. The file access counts and time stats are printed in the end of output file for all types if queries.
