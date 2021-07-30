# juce_headless_audio
Recently I had to help a colleague convert a JUCE application that had a GUI
and audio interface into one that could run just the audio portion of the
program on a Linux server (headless... no Desktop).

That turned out to be a bit more difficult than I suspected, so I put a simple
example here for anyone that may run into the same problem.  Note that I am
not an experienced JUCE programmer, so the style/method used is probably ugly
relative to JUCE programming standards; but it works.

Also, one thing to note...
To make this independent of my development machine's path to JUCE/modules,
the Makefile requires that the shell variable JUCEMODULES be set to the full
path of the (assumed-to-be) installed JUCE source tree.

One other thing...
If you build and run this, be aware that it is just a simple audio loop back,
(mic-in to spkr-out) so you may want to first lower the output volume.

This has only been tested on Linux.
