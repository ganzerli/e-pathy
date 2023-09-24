# e-pathy
e-pathy saves integers (also in a file) in a way that you can find them following a path. /0/1/50/0 for example.

e-pathy is a database shaped as a graph, where you can navigate throught paths and save data in branches, or create other branches in branches.

The goal of the project is to have an extremely fast research, and save memory.

There is a garbage collection, when you delete data, the locations are registered in a file, writeing data when the count is available the new data are written in deleted spots.

Compile linux with: gcc -o e-pathy e-pathy.c
