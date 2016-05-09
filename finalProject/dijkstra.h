#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <limits>
#include <queue>
#include <utility>
#include <algorithm>
#include <iterator>

using namespace std;

typedef int vertex;
typedef double theWeight;

const theWeight maxWeight = numeric_limits<double>::infinity();
struct neighbor {
    vertex target;
    theWeight weight;
    neighbor(vertex where, theWeight weight1) : target(where), weight(weight1){}
};
typedef vector<vector<neighbor> > mygraph;
typedef pair<theWeight, vertex> vertexQueue;

void DijkstraComputePaths(vertex source, const mygraph &adjacencyList, vector<theWeight> &minDistance, vector<vertex> &previous){
    unsigned long n = adjacencyList.size();
    minDistance.resize(n, maxWeight);
    minDistance[source] = 0;
    previous.resize(n, -1);
    priority_queue<vertexQueue, vector<vertexQueue>, greater<vertexQueue> > vertexQueue;
    vertexQueue.push(make_pair(minDistance[source], source));
    while (!vertexQueue.empty()){
        theWeight dist = vertexQueue.top().first;
        vertex u = vertexQueue.top().second;
        vertexQueue.pop();
        if (dist > minDistance[u])
            continue;
        vector<neighbor> neighbors = adjacencyList[u];
        for (vector<neighbor>::const_iterator iter = neighbors.begin(); iter != neighbors.end(); iter++){
            vertex v = iter->target;
            theWeight weight = iter->weight;
            theWeight distanceToU = dist + weight;
            if (distanceToU < minDistance[v]){
                minDistance[v] = distanceToU;
                previous[v] = u;
                vertexQueue.push(make_pair(minDistance[v], v));
            }
        }
    }
}

list<vertex> DijkstraGetShortestPathTo(vertex vertexA, const vector<vertex> &previous){
    list<vertex> path;
    for (; vertexA != -1; vertexA = previous[vertexA]){
        path.push_front(vertexA);
    }
    return path;
}

#endif // DIJKSTRA_H
