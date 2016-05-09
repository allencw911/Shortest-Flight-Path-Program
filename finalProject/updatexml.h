#ifndef UPDATEXML_H
#define UPDATEXML_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
using namespace std;
struct xmlInfo
{
    string opening;
    string closing;
};

struct edges{
    string source;
    string airport;
    string carrier;
    string carrierName;
    pair<double, double> location;
    void print(){
        cout<<"\t"<<"Source: "<<source<<endl;
        cout<<"\t"<<"Airport: "<<airport<<endl;
        cout<<"\t"<<"Carrier: "<<carrier<<endl;
        cout<<endl;
    }

};

struct nodes{
    string name;
    string iata;
    string city;
    string country;
    pair<double, double> location;
    std::vector<edges> myedges;
    void print(){
        cout<<"Name: "<<name<<endl;
        cout<<"Iata: "<<iata<<endl;
        cout<<"City: "<<city<<endl;
        cout<<"Country: "<<country<<endl;
        cout<<"Latitude: "<<location.first<<endl;
        cout<<"Longitude: "<<location.second<<endl;
        for (int i = 0; i < myedges.size(); i ++){
            cout<<"\t"<<"Destination Airport: "<<myedges[i].airport<<endl;
            cout<<"\t"<<"Carrier: "<<myedges[i].carrier<<endl;
            cout<<"\t"<<"Carrier Name: "<<myedges[i].carrierName<<endl;
        }
        cout<<endl;
    }
};

class updatexml{
public:
    updatexml();
private:
    void loadXML(xmlInfo xml[]);
    void removeCharacter(string &line, char item = ',', bool removeReplace = true);
    void readLineIntoArray(const string &line,string airlineInfo[]);
    void writeAirlineXML(ostream &out, xmlInfo xml[], string airlineInfo[]);
    void readAirports(vector<nodes>& a);
    void getRoutes(vector<edges>& a);
    void getAirlines(vector<edges>& a);
    void combineRA(vector<edges>& a, vector<edges>& b);
    void getLocations(vector<edges>& a, vector<nodes>& b);
    void getEdges(vector<nodes>& a, vector<edges>& b);
    void writetoXML(ostream& out, xmlInfo xml[], vector<nodes> mynodes);
};

updatexml::updatexml(){
    xmlInfo xml[12];
    loadXML(xml);
    vector<nodes> mynodes;
    vector<edges> routes;
    vector<edges> airlines;
    readAirports(mynodes);
    cout<<"FINISHED AIRPORTS XML"<<endl;
    getRoutes(routes);
    cout<<"FINISHED ROUTES XML"<<endl;
    getAirlines(airlines);
    cout<<"FINISHED AIRLINES XML. STARTING TO COMBINE..."<<endl;
    combineRA(routes, airlines);
    getLocations(routes, mynodes);
    getEdges(mynodes, routes);
    ofstream out("vertices.xml");
    out<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl
    <<"<Vertices>"<<endl;
    writetoXML(out, xml, mynodes);
    out<<"</Vertices>"<<endl;
    out.close();
}

void updatexml::writetoXML(ostream& out, xmlInfo xml[], vector<nodes> mynodes){
    for (int i = 0; i < mynodes.size(); i++){
        out<<"<Vertex>"<<endl;
        out<<"\t"<<xml[0].opening<<mynodes[i].name<<xml[0].closing<<endl;
        out<<"\t"<<xml[1].opening<<mynodes[i].iata<<xml[1].closing<<endl;
        out<<"\t"<<xml[2].opening<<mynodes[i].city<<xml[2].closing<<endl;
        out<<"\t"<<xml[3].opening<<mynodes[i].location.first<<xml[3].closing<<endl;
        out<<"\t"<<xml[4].opening<<mynodes[i].location.second<<xml[4].closing<<endl;
        out<<"\t"<<xml[5].opening<<endl;
        for (int j = 0; j < mynodes[i].myedges.size(); j ++){
            out<<"\t\t"<<xml[6].opening<<endl;
            out<<"\t\t\t"<<xml[7].opening<<mynodes[i].myedges[j].airport<<xml[7].closing<<endl;
            out<<"\t\t\t"<<xml[8].opening<<mynodes[i].myedges[j].carrier<<xml[8].closing<<endl;
            out<<"\t\t\t"<<xml[9].opening<<mynodes[i].myedges[j].carrierName<<xml[9].closing<<endl;
            out<<"\t\t\t"<<xml[10].opening<<mynodes[i].myedges[j].location.first<<xml[10].closing<<endl;
            out<<"\t\t\t"<<xml[11].opening<<mynodes[i].myedges[j].location.second<<xml[11].closing<<endl;
            out<<"\t\t"<<xml[6].closing<<endl;
        }
        out<<"\t"<<xml[5].closing<<endl;
        out<<"</Vertex>"<<endl;
    }
}

void updatexml::readAirports(vector<nodes>& a){
    string line;
    ifstream in("airports.xml");
    nodes newNode;
    unsigned long start;
    unsigned long end;
    double latitude;
    double longitude;

    while(getline(in, line))
    {
        if (line.find("<name>") != -1){
            start = 2;
            end = line.substr(start + 1).find('<') - 5;
            newNode.name = line.substr(8, end);
        }
        if (line.find("<city>") != -1){
            start = 2;
            end = line.substr(start + 1).find('<') - 5;
            newNode.city = line.substr(8, end);
        }
        if (line.find("<country>") != -1){
            start = 2;
            end = line.substr(start + 1).find('<') - 8;
            newNode.country = line.substr(11, end);
        }
        if (line.find("<iata>") != -1){
            start = 2;
            end = line.substr(start + 1).find('<') - 5;
            newNode.iata = line.substr(8, end);
        }
        if (line.find("<latitude>") != -1){
            start = 2;
            end = line.substr(start + 1).find('<') - 9;
            latitude = stod(line.substr(12, end));
        }
        if (line.find("<longitude>") != -1){
            start = 2;
            end = line.substr(start + 1).find('<') - 10;
            longitude = stod(line.substr(13, end));
        }
        newNode.location = make_pair(latitude, longitude);
        if (line.find("</airport>") != -1)
            a.push_back(newNode);
    }
    in.close();
}

void updatexml::getRoutes(vector<edges>& a){
    ifstream inRoutes("routes.xml");
    string line;
    edges edge;
    while (getline(inRoutes, line)){
        if (line.find("<Airline_ID>") != -1){
            unsigned long start = 2;
            unsigned long end = line.substr(start + 1).find('<') - 11;
            edge.carrier = line.substr(14, end);
        }
        if (line.find("<Source_Airport>") != -1){
            edge.source = line.substr(18,3);
        }
        if (line.find("<Destination_Airport>") != -1){
            edge.airport = line.substr(23, 3);
        }
        if (line.find("</Route>") != -1){
            a.push_back(edge);
        }
    }
    inRoutes.close();
}

void updatexml::getAirlines(vector<edges>& a){
    ifstream inAirlines("airlines.xml");
    string line;
    edges edge;
    unsigned long start, end;
    while(getline(inAirlines, line)){
        if (line.find("<Airline_ID>") != -1){
            start = line.find("<Airline_ID>");
            end = line.substr(start + 1).find('<') - 11;
            edge.carrier = line.substr(15, end);
        }
        if (line.find("<Name>") != -1){
            start = 3;
            end = line.substr(start + 1).find('<') - 5;
            edge.carrierName = line.substr(9, end);
        }
        if (line.find("</Airlines>") != -1){
            a.push_back(edge);
        }
    }
    inAirlines.close();
}

void updatexml::combineRA(vector<edges>& a, vector<edges>& b){
    for (int i = 0; i < a.size(); i++){
        for (int j = 0; j < b.size(); j++){
            if (a[i].carrier == b[j].carrier){
                a[i].carrierName = b[j].carrierName;
            }
        }
    }
}

void updatexml::getLocations(vector<edges>& a, vector<nodes>& b){
    for (int i = 0; i < a.size(); i ++){
        for (int j = 0; j < b.size(); j++){
            if (a[i].airport == b[j].iata){
                a[i].location.first = b[j].location.first;
                a[i].location.second = b[j].location.second;
            }
        }
    }
}

void updatexml::getEdges(vector<nodes>& a, vector<edges>& b){
    for (int i = 0; i < a.size(); i++){
        for (int j = 0; j < b.size(); j++){
            if (a[i].iata == b[j].source){
                a[i].myedges.push_back(b[j]);
            }
        }
    }
}

void updatexml::loadXML(xmlInfo xml[])
{
    xml[0].opening = "<name>";
    xml[0].closing = "</name>";
    xml[1].opening = "<iata>";
    xml[1].closing = "</iata>";
    xml[2].opening = "<city>";
    xml[2].closing = "</city>";
    xml[3].opening = "<latitude>";
    xml[3].closing = "</latitude>";
    xml[4].opening = "<longitude>";
    xml[4].closing = "</longitude>";
    xml[5].opening = "<edges>";
    xml[5].closing = "</edges>";
    xml[6].opening = "<edge>";
    xml[6].closing = "</edge>";
    xml[7].opening = "<airport>";
    xml[7].closing = "</airport>";
    xml[8].opening = "<carrier>";
    xml[8].closing = "</carrier>";
    xml[9].opening = "<carrierName>";
    xml[9].closing = "</carrierName>";
    xml[10].opening = "<dest_latitude>";
    xml[10].closing = "</dest_latitude>";
    xml[11].opening = "<dest_longitude>";
    xml[11].closing = "</dest_longitude>";
}

void updatexml::writeAirlineXML(ostream &out, xmlInfo xml[], string airlineInfo[])
{
    out<<"\t<vertex>"<<endl;
    for(int i = 0; i < 8; ++i)
        out<<"\t\t"<<xml[i].opening<<airlineInfo[i]<<xml[i].closing<<endl;
    out<<"\t</vertex>"<<endl;
}

#endif // UPDATEXML_H
