#include "settest.h"

void setTest() {

    Set<int> s;
    printDebugInfo(s.insert(8));
    printDebugInfo(s.insert(4));
    printDebugInfo(s.insert(10));
    printDebugInfo(s.insert(10));
    printDebugInfo(s.insert(2));
    printDebugInfo(s.insert(5));
    printDebugInfo(s.insert(9));
    printDebugInfo(s.insert(12));
    printDebugInfo(s.insert(12));
    printDebugInfo(s.insert(1));
    printDebugInfo(s.insert(14));
    printDebugInfo(s.insert(7));
    {
        auto s2 = s;
        printSetSizeAndElems(s2);
    }
    auto it = s.find(10);
    // *it = 2;
    printDebugInfo(s.find(5));
    s.remove(4);
    printDebugInfo(s.find(5));
    s.remove(12);
    s.remove(14);
    printDebugInfo(s.insert(0));
    printDebugInfo(s.insert(16));
    {
        Set<int> s3;
        s3 = s;
        printSetSizeAndElems(s3);
    }
    s.remove(8);
    printSetSizeAndElems(s);
    const auto s4 = s;
    printSetSizeAndElems(s4);
}
