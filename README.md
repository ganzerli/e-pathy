# e-pathy
e-pathy saves integers (also in a file) in a way that you can find them following a path. /0/1/50/0 for example.

e-pathy is a database shaped as a graph, where you can navigate throught paths and save data in branches, or create other branches in branches.

The goal of the project is to have an extremely fast search, and save memory.

There is a garbage collection: when you delete data the locations are written in a file, adding new data, if the count is available, are written in deleted spots.

Compile linux with: gcc -o e-pathy e-pathy.c

How it works:
- compile and start e-pathy.
- compile and start relacy.
- using a browser write the url: http://localhost:8086/

No other server needed, relacy has already a backend-framework integrated.
Write in the form your nodes separated by space, navigate into the nodes, and create other nodes in nodes.
