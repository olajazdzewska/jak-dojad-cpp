# Jak Dojade?

## Description:
This program evaluates the shortest path between source city and destination city. It takes as input
 - dimension of map
 - map consisting of cities (asterisk and names where asterisk is a exact place of a city on map and is always around its name), paths (hashtags) and dots (empty fields) 
 - number of airports (n)
 - next n lines conist of source airport city, destination airport city and time the flight takes
 - mumber of queries (k)
 - next k lines consist of source city, destination city and number (1 means that cities on the way should be printed out, 0 means that they shoudn't be printed out)
 The program gather and intepret map, counts all roads between cities using BFS algorythm, creates map of roads, then chooses the shortest path using Dijkstra algorythm. As output it prints out cities that are on the way and travel time (1 hashtag = 1 unit of time).


## Stack:
 - C++ with iostream (as it was University project and other libraries were forbidden)

## Instruction:
 Go to `main.cpp` file and run it, then insert file example_input.in as input file. 
