/*
 * Order.h
 *
 *  Created on: May 3, 2015
 *      Author: lzy
 */

#ifndef ORDER_H_
#define ORDER_H_

#include <string>
using namespace std;

namespace Matching
{

/**
 * Order Type
 * e.g. 70000001,Mal,73.21,100,100001,BUY
 * */
typedef struct Order
{
    int m_id;
    int m_price; // in cents
    int m_quantity;
    int m_time;
    bool m_isBuy;
    string m_name;

    Order( int id, string name, int price, int quantity, int time, bool isBuy ) :
            m_id( id ), m_price( price ), m_quantity( quantity ),
            m_time( time ), m_isBuy( isBuy ), m_name( name ) {}
} Order;

inline
ostream& operator << ( ostream& out, const Order& order )
{
    out << order.m_name << "_" << order.m_price << "_" << order.m_quantity <<
            "_" << order.m_time << "_" << order.m_isBuy;
    return out;
}

inline
bool operator == ( const Order& lhs, const Order& rhs )
{
    return lhs.m_id == rhs.m_id &&
            lhs.m_name == rhs.m_name &&
            lhs.m_price == rhs.m_price &&
            lhs.m_quantity == rhs.m_quantity &&
            lhs.m_time == rhs.m_time &&
            lhs.m_isBuy == rhs.m_isBuy;
}

inline
bool operator != ( const Order& lhs, const Order& rhs )
{
    return !( lhs == rhs );
}

}

#endif /* ORDER_H_ */
