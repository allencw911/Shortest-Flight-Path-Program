
#include <iostream>
#include "airlinexml.h"
#include "airportxml.h"
#include "routexml.h"
#include "updatexml.h"
#include "graph.h"
using namespace std;

int main(){
        string choice;
        cout<<"Do you want to update the XML Files? (Y/N)"<<endl;
        cin>>choice;
        if (choice == "Y"){
            airlinexml airlinexml;
            routexml routexml;
            airportxml airportxml;
            updatexml update;
        }
        program program;
        program.go();
        
        return 0;
    }
