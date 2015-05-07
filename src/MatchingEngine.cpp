/*
 * MatchingEngine.cpp
 *
 *  Created on: May 3, 2015
 *      Author: lzy
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <cstring>
#include "MatchingEngine.h"
#include "OrderBook.h"

namespace Matching
{

MatchingEngine::MatchingEngine()
{
    m_orderBook = new OrderBook();
    vector<string> names{ TRADER };
    init( names );
}

void MatchingEngine::init( const vector<string>& names )
{
    m_orderBook->bookTradeForTrader( names );
}

void MatchingEngine::processOrder( Order* order )
{
    int qtyToMatch = order->m_quantity;

    // only match marketable order, qtyToMatch is the residual need to post on return
    m_orderBook->match( order, qtyToMatch );

    // post non marketable portion
    if( qtyToMatch > 0 )
    {
        if( qtyToMatch != order->m_quantity )
            order->m_quantity = qtyToMatch;
        m_orderBook->add( order );
    }
}

int MatchingEngine::run( const string& inFile )
{

    FILE *file = fopen( inFile.c_str(), "r");
    if ( NULL == file ) {
        fprintf( stderr, "Cannot open file at %s\n", inFile.c_str() );
        return -1;
    }

    int id, quantity, time;
    char name[20], buySellStr[5];
    float price;
    while( !feof( file ) ){
        // 70000001,Mal,73.21,100,100001,BUY
        int nItemsRead = fscanf( file, "%d,%[^,],%f,%d,%d,%s\n",
                &id, name, &price, &quantity, &time, buySellStr );
        if ( NCOL != nItemsRead)
        {
            cout << "Bad line" << endl;
        }

        bool isBuy = strcmp( BUYSTR, buySellStr ) == 0;
        int priceInt = price * 100 + 0.5;

        Order* order = new Order( id, name, priceInt, quantity, time, isBuy );
        processOrder( order );
    }

    int exposure = m_orderBook->getTraderExposure( TRADER );
    string str = exposure >= 0 ? "L" : "S";
    cout << str << endl;
    cout << abs( exposure ) << endl;

    return 0;
}

}



