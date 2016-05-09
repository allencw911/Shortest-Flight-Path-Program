#ifndef AIRPORTXML_H
#define AIRPORTXML_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct xmlInfo2
{
    string opening;
    string closing;
};

class airportxml{
public:
    airportxml();
private:
    void loadXML(xmlInfo2 xml[]);
    void removeCharacter(string &line, char item = ',', bool removeReplace = true);
    void readLineIntoArray(const string &line,string airportInfo[]);
    void writeAirportXML(ostream &out, xmlInfo2 xml[], string airportInfo[]);
};

airportxml::airportxml(){
    string line, airportInfo[12];
    xmlInfo2 xml[12];
    ofstream out("airports.xml");
    ifstream in("airports.dat");
    loadXML(xml);
    out<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl<<"<airports>"<<endl;
    while(getline(in, line))
    {
#ifdef DEBUG
        //cout<<line<<endl;
#endif
        readLineIntoArray(line, airportInfo);
        writeAirportXML(out, xml, airportInfo);
    }
    out<<"</airports>";
    in.close();
    out.close();
    
}

void airportxml::readLineIntoArray(const string &line,string airportInfo[])
{
    stringstream ss;
    ss << line;
    for(int i = 0; i < 12; i++)
    {
        getline(ss, airportInfo[i], ',');
        removeCharacter(airportInfo[i],'\"');
        removeCharacter(airportInfo[i],'\n');
        removeCharacter(airportInfo[i],'\012');
        removeCharacter(airportInfo[i],'\015');
    }
}

void airportxml::removeCharacter(string &line, char item, bool removeReplace)
{
    unsigned int pos = 0;
    while((pos = line.find(item)) < line.size())
        if(removeReplace)
            line.erase(pos,1);
        else
            line[pos] = ' ';
}

void airportxml::loadXML(xmlInfo2 xml[])
{
    xml[0].opening = "<id>";
    xml[0].closing = "</id>";
    xml[1].opening = "<name>";
    xml[1].closing = "</name>";
    xml[2].opening = "<city>";
    xml[2].closing = "</city>";
    xml[3].opening = "<country>";
    xml[3].closing = "</country>";
    xml[4].opening = "<iata>";
    xml[4].closing = "</iata>";
    xml[5].opening = "<icao>";
    xml[5].closing = "</icao>";
    xml[6].opening = "<latitude>";
    xml[6].closing = "</latitude>";
    xml[7].opening = "<longitude>";
    xml[7].closing = "</longitude>";
    xml[8].opening = "<altitude>";
    xml[8].closing = "</altitude>";
    xml[9].opening = "<timezone>";
    xml[9].closing = "</timezone>";
    xml[10].opening = "<dst>";
    xml[10].closing = "</dst>";
    xml[11].opening = "<tz>";
    xml[11].closing = "</tz>";
}

void airportxml::writeAirportXML(ostream &out, xmlInfo2 xml[], string airportInfo[])
{
    bool bad = false;
    out<<"\t<airport>"<<endl;
    for(int i = 0; i < 12; ++i){
        if ( (i == 0||i == 1|| i == 2|| i == 3 || i == 4 || i == 6 || i == 7) && bad == false ){
            if (i == 6){
                if (airportInfo[i].find('.') == -1){
                    out<<"\t\t"<<xml[i].opening<<airportInfo[i+1]<<xml[i].closing<<endl;
                    bad = true;
                }else{
                    out<<"\t\t"<<xml[i].opening<<airportInfo[i]<<xml[i].closing<<endl;
                }
            }else{
                out<<"\t\t"<<xml[i].opening<<airportInfo[i]<<xml[i].closing<<endl;
            }
        }
        if (i == 7 && bad == true){
            out<<"\t\t"<<xml[i].opening<<airportInfo[i+1]<<xml[i].closing<<endl;
        }
    }
    out<<"\t</airport>"<<endl;
    bad = false;
}

#endif // AIRPORTXML_H
