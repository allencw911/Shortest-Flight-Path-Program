#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <list>
#include <queue>
#include <algorithm>
#include <map>
#include <locale>
#include "dijkstra.h"
using namespace std;

struct edge{
    string source;
    string airport;
    string carrier;
    string carrierName;
    pair<double, double> location;
    double distance;
    void print(){
        cout<<"\t"<<"Source: "<<source<<endl;
        cout<<"\t"<<"Destination Airport: "<<airport<<endl;
        cout<<"\t"<<"Carrier: "<<carrier<<endl;
        cout<<"\t"<<"Carrier Name: "<<carrierName<<endl;
        cout<<"\t"<<"Latitude: "<<location.first<<endl;
        cout<<"\t"<<"Longitude: "<<location.second<<endl;
        cout<<"\t"<<"Distance: "<<distance<<endl;
        cout<<endl;
    }
};

struct node{
    string name;
    string iata;
    string city;
    string country;
    pair<double, double> location;
    vector<edge> edges;
    void print(){
        cout<<"Name: "<<name<<endl;
        cout<<"Iata: "<<iata<<endl;
        cout<<"City: "<<city<<endl;
        cout<<"Latitude: "<<location.first<<endl;
        cout<<"Longitude: "<<location.second<<endl;
        cout<<"Number of Edges: "<<edges.size()<<endl;
        for (int i = 0; i < edges.size(); i ++){
            edges[i].print();
        }
        cout<<endl;
    }
};

class program{
public:
    program();
    void go();
private:
    void parseXML(vector<node>& a);
    void getDistances(vector<node>& nodes);
    void makeGraph(mygraph& adjacencyList, vector<node> nodes, map<string, int> nameMap);
    void getPath(mygraph adjacencyList, map<int, string> indexMap, int initial, int destination, vector<node> nodes);
    string findCity(vector<node> nodes, string name);
};

program::program(){}
void program::go(){
    locale loc;
    string initial, destination;
    vector<node> nodes;
    mygraph adjacencyList;
    parseXML(nodes);
    getDistances(nodes);
    
    map<string, int> nameMap;
    map<int, string> indexMap;
    for (int i = 0; i < nodes.size(); i++){
        indexMap[i] = nodes[i].iata;
        nameMap[nodes[i].iata] = i;
    }
    
    makeGraph(adjacencyList, nodes, nameMap);
    while (1){
        cout<<"TYPE 'QUIT' TO QUIT"<<endl;
        cout<<"Please give initial airport: ";
        cin>>initial;
        if (initial == "QUIT")
            break;
        cout<<"Please give destination: ";
        cin>>destination;
        for (int i = 0; i < initial.length(); i++){
            initial[i] = toupper(initial[i], loc);
            destination[i] = toupper(destination[i], loc);
        }
        if ( nameMap[initial] == 0 && initial != "GKA" ){
            cout<<"Invalid Input"<<endl;
        } else if (nameMap[destination] == 0 && destination != "GKA"){
            cout<<"Invalid Input"<<endl;
        }else{
            cout<<endl;
            getPath(adjacencyList, indexMap, nameMap[initial], nameMap[destination], nodes);
        }
    }
    
}

void program::getPath(mygraph adjacencyList, map<int, string> indexMap, int initial, int destination, vector<node> nodes){
    vector<theWeight> minDistance;
    vector<vertex> previous;
    vector<string> getPathForAirlines;
    string cityA = findCity(nodes, indexMap[initial]);
    string cityB = findCity(nodes, indexMap[destination]);
    DijkstraComputePaths(initial, adjacencyList, minDistance, previous);
    cout<<"Distance from "<<cityA<<" ("<<indexMap[initial]<<")"<<" to "<<cityB<<" ("<<indexMap[destination]<<"): "<<minDistance[destination]<<" km"<<endl;
    list<vertex> path = DijkstraGetShortestPathTo(destination, previous);
    cout<<"Path: "<<endl;
    list<vertex>::const_iterator iterator;
    for (iterator = path.begin(); iterator != path.end(); ++iterator) {
        getPathForAirlines.push_back(indexMap[*iterator]);
    }
    double distanceA = 0.0;
    for (int i = 0; i < getPathForAirlines.size() - 1; i ++){
        cityA = findCity(nodes, getPathForAirlines[i]);
        cityB = findCity(nodes, getPathForAirlines[i+1]);
        cout<<"From "<<cityA<<" ("<<getPathForAirlines[i]<<")"<<" to "<<cityB<<" ("<<getPathForAirlines[i+1]<<"): "<<endl;;
        for (int j = 0; j < nodes.size(); j++){
            if (getPathForAirlines[i] == nodes[j].iata){
                cout<<"Choose from: "<<endl;
                for (int k = 0; k < nodes[j].edges.size(); k++){
                    if (getPathForAirlines[i+1] == nodes[j].edges[k].airport){
                        cout<<nodes[j].edges[k].carrierName<<"   ";
                        distanceA = nodes[j].edges[k].distance;
                    }
                }
                cout<<endl;
                cout<<"For "<<distanceA<<" km."<<endl;
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

string program::findCity(vector<node> nodes, string name){
    string city;
    for (int i = 0; i < nodes.size(); i ++){
        if (nodes[i].iata == name){
            city = nodes[i].city;
        }
    }
    return city;
}

void program::makeGraph(mygraph& adjacencyList, vector<node> nodes, map<string, int> nameMap){
    vector<neighbor> neighbors;
    for (int i = 0; i < nodes.size(); i ++){
        for (int j = 0; j < nodes[i].edges.size(); j++){
            string destAirport = nodes[i].edges[j].airport;
            int destIndex = nameMap[destAirport];
            double distance = nodes[i].edges[j].distance;
            neighbors.push_back(neighbor(destIndex, distance));
        }
        adjacencyList.push_back(neighbors);
        neighbors.clear();
    }
}

void program::getDistances(vector<node>& nodes){
    double pi = 3.14159265359;
    double totaldistance;
    double latitude1, latitude2, longitude1, longitude2, latDifference, longDifference;
    double a, c;
    double R = 6371;
    for (int i = 0; i < nodes.size(); i ++){
        latitude1 = nodes[i].location.first;
        latitude1 = (latitude1 * pi) / 180;
        longitude1 = nodes[i].location.second;
        longitude1 = (longitude1 * pi) / 180;
        
        for (int j = 0; j < nodes[i].edges.size(); j++){
            latitude2 = nodes[i].edges[j].location.first;
            latitude2 = (latitude2 * pi) / 180;
            longitude2 = nodes[i].edges[j].location.second;
            longitude2 = (longitude2 * pi) / 180;
            latDifference = latitude1 - latitude2;
            longDifference = longitude1 - longitude2;
            a = sin(abs(latDifference)/2) * sin(abs(latDifference)/2)
            + cos(latitude1) * cos(latitude2)
            * sin(abs(longDifference)/2) * sin(abs(longDifference)/2);
            c = 2 * atan2(sqrt(a), sqrt(1-a));
            totaldistance = R * c;
            nodes[i].edges[j].distance = totaldistance;
        }
    }
}

void program::parseXML(vector<node>& a){
    ifstream in("vertices.xml");
    string line;
    node newNode;
    vector<edge> edgess;
    edge edge;
    unsigned long start, end;
    double latitude, longitude, edgeLat, edgeLong;
    while (getline(in, line)){
        if (line.find("<name>") != -1){
            start = 1;
            end = line.substr(start + 1).find('<') - 5;
            newNode.name = line.substr(7, end);
        }
        if (line.find("<iata>") != -1){
            start = 1;
            end = line.substr(start +1).find('<') - 5;
            newNode.iata = line.substr(7, end);
        }
        if (line.find("<city>") != -1){
            start = 1;
            end = line.substr(start +1).find('<') - 5;
            newNode.city = line.substr(7, end);
        }
        if (line.find("<latitude>") != -1){
            start = 1;
            end = line.substr(start + 1).find('<') - 9;
            latitude = stod(line.substr(11, end));
        }
        if (line.find("<longitude>") != -1){
            start = 1;
            end = line.substr(start + 1).find('<') - 10;
            longitude = stod(line.substr(12, end));
        }
        newNode.location = make_pair(latitude, longitude);
        if (line.find("<airport>") != -1){
            edge.source = newNode.iata;
            edge.airport = line.substr(12, 3);
        }
        if (line.find("<carrier>") != -1){
            start = 3;
            end = line.substr(start + 1).find('<') - 8;
            edge.carrier = line.substr(12, end);
        }
        if (line.find("<carrierName>") != -1){
            start = 3;
            end = line.substr(start + 1).find('<') - 12;
            edge.carrierName = line.substr(16, end);
        }
        if (line.find("<dest_latitude>") != -1){
            start = 3;
            end = line.substr(start + 1).find('<') - 14;
            edgeLat = stod(line.substr(18, end));
        }
        if (line.find("<dest_longitude>") != -1){
            start = 3;
            end = line.substr(start + 1).find('<') - 15;
            edgeLong = stod(line.substr(19, end));
        }
        edge.location = make_pair(edgeLat, edgeLong);
        if (line.find("</edge>") != -1){
            edgess.push_back(edge);
        }
        if (line.find("</edges>") != -1){
            newNode.edges = edgess;
        }
        if (line.find("</Vertex>") != -1){
            a.push_back(newNode);
            edgess.clear();
        }
    }
}

#endif // GRAPH_H
