Project-5 - more1
===============

Shaun Carlson

10576840

It took me a while to understand exactly how to get the reading from stdin
to work properly.  It wasn't clear to me from reading the project description
how to do this at first, but after I implemented the part that reads from an
actual file, it clicked for me what the description was driving at.

NOTE: Input without a newline at the end tends to mess up where the terminal
prompt gets displayed when the program terminates.  I considered forcing a 
newline character at the end of the program, but I felt this might misrepresent
the contents of the file.  Plus, files that *did* have a newline at the
end would then have a blank line between the end of the content and the
terminal prompt.