Project-6 - threads
===============

Shaun Carlson

10576840

##PART A
As expected, the counter value was always 0 upon exit.  The reason is that the main
function ends before any of the worker threads even get to start, which means they
all die horrible deaths before their short lives even really began.  Because the
sleep call comes before count is actually modified by the worker threads, this almost
guarantees the threads will never modify count before the program itself exits,
although it is technically possible.

##PART B
Each time the program was run this time, the value of count was 10.  Instinctively
you expect the value of count to be at 40 since each thread increments count by one
and there are four threads running.  However, each thread grabs a copy of count first
and then goes to sleep for a second.  When it wakes up, then it increments counter 
before going on to the next iteration.  What's happening is that each grabs a copy of
count, which is 0 at the start, and then sleeps.  This means that all threads will grab
a copy of 0 before any thread wakes up and modifies the count.  Then it increments its
copy by one and stores that result back in count.  The value of count is now 1 (with 1
being written into count four different times) and now the process begins again.  Each
thread will grab a copy of count, but because of the position of the sleep statement,
each thread will now grab a copy of 1 before any other thread wakes up and modifies
count.  So after the second iteration for all threads, the value will be 2.  Again, it's
possible for the threads to get out of sync enough such that the final count will not
be 10, but it's very unlikely with only 4 threads and 10 iterations.  Increase the number
of threads or the number of iterations and it becomes more likely.

##PART C

##PART D