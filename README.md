![Banner](https://raw.githubusercontent.com/zivkovicmilos/TSP/master/img/BannerImg.jpg)

# Project Description
The goal of this homework project was to solve the Traveling Salesman problem using the following methods:
1. The Nearest Neighbor Heuristic for a large number of cities
2. The Brute Force method for a maximum of 10 cities

# Features
The following menu is used for interacting with the program.

![Interactive Menu](./img/menu.svg)

# Program Breakdown
## Import DB
Imports the Database into an array, and forms a distance matrix based on the selected file. The sample databases are provided with the project in the form of (distCountry.csv). The provided databases are:

1. Serbia (316 cities)
2. Ireland (332 cities)
3. Finland (416 cities)
4. Germany (1000 cities)

The databases were downloaded from https://www.mileage-charts.com/ so any other country/continent will also work.
## Print DB
Prints out the list of cities from the imported database, as well as their unique ID.
## Find Nearest City
Waits for user input, then finds the nearest city based on the input.
## Form Tour (Nearest Neighbor)
Forms a linked list tour using the Nearest Neighbor heuristic, in the following way:
1. User chooses a starting city (with the city ID). This doesn't matter in retrospect for the heuristic, but is useful if the user wants to choose a starting point
2. A list of cities A is generated (in the beginning, it only contains the starting city), and a list of remaining cities B is generated
3. The nearest city to the last added city is added to A, and removed from B
4. Repeat steps 2-3 until all cities are traversed (B list is empty). At the end, the starting city is added, because the traveling salesman needs to return to his starting city after visiting all the other ones

Of course, this heuristic doesn't produce the truly optimal result, as opposed to the Brute Force method.
## Form Tour (All Permutations)
A recursive method to generate all tours between a finite number of cities (max 10), and pick out the best one. This method surely produces the shortest path for the traveling salesman.
The user specifies an *n* number of cities he wants the salesman to travel to. After that, the program chooses *n* random cities and starts the Brute Force algorithm (*bruteCalc*).
When the function finishes, the brute solution tour is printed.
## Print Tour
Prints out the Nearest Neighbor tour.
## Delete Tour
Deletes a tour the user specified. 
Previous tours are automatically deleted when the user chooses the option to generate a new Nearest Neighbor or Brute Method tour.
