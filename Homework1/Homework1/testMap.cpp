#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Map m;  // maps strings to doubles
    assert(m.empty());
    ValueType v = -1234.5;
    assert( !m.get("abc", v)  &&  v == -1234.5); // v unchanged by get failure
    m.insert("xyz", 9876.5);
    assert(m.size() == 1);
    KeyType k = "hello";
    assert(m.get(0, k, v)  &&  k == "xyz"  &&  v == 9876.5);
    cout << "Passed all tests" << endl;
    
//    Map m;  // maps ints to strings
//    assert(m.empty());
//    ValueType v = "Ouch";
//    assert( !m.get(42, v)  &&  v == "Ouch"); // v unchanged by get failure
//    m.insert(123456789, "Wow!");
//    assert(m.size() == 1);
//    KeyType k = 9876543;
//    assert(m.get(0, k, v)  &&  k == 123456789  &&  v == "Wow!");
//    cout << "Passed all tests" << endl;
}

//void test()
//{
//    Map m;
//    assert(m.insert("Fred", 2.956));
//    assert(m.insert("Ethel", 3.538));
//    assert(m.size() == 2);
//    ValueType v = 42;
//    assert(!m.get("Lucy", v)  &&  v == 42);
//    assert(m.get("Fred", v)  &&  v == 2.956);
//    v = 42;
//    KeyType x = "Lucy";
//    assert(m.get(0, x, v)  &&
//           ((x == "Fred"  &&  v == 2.956)  ||  (x == "Ethel"  &&  v == 3.538)));
//    KeyType x2 = "Ricky";
//    assert(m.get(1, x2, v)  &&
//           ((x2 == "Fred"  &&  v == 2.956)  ||  (x2 == "Ethel"  &&  v == 3.538))  &&
//           x != x2);
//}
//
//int main()
//{
//    test();
//    cout << "Passed all tests" << endl;
//}
