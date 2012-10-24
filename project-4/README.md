Project-4 - simulation
===============

Shaun Carlson

10576840

#Algorithms
##First Come First Served
###Algorithm Description
First come first served is just a queue implementation of seeking where the head moves
from the current position to the next position on the queue and so on until all searches
are completed.
###Observations
This was the drop-dead easiest to implement since all it required was iterating across
the array and computing the difference, resulting in a O(n) running time.  Of course,
this tended to be the least efficient in terms of minimizing the seeks required.

##Shortest Seek Time
###Algorithm Description
Shortest seek time scans through the entire array to find the the next track that is the
shortest hop from the current position, then makes that hop.  It continues to do this
scanning until all requested tracks are visited.
###Observations
This was only slightly more complicated to implement.  To keep track of what tracks have
already been served, I created a second array of flags.  Alternatively you could toggle
the actual values in the array when you're done with them, but this way I could re-use
the same input array without having to copy it.  However, this algorithm  requires that 
you loop through the entire array each time, resulting in O(n^2) running time.

##Elevator
###Algorithm Description
Elevator is similar to shortest seek time, except that from its start position it continues
in one direction until it reaches one end of the disk.  Then it reverses directions and
continues in the opposite direction to pick up the remainder of the unvisited tracks.
###Observations
For the most part, elevator isn't that much more difficult to implement than shortest seek
time.  It suffers from the same problems that shortest seek time does in that it's going
to require O(n^2) running time because you keep having to go over the array to find the
shortest seek time going in a certain direction.

For simplicity of implementation, I chose to sort the array prior to going through the 
"meat" of the simulation.  A more "pure" version of the algorithm would simply be a series
of shortest seek times searches done with the constraint that your next track has to be 
higher than your current track until you reach the end.  At that point your next track 
would have to be smaller than your current track until you reached the bottom.

However, sorting the array does not affect the complexity of the algorithm in any 
meaningful way.  The process of sorting itself performs those series of shortest seek time 
searches as described above.
 

#Conclusion
First come first served is trivial to implement, but unless you happen to get lucky and your
tracks roughly come in order, it's going to be extremely inefficient in terms arm movement.

At first glance, the shortest seek time and elevator algorithms appear to behave very
similarly.  For a static array of tracks this is going to be true.  For shortest seek
time, the arm is going to have to move either up or down.  If it moves up for the shortest
seek, it becomes a little more likely that it will move up again for the next seek as it
will have moved away from it's second option of moving down.  Eventually it will just
continue to move up until it hits the end and has to reverse course and go down, just like
the elevator algorithm.  While you could construct a list of tracks for which this would
not be true (e.g. 15 16 13 20 4 99), odds are a static list will behave very similar to
the elevator algorithm.

However, track queues are not static when implemented on a computer.  So long as the
computer is running and doing something, this queue is going to be continually fed new
tracks to look up.  In a situation like this, it becomes much easier for an outlier track
to be "starved" as new track requests come in.  For example, suppose the initial queue was
15 17 14 99 14 and then subsequent tracks kept being requested between the values of 0 and
30.  The 99 track would never be reached in a shortest seek time because these other new
tracks would always be closer.  With the elevator algorithm, this is avoided because once
the arm begins moving toward 99, it won't stop until it reaches it before turning around again. 
 