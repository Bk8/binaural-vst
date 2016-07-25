# BinAural VST

My fork of https://github.com/tmwoz/binaural-vst, changed the triangulation to quick hull method,
this reduces the amount of triangles drastically maybe not the best fix,
but it works and sounds fine.

Removed the dependencies of triangle++, so now the code is completely portable, 
to compile just update the jucer file set the corrects paths of the Steinberg vst SDK,
and juce modules. Now compiles on OSX. 

Also VST binary added to dowload. Dowload the whole project as zip and under OSX build there you will find 
the vst binary.

Original readme:

![](screenshot.png?raw=true)

A mono-to-stereo vst plugin that positions sound in a 3D space using Head-Related Transfer Functions.
Created with [JUCE](http://www.juce.com/) framework.

## Install:
Copy the contents of the 'build' folder to your VST directory.

## License:
GPL (because JUCE is GPL)
