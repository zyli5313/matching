/*
 * TestMatching.cpp
 *
 *  Created on: May 5, 2015
 *      Author: lzy
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestMatch
#include <boost/test/unit_test.hpp>
#include "../src/MatchingEngine.h"
#include "TestUtils.h"
using namespace std;

/**
 * Test Plan:
 * Add
 * Same/different tree (covered in Match tests)
 * Ordering priceTree orderTree (covered in match tests)
 *
 * Match
 * No match
 * Deplete a portion of quote
 * Deplete one quote
 * Deplete multiple quote. Post and no post
 * Deplete one price level
 * Deplete multiple price level
 * Deplete entire tree
 *
 * */
BOOST_AUTO_TEST_SUITE( Matching )

BOOST_AUTO_TEST_CASE( TestNoMatch )
{
    MatchingEngine me;
    string n1 = "Mal", n2 = "Kaylee", n3 = "Tom";
    me.init( {n1, n2, n3 } );

    Order* b1 = new Order( 70000001, n1, 7321, 100, 100001, true );
    Order* b2 = new Order( 70000002, n2, 7321, 200, 100002, true );
    Order* s1 = new Order( 70000003, n3, 7421, 300, 100003, false );
    me.processOrder( b1 );
    me.processOrder( b2 );
    me.processOrder( s1 );

    OrderBook* orderBook = const_cast< OrderBook* >( me.getOrderBook() );
    BOOST_CHECK( orderBookEquals( orderBook, { b2, b1 }, { s1 } ) );

    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n1 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n2 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n3 ), 0 );
}

BOOST_AUTO_TEST_CASE( TestOneQuotePartialMatch )
{
    MatchingEngine me;
    string n1 = "Mal", n2 = "Kaylee", n3 = "Tom";
    me.init( {n1, n2, n3 } );

    Order* b1 = new Order( 70000001, n1, 7321, 100, 100001, true );
    Order* b2 = new Order( 70000002, n2, 7321, 200, 100002, true );
    Order* s1 = new Order( 70000003, n3, 7221,  50, 100003, false );
    me.processOrder( b1 );
    me.processOrder( b2 );
    me.processOrder( s1 );

    OrderBook* orderBook = const_cast< OrderBook* >( me.getOrderBook() );
    b2->m_quantity -= 50;
    BOOST_CHECK( orderBookEquals( orderBook, { b2, b1 }, {} ) );

    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n1 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n2 ), 50 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n3 ), -50 );
}

BOOST_AUTO_TEST_CASE( TestOneQuoteFullyMatch )
{
    MatchingEngine me;
    string n1 = "Mal", n2 = "Kaylee", n3 = "Tom";
    me.init( {n1, n2, n3 } );

    Order* b1 = new Order( 70000001, n1, 7321, 100, 100001, true );
    Order* b2 = new Order( 70000002, n2, 7321, 200, 100002, true );
    Order* s1 = new Order( 70000003, n3, 7221, 200, 100003, false );
    me.processOrder( b1 );
    me.processOrder( b2 );
    me.processOrder( s1 );

    OrderBook* orderBook = const_cast< OrderBook* >( me.getOrderBook() );
    BOOST_CHECK( orderBookEquals( orderBook, { b1 }, {} ) );

    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n1 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n2 ), 200 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n3 ), -200 );
}

BOOST_AUTO_TEST_CASE( TestMultipleQuoteFullyMatch )
{
    MatchingEngine me;
    string n1 = "Mal", n2 = "Kaylee", n3 = "Tom", n4 = "Kate", n5 = "Rob";
    me.init( {n1, n2, n3, n4, n5 } );

    Order* b1 = new Order( 70000001, n1, 7321, 100, 100001, true );
    Order* b2 = new Order( 70000002, n2, 7321, 200, 100002, true );
    Order* b3 = new Order( 70000003, n3, 7321, 300, 100003, true );
    Order* s1 = new Order( 70000004, n4, 7441, 200, 100004, false );
    Order* s2 = new Order( 70000005, n5, 7221, 500, 100005, false );
    me.processOrder( b1 );
    me.processOrder( b2 );
    me.processOrder( b3 );
    me.processOrder( s1 );
    me.processOrder( s2 );

    OrderBook* orderBook = const_cast< OrderBook* >( me.getOrderBook() );
    BOOST_CHECK( orderBookEquals( orderBook, { b1 }, { s1 } ) );

    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n1 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n2 ), 200 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n3 ), 300 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n4 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n5 ), -500 );
}

BOOST_AUTO_TEST_CASE( TestMultipleQuoteOutsizeMatchWithResidual )
{
    MatchingEngine me;
    string n1 = "Mal", n2 = "Kaylee", n3 = "Tom", n4 = "Kate", n5 = "Rob";
    me.init( {n1, n2, n3, n4, n5 } );

    Order* b1 = new Order( 70000001, n1, 7311, 100, 100001, true );
    Order* b2 = new Order( 70000002, n2, 7321, 200, 100002, true );
    Order* b3 = new Order( 70000003, n3, 7322, 300, 100003, true );
    Order* s1 = new Order( 70000004, n4, 7441, 200, 100004, false );
    Order* s2 = new Order( 70000005, n5, 7320, 600, 100005, false );
    me.processOrder( b1 );
    me.processOrder( b2 );
    me.processOrder( b3 );
    me.processOrder( s1 );
    me.processOrder( s2 );

    OrderBook* orderBook = const_cast< OrderBook* >( me.getOrderBook() );
    s2->m_quantity -= (200 + 300);
    BOOST_CHECK( orderBookEquals( orderBook, { b1 }, { s2, s1 } ) );

    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n1 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n2 ), 200 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n3 ), 300 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n4 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n5 ), -500 );
}

BOOST_AUTO_TEST_CASE( TestOnePriceLevelMatch )
{
    MatchingEngine me;
    string n1 = "Mal", n2 = "Kaylee", n3 = "Tom", n4 = "Kate", n5 = "Rob";
    me.init( {n1, n2, n3, n4, n5 } );

    Order* b1 = new Order( 70000001, n1, 7311, 100, 100001, true );
    Order* b2 = new Order( 70000002, n2, 7322, 200, 100002, true );
    Order* b3 = new Order( 70000003, n3, 7322, 300, 100003, true );
    Order* s1 = new Order( 70000004, n4, 7441, 200, 100004, false );
    Order* s2 = new Order( 70000005, n5, 7320, 500, 100005, false );
    me.processOrder( b1 );
    me.processOrder( b2 );
    me.processOrder( b3 );
    me.processOrder( s1 );
    me.processOrder( s2 );

    OrderBook* orderBook = const_cast< OrderBook* >( me.getOrderBook() );
    BOOST_CHECK( orderBookEquals( orderBook, { b1 }, { s1 } ) );

    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n1 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n2 ), 200 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n3 ), 300 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n4 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n5 ), -500 );
}

BOOST_AUTO_TEST_CASE( TestMultiplePriceLevelMatch )
{
    MatchingEngine me;
    string n1 = "Mal", n2 = "Kaylee", n3 = "Tom", n4 = "Kate", n5 = "Rob", n6 = "Bill";
    me.init( {n1, n2, n3, n4, n5, n6 } );

    Order* b1 = new Order( 70000001, n1, 7311, 100, 100001, true );
    Order* b2 = new Order( 70000002, n2, 7322, 200, 100002, true );
    Order* b3 = new Order( 70000003, n3, 7322, 300, 100003, true );
    Order* b4 = new Order( 70000004, n4, 7323, 400, 100004, true );
    Order* s1 = new Order( 70000005, n5, 7441, 200, 100005, false );
    Order* s2 = new Order( 70000006, n6, 7320, 900, 100006, false );
    me.processOrder( b1 );
    me.processOrder( b2 );
    me.processOrder( b3 );
    me.processOrder( b4 );
    me.processOrder( s1 );
    me.processOrder( s2 );

    OrderBook* orderBook = const_cast< OrderBook* >( me.getOrderBook() );
    BOOST_CHECK( orderBookEquals( orderBook, { b1 }, { s1 } ) );

    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n1 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n2 ), 200 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n3 ), 300 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n4 ), 400 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n5 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n6 ), -900 );
}

BOOST_AUTO_TEST_CASE( TestEntireTreeMatch )
{
    MatchingEngine me;
    string n1 = "Mal", n2 = "Kaylee", n3 = "Tom", n4 = "Kate", n5 = "Rob", n6 = "Bill";
    me.init( {n1, n2, n3, n4, n5, n6 } );

    Order* b1 = new Order( 70000001, n1, 7311, 100, 100001, true );
    Order* b2 = new Order( 70000002, n2, 7322, 200, 100002, true );
    Order* b3 = new Order( 70000003, n3, 7322, 300, 100003, true );
    Order* b4 = new Order( 70000004, n4, 7323, 400, 100004, true );
    Order* s1 = new Order( 70000005, n5, 7441, 200, 100005, false );
    Order* s2 = new Order( 70000006, n6, 7320, 900, 100006, false );
    me.processOrder( b1 );
    me.processOrder( b2 );
    me.processOrder( b3 );
    me.processOrder( b4 );
    me.processOrder( s1 );
    me.processOrder( s2 );

    OrderBook* orderBook = const_cast< OrderBook* >( me.getOrderBook() );
    BOOST_CHECK( orderBookEquals( orderBook, {  }, { s1 } ) );

    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n1 ), 100 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n2 ), 200 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n3 ), 300 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n4 ), 400 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n5 ), 0 );
    BOOST_CHECK_EQUAL( orderBook->getTraderExposure( n6 ), -1000 );
}

BOOST_AUTO_TEST_SUITE_END()


