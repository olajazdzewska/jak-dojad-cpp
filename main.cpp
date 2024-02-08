//
//  main.cpp
//  dijkstraC++
//
//  Created by Aleksandra Jażdżewska on 24/04/2023.
//

#include <iostream>
#include "queue.hpp"
using namespace std;

#define inv 10000


int getCitiesGraphIndex(char* city, char** cities, int citiesCount) {
    for (int i = 0; i < citiesCount; i++) {
        if (!strcmp(cities[i], city)) {
            return i;
        }
    }
    return -1;
}

void getMapFromInput(char **map, int h, int w, int &cityCounter, char**cities) {
    int j = 0;
    int i = 0;
    char word;
    int currentCitiesNumber = 1;
    int cityLetterCounter = 0;
    bool waitForCityEnd = false;
    word = getchar();
    while (j < h) {
        word = getchar();
        if (word != '.' && word != '*' && word != '#' && word != '\n') {
            cities[cityCounter][cityLetterCounter] = word;
            waitForCityEnd = true;
            cityLetterCounter++;
        }
        else if ((word == '.' || word == '\n' || word == '*') && waitForCityEnd) {
            waitForCityEnd = false;
            cities[cityCounter][cityLetterCounter] = '\0';
            cityCounter++;
            cityLetterCounter = 0;
        }
        if (word == '*') {
            currentCitiesNumber++;
        }
        if (word == '\n') {
            map[j][i] = word;
            j++;
            i = 0;
        }
        else {
            map[j][i] = word;
            i++;
        }
    }
}

void getCitiesFromMap(char** map, int h, int w, char** cities, int ** citiesCords){
    char currentChar = ' ';
    char nextChar = ' ';
    int currentCitiesNumberOnMap = 1;
    //dirX and dirY are defined that way to look for astrix around the first and last letter of city
    int dirX[] = { 0, 1, 1, 1, 0, -1, -1, -1, 0 };
    int dirY[] = { -1, -1, 0, 1, 1,  1,  0, -1, 0 };
    int iterations = w > 1 ? w : 2; //in case of width = 1 we change it to 2 for the usage of below loop
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < iterations - 1; i++) {
            currentChar = map[j][i];
            nextChar = map[j][i + 1];
            // word starting
            if ((currentChar == '.' || currentChar == '*' || currentChar == '#') && nextChar != '.' && nextChar != '*' && nextChar != '#') { // it means that the name of the city started
                for (int dir = 0; dir < 8; dir++) { //in that case we look for '*' around that word
                    int neighbourX = i + 1 + dirX[dir];
                    int neighbourY = j + dirY[dir];
                    if (neighbourY < 0 || neighbourY >= h || neighbourX < 0 || neighbourX >= w) continue; //avoid the problem of going further then the map
                    if (map[neighbourY][neighbourX] == '*') {
                        map[neighbourY][neighbourX] = char(currentCitiesNumberOnMap);
                        citiesCords[currentCitiesNumberOnMap][0] = neighbourY;
                        citiesCords[currentCitiesNumberOnMap][1] = neighbourX;
                        currentCitiesNumberOnMap++;
                    }
                }
            }
            // word end
            else if ((nextChar == '.' || nextChar == '*' || nextChar == '#' || nextChar == '\n') && currentChar != '.' && currentChar != ' ' && currentChar != '#') {
                for (int dir = 0; dir < 8; dir++) {
                    int neighbourX = i + dirX[dir];
                    int neighbourY = j + dirY[dir];
                    if (neighbourY < 0 || neighbourY >= h || neighbourX < 0 || neighbourX >= w) continue;
                    if (map[neighbourY][neighbourX] == '*') {
                        map[neighbourY][neighbourX] = currentCitiesNumberOnMap;
                        citiesCords[currentCitiesNumberOnMap][0] = neighbourY;
                        citiesCords[currentCitiesNumberOnMap][1] = neighbourX;
                        currentCitiesNumberOnMap++;
                    }
                }
            }

        }
    }
}

void getAirports(int airPorts,char** cities, int citiesCount, int ** citiesGraph) {
    /* get airports from input and update matrix of cities (citiesGraph) with the new values of length between them */
    int airPortsCount = 0;
    char bufforPort[16] = "\0";
    int bufforIndex = 0;
    int startingCityIndex = -1;
    int endingCityIndex = -1;
    int timeCost = -1;
    char word;
    word = getchar(); //collect white sign
    while (airPortsCount < airPorts) {
        do {
            word = getchar();
            bufforPort[bufforIndex] = word;
            bufforIndex++;
            if (word == ' ' && startingCityIndex == -1) { //if we already collected the whole name of the source city
                bufforPort[--bufforIndex] = '\0';
                startingCityIndex = getCitiesGraphIndex(bufforPort, cities, citiesCount); //we look for the index of this city in the matrix of cities
                bufforIndex = 0;
                for (int i = 0; i < 16; i++) bufforPort[i] = '\0'; //clear buffor
            }
            else if (word == ' ' && startingCityIndex != -1 && endingCityIndex == -1) { //the same as above but for the destination city
                bufforPort[--bufforIndex] = '\0';
                endingCityIndex = getCitiesGraphIndex(bufforPort, cities, citiesCount);
                bufforIndex = 0;
                for (int i = 0; i < 16; i++) bufforPort[i] = '\0';
            }
            else if (startingCityIndex != -1 && endingCityIndex != -1 && timeCost == -1 && word == '\n') { //collect path length
                bufforPort[--bufforIndex] = '\0';
                sscanf(bufforPort, "%d", &timeCost);
                bufforIndex = 0;
                for (int i = 0; i < 16; i++) bufforPort[i] = '\0';
            }
        } while (word != '\n'); //when we reach the end of the airplane line
        if (startingCityIndex != -1 && endingCityIndex != -1 && timeCost != -1) {
            if (timeCost < citiesGraph[startingCityIndex][endingCityIndex]) //if the llength of airplane path is shorter than the path on map then update the shorter path in matrix
                citiesGraph[startingCityIndex][endingCityIndex] = timeCost;
            startingCityIndex = -1;
            endingCityIndex = -1;
            timeCost = -1;
        }
        airPortsCount++;
    }
}



void getDestinationPath(int currentNode,int * parents, int src, int end, int citiesIndex, int * visitedCities)
{
    if (currentNode == -1) {
        visitedCities[0] = citiesIndex-1; //set in visitedCities[0] number of visited cities
        return;
    }
    if (currentNode != src && currentNode != end) {
        visitedCities[citiesIndex] = currentNode;
        citiesIndex++;
    }
    getDestinationPath(parents[currentNode], parents, src, end, citiesIndex, visitedCities);
}

int * getVisitedCities(int startNode, int end,  int* distances, int* parents, int cityCounter)
{
    int nVertices = cityCounter;
    int* visitedCities = new int[cityCounter+1];
    for (int i = 0; i < cityCounter; i++)visitedCities[i] = -1;
    for (int indexOfNode = 0; indexOfNode < nVertices; indexOfNode++) {
        if (indexOfNode != startNode && indexOfNode == end) {
            getDestinationPath(indexOfNode, parents, startNode, end, 1, visitedCities);
        }
    }
    return visitedCities;
}

int* dijkstraAlgo(int ** citiesGraph, int source, int end, int cityCounter)
{
    int *ans = new int[cityCounter+2];
    int *shortestDistances = new int[cityCounter];
    bool* visited = new bool[cityCounter];

    for (int indexOfNode = 0; indexOfNode < cityCounter; indexOfNode++) {
        shortestDistances[indexOfNode] = inv;
        visited[indexOfNode] = false;
    }

    shortestDistances[source] = 0;

    int* parents = new int[cityCounter];

    parents[source] = -1;

    for (int i = 1; i < cityCounter; i++) {
        int nearestNode = source;
        int shortestDistance = inv;
        for (int indexOfNode = 0; indexOfNode < cityCounter; indexOfNode++) {
            if (!visited[indexOfNode] && shortestDistances[indexOfNode] < shortestDistance) {
                nearestNode = indexOfNode;
                shortestDistance = shortestDistances[indexOfNode];
            }
        }
        visited[nearestNode] = true;
        for (int indexOfNode = 0; indexOfNode < cityCounter; indexOfNode++) {
            int edgeDistance= citiesGraph[nearestNode][indexOfNode];
            if (edgeDistance > 0 && ((shortestDistance + edgeDistance) < shortestDistances[indexOfNode])) {
                parents[indexOfNode] = nearestNode;
                shortestDistances[indexOfNode]= shortestDistance + edgeDistance;
            }
        }
        
    }
    int* visitedCities;
    visitedCities = getVisitedCities(source, end, shortestDistances, parents, cityCounter); //we get indexes of visited cities
    ans[0] = shortestDistances[end]; //set ans[0] as total path length
    memcpy(&ans[1], visitedCities, sizeof(ans[1]) * cityCounter); //copy visited cities to ans starting from 1 index
    delete[] visitedCities;

    int reverseCounter = 0;
    if (ans[1] > 0) { //reversing visited cities
         do {
             int temp = ans[2 + reverseCounter];
             ans[2 + reverseCounter] = ans[ans[1]+1-reverseCounter];
             ans[ans[1] + 1 - reverseCounter] = temp;
             reverseCounter++;
         } while (reverseCounter < ans[1] / 2);
    }
    delete[] parents;
    delete[] shortestDistances;
    delete[] visited;
   
    
    return ans;
}


void BFSSearch(int sy, int sx, int y, int x,  int width, int height, char** map, int startingCity, int** citiesGraph) {
    /* in bfs we use '#' as our vertexes */
    bool** visited = new bool* [height];
    for (int i = 0; i < height; i++) {
        visited[i] = new bool[width];
        {
            {
                for (int j = 0; j < width; j++) visited[i][j] = false;
            }
        }
    }
    
    
    int** distances = new int* [height]; //matrix of distances that bfs collect
    for (int i = 0; i < height; i++) {
        distances[i] = new int[width];
        {
            {
                for (int j = 0; j < width; j++) distances[i][j] = inv;
            }
        }
    }
    myQueue q;

    myQueue::point startingPoint;
    startingPoint.x = sx;
    startingPoint.y = sy;
    q.enqueue(startingPoint);
    
    visited[sy][sx] = true;
    //looking for '#'
    int dirX[] = { -1, 0, 1, 0 };
    int dirY[] = { 0, 1, 0, -1 };
    int prevDist = 0;
    distances[sy][sx] = 0;
    while (!q.empty())
    {
        myQueue::point cell = q.getHead();
        int y = cell.y;
        int x = cell.x;
        q.dequeue();
        
        prevDist = distances[y][x];
        for (int i = 0; i < 4; i++) {
            int xdir = x + dirX[i];
            int ydir = y + dirY[i];
            if (xdir >= 0 && xdir < width && ydir >= 0 && ydir < height) { //check if the neighbour is in the map
                if (!visited[ydir][xdir] && map[ydir][xdir] != '.'){
                    visited[ydir][xdir] = true;
                    distances[ydir][xdir] = prevDist+1; //counting length of a path
                    
                    myQueue::point pathPoint;
                    pathPoint.x = xdir;
                    pathPoint.y = ydir;
                    q.enqueue(pathPoint);
                    
                    if (citiesGraph[startingCity][map[ydir][xdir] - 1] > distances[ydir][xdir] && (map[ydir][xdir] != '.' && map[ydir][xdir] != '#' && !(sx == xdir && sy == ydir))) { //if we find city number then :
                        citiesGraph[startingCity][map[ydir][xdir] - 1] = distances[ydir][xdir]; //update the matrix od cities with new length of path between the cities
                        
                         q.popBack(); //pop city number out of queue to not check the neighbour
                         
                    }

                    

                }
            }
        }
        
    }
    
    for(int i=0; i<height; i++){
        delete visited[i];
    }
    delete[] visited;
    
    for (int i = 0; i < height; i++){
        delete distances[i];
    }
    delete[] distances;
}

void getDestinations(int cityCounter, int destinations, char** cities, int citiesCount, int** citiesGraph) {
    int destinationsCount = 0;
    char bufforPort[16] = "\0";
    int bufforIndex = 0;
    int sourceCityIndex = -1;
    int finalCityIndex = -1;
    int printCommand = -1;
    int* dijkstraResult = new int(cityCounter + 1);
    char word = getchar();
    while (destinationsCount < destinations) {
        do {
            word = getchar();
            bufforPort[bufforIndex] = word;
            bufforIndex++;
            if (word == ' ' && sourceCityIndex == -1) { // source city
                bufforPort[--bufforIndex] = '\0';
                sourceCityIndex = getCitiesGraphIndex(bufforPort, cities, citiesCount);
                bufforIndex = 0;
                for (int i = 0; i < 16; i++) bufforPort[i] = '\0';
            }
            else if (word == ' ' && sourceCityIndex != -1 && finalCityIndex == -1) { //end city
                bufforPort[--bufforIndex] = '\0';
                finalCityIndex = getCitiesGraphIndex(bufforPort, cities, citiesCount);
                bufforIndex = 0;
                for (int i = 0; i < 16; i++) bufforPort[i] = '\0';
            }
            else if (sourceCityIndex != -1 && finalCityIndex != -1 && printCommand == -1 && word == '\n') { //command
                bufforPort[--bufforIndex] = '\0';
                sscanf(bufforPort, "%d", &printCommand);
                bufforIndex = 0;
                for (int i = 0; i < 16; i++) bufforPort[i] = '\0';
            }
        } while (word != '\n');
        if (sourceCityIndex != -1 && finalCityIndex != -1 && printCommand == 0) { //if 0 command -> cout total length of path
            dijkstraResult = dijkstraAlgo(citiesGraph, sourceCityIndex, finalCityIndex, cityCounter);
            cout << dijkstraResult[0] << endl;
            sourceCityIndex = -1;
            finalCityIndex = -1;
            printCommand = -1;
        }
        else if (sourceCityIndex != -1 && finalCityIndex != -1 && printCommand == 1) { //if command 1 -> cout total path and visited cities
            dijkstraResult = dijkstraAlgo(citiesGraph, sourceCityIndex, finalCityIndex, cityCounter);
            cout << dijkstraResult[0]; //total length
            for (int i = 2; i <= dijkstraResult[1] + 1; i++) { //dijsktraResult[1] = number of visited cities
                cout << " " << cities[dijkstraResult[i]]; //get index of visited citt from dijsktra and then get the name of it from cities, then cout the name
            }
            cout << endl;
            //set default values
            sourceCityIndex = -1;
            finalCityIndex = -1;
            printCommand = -1;
        }

        destinationsCount++;
    }
    delete[] dijkstraResult;
}

void replaceCityNamesWithDots(int h, int w, char ** map) {
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            if (isalpha(map[j][i])) { //is it a letter
                map[j][i] = '.';
            }
        }
    }
    
}


int main() {
    int w, h, airPorts, destinations;
    cin >> w;
    cin >> h;
    int maxSize = h*w;
    int megaSize = w;
    if (h >w) megaSize =h;
    char** map = new char* [h];
    for (int i = 0; i <h; i++) {
        map[i] = new char[w];
    }
    char** cities = new char* [megaSize];
    for (int i = 0; i < megaSize; i++) {
        cities[i] = new char[megaSize];
        for (int j = 0; j < w; j++) cities[i][j] = '\0';
    }
    int cityCounter = 0;
    getMapFromInput(map, h, w, cityCounter, cities);
    int** citiesCords = new int* [cityCounter+1];
    for (int i = 0; i < cityCounter+1; i++) {
        citiesCords[i] = new int[2];
        for (int j = 0; j < 2; j++) {
            citiesCords[i][j] = -1;
        }
    }
    // labeling the cities on map
    getCitiesFromMap(map, h, w, cities, citiesCords);

    // Replacing names with dots
    replaceCityNamesWithDots(h, w, map);

    // cities graph 0-7x0-7 if 8 cities
    int** citiesGraph = new int* [cityCounter + 1];
    for (int i = 0; i < cityCounter ; i++) {
        citiesGraph[i] = new int[cityCounter + 1];
        for (int j = 0; j < cityCounter; j++)
        {
            if (i == j) citiesGraph[i][j] = 0;
            else citiesGraph[i][j] = inv;
        }
    }
    int citiesCount = 1;
    while (citiesCount < cityCounter + 1)
    {
        BFSSearch(citiesCords[citiesCount][0], citiesCords[citiesCount][1], citiesCords[citiesCount][0], citiesCords[citiesCount][1], w, h, map, citiesCount-1, citiesGraph);
        citiesCount++;
    }
    cin >> airPorts;
    getAirports(airPorts, cities, citiesCount, citiesGraph);
    cin >> destinations;
    getDestinations(cityCounter, destinations, cities, citiesCount, citiesGraph);
    
    for (int i = 0; i < megaSize; i++){delete cities[i];}
    delete[] cities;
    for (int i = 0; i < cityCounter+1; i++){delete citiesCords[i];}
    delete[] citiesCords;
    for (int i = 0; i < cityCounter ; i++){delete citiesGraph[i];}
    delete[] citiesGraph;
    for(int i=0; i<h; i++){delete map[i];}
    delete[] map;
    return 0;
}


