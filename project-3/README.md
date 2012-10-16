Project-3 - du1
===============

Shaun Carlson

10576840

I solved this problem via a recursive function that calls itself should it come across a file
system entry that happens to be a directory.  It opens the directory and then iterates across
all elements inside that directory, making the aforementioned recursive call with this new
path.  However, in order to be able to print out the full relative path (e.g. ../project-3/du1.c),
I had to dynamically create char arrays and build the path as seen on lines 51-59.  I feel like
there is a better way to do this, but I couldn't think of anything else at the time.