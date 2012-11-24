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

##PART C

##PART D