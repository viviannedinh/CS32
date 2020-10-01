//
//  main.cpp
//  Project2
//
//  Created by Vivianne Dinh on 1/25/20.
//  Copyright © 2020 Vivianne Dinh. All rights reserved.
//

#include <iostream>
#include "Map.h"
using namespace std;

int main()
{
    // test insert and constructor
    Map m1;
    m1.insert("ben", 58);
    m1.insert("vanessa", 50);
    m1.insert("krystal", 24);
    m1.insert("austin", 20);
    m1.insert("vivianne", 18);
    m1.insert("matthew", 15);
    m1.dump();

    // test copy constructor
    Map m2(m1);
    m2.dump();

    // test assignment operator
    Map m3;
    m3.insert("julia", 20);
    m3.insert("tyler", 16);
    m3.dump();
    m3 = m1;
    m3.dump();

    // test empty function
    if (m3.empty())
        cout << "map is empty" << endl;
    else
        cout << "map is not empty" << endl;

    // test size function
    Map m4;
    m3.dump();
    cout << "map has " << m3.size() << " elements" << endl;       // not empty map
    cout << "map has " << m4.size() << " elements" << endl;       // empty map, returns 0

    // test update function
    m1.update("vivianne", 19);
    m1.update("trevor", 20);          // key not in the map
    m1.dump();

    // test insertOrUpdate function
    m1.insertOrUpdate("christopher", 27);         // insert
    m1.insertOrUpdate("vivianne", 20);            // update
    m1.dump();

    // test erase function
    m1.erase("austin");
    m1.erase("hello");      // erase a key that isnt in the map
    m1.dump();

    // test contains function
    if (m1.contains("vivianne"))        // key is in map
        cout << "map contains the key" << endl;
    else
        cout << "map does not contain the key" << endl;

    if (m1.contains("austin"))          // key is not in map
        cout << "map contains the key" << endl;
    else
        cout << "map does not contain the key" << endl;

    // test first get function
    double val;
    bool nm = m1.get("vivianne", val);
    cout << "the value is " << val << endl;
    nm = m1.get("henry", val);          // key not in the map

    // test second get function
    string ns;
    nm = m1.get(8, ns, val);            // i is not in the map (over size())
    cout << "key: " << ns << " / value: " << val << endl;
    nm = m1.get(4, ns, val);
    cout << "key: " << ns << " / value: " << val << endl;

    // test swap function
    m1.dump();
    m2.dump();
    m1.swap(m2);
    m1.dump();
    m2.dump();

    // test reassign
    m1.dump();
    reassign(m1, m3);
    m3.dump();

    // test combine
    m1.dump();
    m2.dump();
    m2.insert("jillian", 24);
    m2.insert("timmy", 48);
    combine(m1, m2, m3);
    m3.dump();
}
