Excel TRIMMEAN function:

This function asks to trim the ends (both the lower and upper) of a dataset and calculate
the resulting mean.

The function requires an ordering of the points by value since the upper and lower fraction of the set needed to be removed.

To allow for more input data, I read from an input file of newline separated values
and printed out the average. The code can be run as a command line tool with a help option
and a file name input and percentage.

A first approach is to sort the dataset and calculate the mean ignoring the trimmed off
values at the lower and upper ends of the set.
This takes O(N * log N) on average due to the sorting algorithm.

Instead, my program uses an O(N) solution by basically finding the kth smallest and largest value in the
array, and ignoring values that are below the lower end or higher than the upper end.

To find the kth smallest element in a vector, I partitioned the vector so that all elements less than the pivot
are before the pivot. When reaching that the pivot is at index k, then it stops because we have found the kth
largest element in the vector.

This solution is O(N) because selecting the kth smallest (or largest) element roughly splits the array in half,
and calls the function again on only one half. At each call, partition takes O(size of considered array)
so the call takes around N + N/2 + N/4 + … = 2N = O(N)


Another solution (not implemented) could be to keep track of the k largest (or smallest elements) in
a min heap (or max heap) and for each number seen in file, update this heap. This would take O(N * log K)
where N is the number of elements in the file. This would take less memory for a really large file
but would be slightly slower since finding kth largest element doesn’t take O(N) time.

To run the program:

./trimmean file.txt 0.89 (where 0.89 could be chosen percentage and file.txt is the selected file to read)

./trimmean —-help (for help option)