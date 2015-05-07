/*
 * Main.cpp
 *
 *  Created on: May 3, 2015
 *      Author: lzy
 */

#include <iostream>
#include <unistd.h>
#include "MatchingEngine.h"
using namespace std;

void usage()
{
    cout << "Matching Engine\n" << endl;
    cout << "Usage: matching [-i inputFile]\n" << endl;
    cout << "Options: " << endl;
    cout << "  -i, input file order.csv path. If not specify, default to ../data/orders.csv" << endl;
    cout << endl;
}

int main( int argc, char** argv )
{
    string infile = "../data/orders.csv";
    int opt;
    while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch(opt) {
        case 'i':
            infile = optarg;
            break;
        default:
            usage ();
            return -1;
        }
    }

    Matching::MatchingEngine engine;
    return engine.run( infile );
}


