### **AoC (not that one) 2022!!!**

Doing this in C (raw, dirty, C), on Windows 10, as a Visual Studio (2019)
project for the laughs.

Starting 3/12/2022, so we're a bit behind schedule

#### Day 1 

First thoughts; god this would be easier to do in Python.

Ok, problem is simple - read from a file (of unkown size), parse some strings
(of unknown length), do some math with those strings, output the largest
number from that math.

We could _probably_ read the entire file into memory, but my initial instinct
is to read a page at a time, parse the data we have there, flush, repeat until
we hit EOF.

Right, what if the page break happens inside a number? Inside a chain? Bleh

Tangentially - figured out how to FINALLY align VS function breaks the way I
want. [StackOverFlow to the rescue](https://stackoverflow.com/a/60297501/4459746)
Basically you need to go to the ```Tools/Options/Text Editor/C/Formatting/Indentation```
which is completely reasonable.

Honestly this is just teaching me a lot about casting. 

Stuck a bit - hit a wall where I'm not seeing the newlines that separate the "elves" 
This could be an issue with my logic OR how ```atoi``` consumes whitespace. 

Ok, figured it out - just needed to read forward 2 bytes after we found a '\r'
If we saw another '\r', we were in a newline. This caught a bug I wasn't seeing
previously - we were duplicating the value after a newline, because ```atoi``` 
cosumes whitespace BEFORE the value it converts - so we'd see a newline, but it
would be read like an int because there was a valid convertable string waiting
for it. 

#### Day 2

Ok, here we get into the issue of how to organize things. I could add different
VS projects under the main AOC solution and switch between them, OR I could
just blow everything away and use git tags/branches to keep the historical
record of what has been done. Kind of leaning towards the later, though it's
going to make this read me LARGE. I could also make seperate folders for each
day, which is kind of the same thing as the sub projects but git might like
better. 