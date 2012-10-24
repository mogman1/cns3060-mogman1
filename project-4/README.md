Project-4 - simulation
===============

Shaun Carlson

10576840

##First Come First Served
This was the drop-dead easiest to implement since all it required was iterating across
the array and computing the difference, resulting in a O(n) running time.  Of course,
this tended to be the least efficient in terms of minimizing the seeks required.

##Shortest Seek Time
This was only slightly more complicated to implement.  To keep track of what tracks have
already been served, I created a second array of flags.  Alternatively you could toggle
the actual values in the array when you're done with them, but this way I could re-use
the same input array without having to copy it.  However, this algorithm  requires that 
you loop through the entire array each time, resulting in O(n^2) running time.

##Elevator
