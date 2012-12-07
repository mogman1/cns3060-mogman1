Project-7b - Scheduling Simulation
===============

Shaun Carlson

10576840


##First Come First Served
This was the easiest algorithm to implement and using it would guarantee that all jobs would
be run, specifically in the order they were received.  However, if an extremely long job came
in first followed by several shorter jobs, those jobs would have to wait quite a while until
that first job finished before being able to continue themselves.

##Shortest Job First
This required only a slight modification to the first come first served algorithm to check the
job queue every time a job finished for the next shortest.  This had a noticeably shorter average
wait and turnaround time for each job.  However, if a long job came in the middle, followed by
shorter jobs that kept coming in afterwards, it's possible that this longer job would never
execute because the scheduler would keep grabbing these shorter jobs.

##Shortest Remaining Time Next
This had an even shorter average wait and turnaround time when compared to shortest job first.
However, because this algorithm checks the queue every millisecond for a job with less time left
to complete, it potentially is doing a context switch every millisecond.  Suppose that a job
taking 20ms came in, and then every millisecond after that a new job came in that took 5ms less
than the previous one.  This would result in a context switch every millisecond to grab the next
shortest job.  While this simulation used no context switch penalties, if one did exist then 
this would almost surely be enough to wipe out the modest gains in average wait and turnaround
time it received.  Additionally, this algorithm suffers the same pitfall as shortest job first
in that if a long job comes in first, but then a long line of jobs that take much less time to
complete come in afterwards, that long job could be effectively starved and never given a chance
to complete.

##Round Robin (no context switch penalty)
This had an average wait and turnaround time comparable to shortest job first.  However, in this 
algorithm each job is given a certain amount of time to complete before it is put on hold and
pushed to the back of the line in order to give another job a chance to run.  Because of this,
round robin guarantees that no job will ever be starved indefinitely, although its turnaround time
may suffer due to it being interrupted part-way through its execution.  With a longer time slice,
fewer jobs will be interrupted, and thus the average turnaround time will be lower, but too long
of a time slice and this becomes no different than first come first served.

This algorithm was the most complicated to implement, primarily due to having having context switch
penalties that could be fractions of a millisecond.  Initially I tried to use floating point numbers
to keep track of all this, but ran into floating point arithmetic errors because of that.  I got
what I deserved for that silly mistake.

##Round Robin (with context switch penalty)
This had the worst average wait and turnaround time of all, but it was also the most reflective
of reality for incorporating a context switch penalty.  Had the time slice been 5ms instead of
4ms, more jobs would have been able to complete before being switched out, which would have lowered
turnaround time but at the price of raising wait time.