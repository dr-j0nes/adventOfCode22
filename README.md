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