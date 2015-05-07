/*
 * MathingEngine.h
 *
 *  Created on: May 3, 2015
 *      Author: lzy
 */

#ifndef MATCHINGENGINE_H_
#define MATCHINGENGINE_H_

#include "OrderBook.h"

namespace Matching
{

#define BUYSTR "BUY"
#define TRADER "Kaylee"
#define NCOL 6

class MatchingEngine
{
private:
    OrderBook* m_orderBook;

public:
    MatchingEngine();
    virtual ~MatchingEngine() { clean(); }

    const OrderBook* getOrderBook() const { return m_orderBook; }

    void init( const vector<string>& names );
    void clean() { delete m_orderBook; }
    int run( const string& inFile );

    void processOrder( Order* order );
};

}

#endif /* MATCHINGENGINE_H_ */
