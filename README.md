# juce_headless_audio
Recently I had to help a colleague convert a JUCE application that had a GUI and audio 
interface into one that could run on a Linux server (headless... no Desktop).
That turned out to be not quite as trivial as I suspected, so I put a simple example
here for anyone that may run into the same problem.
Note that I am not an experience JUCE programmer, so the style/method used here may
be ugly; but it works.

Also, one thing to note...
To make this independent of my development machine's path to JUCE/modules, the Makefile
requires that the shell variable JUCEMODULES be set to the full path of the
(assumed-to-be) installed JUCE source tree.

One other thing...
If you build and run this, be aware that it is just a simple audio loop back, so 
you may want to first lower the output volume.
