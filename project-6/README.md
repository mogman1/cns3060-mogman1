Project-6 - threads
===============

Shaun Carlson

10576840

##THOUGHTS
There wasn't a whole lot to the project and since the threads themselves were so
short, nothing particularly interesting ever happened (e.g. messed up counts).
Having a program that did something a little more extreme would be more illustrative.
For example, have several threads (10 or more to be sure that some switching has to
happen) increment counter and then decrement counter by oneand have it do this 10,000
times.  If everything works properly, the final output should be 0.  However, with
improper thread control the output is likely to stray from 0 quite regularly and be
more interesting.

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
With the sleep command commented out, now the final count was reliably 40 in all
executions of the program.  However, this is a little deceiving.  What is most likely
happening is that each thread manages to completely finish before the next thread starts,
since ten iterations will complete very quickly.  Thus, there are no instances where the
threads interfere with each other.  However, it is technically possible for this to happen.
One thread could come in and grab a copy of count, which we'll suppose is at 12, and then
another thread comes in and grabs of count before the first thread does its incrementing.
If this were to happen, both threads would push the value 13 back into count, meaning we
"lost" one incremented value and the final total would be 39.  However, as said, the whole
loop will execute so quickly that each thread probably finishes before the next thread
even starts.

##PART D
Finally, a mutex has been created for this last part.  The mutex is claimed by a thread
just before that thread attempts to fetch count and increment it.  Once the thread has
incremented count, it releases the mutex.  The sleep command has been moved outside of the
mutex calls, that way the thread isn't sleeping while it has control of the mutex.  With
this implementation, the final result of count is reliably 40 and it will always be 40 as
there are no conditions in which one thread can modify count while another thread is in the
middle of doing the same thing.  You could also spin up as many threads or have each thread
run for as many iterations as you like without any fear of inter-thread conflicts.  You
could also remove the sleep command without any risk.
