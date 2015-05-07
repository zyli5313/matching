/*
 * TestUtils.h
 *
 *  Created on: May 7, 2015
 *      Author: lzy
 */

#ifndef TEST_TESTUTILS_H_
#define TEST_TESTUTILS_H_

#include "../src/OrderBook.h"
using namespace std;

namespace Matching
{

/**
 * Test bid in descending order
 * Test ask in ascending order
 * */
inline
bool priceTreeEquals( PriceTree* priceTree, vector< Order* >& orders )
{
    unsigned int num = 0; // num elements in priceTree
    for( PriceTreeIt it = priceTree->begin(); it != priceTree->end(); ++it )
    {
        OrderTree* orderTree = it->second->getOrderTree();
        for( OrderTreeIt itOrder = orderTree->begin(); itOrder != orderTree->end(); ++itOrder )
        {
            if( num >= orders.size() || *orders[num] != *( *itOrder ) )
                return false;
            ++num;
        }
    }
    return num == orders.size();
}

/**
 * Test if orderBook equals expected
 * */
inline
bool orderBookEquals( OrderBook* book, vector< Order* > bids, vector< Order* > asks)
{
    return priceTreeEquals( book->getBidTree(), bids ) &&
            priceTreeEquals( book->getAskTree(), asks );
}

}
#endif /* TEST_TESTUTILS_H_ */
