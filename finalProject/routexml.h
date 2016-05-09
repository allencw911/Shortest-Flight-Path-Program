#ifndef ROUTEXML_H
#define ROUTEXML_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct xmlInfo3
{
    string opening;
    string closing;
};

class routexml{
public:
    routexml();
private:
    void loadXML(xmlInfo3 xml[]);
    void removeCharacter(string &line, char item = ',', bool removeReplace = true);
    void readLineIntoArray(const string &line,string airlineInfo[]);
    void writeAirlineXML(ostream &out, xmlInfo3 xml[], string airlineInfo[]);

};

routexml::routexml(){
    string line, airlineInfo[9];
    xmlInfo3 xml[9];
    ofstream out("routes.xml");
    ifstream in("routes.dat");
    loadXML(xml);
    out<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl
      <<"<Routes>"<<endl;
    while(getline(in, line))
    {
#ifdef DEBUG
        //cout<<line<<endl;
#endif
        readLineIntoArray(line, airlineInfo);
        writeAirlineXML(out, xml, airlineInfo);
    }
    out<<"</Routes>";
    in.close();
    out.close();
}

void routexml::readLineIntoArray(const string &line,string airlineInfo[])
{
    stringstream ss;
    ss << line;
    for(int i = 0; i < 8; i++)
    {
        getline(ss, airlineInfo[i], ',');
        removeCharacter(airlineInfo[i],'\"');
    }
    getline(ss, airlineInfo[8]);
}

void routexml::removeCharacter(string &line, char item, bool removeReplace)
{
    unsigned int pos = 0;
    while((pos = line.find(item)) < line.size())
        if(removeReplace)
            line.erase(pos,1);
        else
            line[pos] = ' ';
}

void routexml::loadXML(xmlInfo3 xml[])
{
    xml[0].opening = "<Airline>";
    xml[0].closing = "</Airline>";
    xml[1].opening = "<Airline_ID>";
    xml[1].closing = "</Airline_ID>";
    xml[2].opening = "<Source_Airport>";
    xml[2].closing = "</Source_Airport>";
    xml[3].opening = "<Source_Airport_ID>";
    xml[3].closing = "</Source_Airport_ID>";
    xml[4].opening = "<Destination_Airport>";
    xml[4].closing = "</Destination_Airport>";
    xml[5].opening = "<Destination_Airport_ID>";
    xml[5].closing = "</Destination_Airport_ID>";
    xml[6].opening = "<Code Share>";
    xml[6].closing = "</Code Share>";
    xml[7].opening = "<Stops>";
    xml[7].closing = "</Stops>";
    xml[8].opening = "<Equipment>";
    xml[8].closing = "</Equipment>";
}

void routexml::writeAirlineXML(ostream &out, xmlInfo3 xml[], string airlineInfo[])
{
    out<<"\t<Route>"<<endl;
    for(int i = 0; i < 9; ++i)
        if (i == 0|| i == 1|| i == 2 || i == 3 || i == 4 || i == 5)
        out<<"\t\t"<<xml[i].opening<<airlineInfo[i]<<xml[i].closing<<endl;
    out<<"\t</Route>"<<endl;
}

#endif // ROUTEXML_H
