There are two projects in this zip file
One Seq is the sequential C++ program 
The other is Cilk.
I was unable to get cilk to compile at all even simple programs as such no test where been able to be conducted on the cilk code. 

To run pass the location of a text file containing the puzzel. Format of the puzzel is expected as follow.

First line is the dimensions (rows and columns)
The next line is the location of all cells that will have a solution. Treat the location as a 1D array, for example
if the cell [2][5] is a solution cell then on the second line there whould be an int 10.

Each line after has 5 ints:
The first int is simmullar to the ints in row 2, which shows the location of the cell. The next is the two ints are for the dow value
<sum,size> the first is the sum going down and the next is how far is the sum allowed to go. The next two numbers are for the right
run and are the same as the sum going down.

Each row to the end of the file represents a cell that contains sum values. Example could be:

8 6
0 3 18 24 29
1 17 2 0 0
2 28 7 0 0
4 42 7 0 0
5 22 3 0 0
6 0 0 9 2
9 31 5 14 2
12 0 0 20 5
19 0 0 30 4
25 22 3 24 3
30 0 0 25 4
35 11 2 0 0
36 0 0 20 5
42 0 0 14 2
45 0 0 17 2
