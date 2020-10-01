#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

unsigned int hasher(const GeoCoord& g) {
    return hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl {
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_map;
};

StreetMapImpl::StreetMapImpl() {
}

StreetMapImpl::~StreetMapImpl() {
}

bool StreetMapImpl::load(string mapFile) {
    ifstream infile(mapFile);
    if (!infile)
        return false;
    
    string s;
    while (getline(infile, s)) {
        string streetName;
        istringstream iss(s);
        if (iss >> streetName) {
            string numSegments;
            getline(infile, numSegments);
            for (int i = 0; i < stoi(numSegments); i++) {
                string startLat, startLong, endLat, endLong;
                getline(infile, startLat, ' ');
                getline(infile, startLong, ' ');
                getline(infile, endLat, ' ');
                getline(infile, endLong);
                
                GeoCoord start(startLat, startLong);
                GeoCoord end(endLat, endLong);
                StreetSegment startSeg(start, end, streetName);
                StreetSegment endSeg(end, start, streetName);
                
                vector<StreetSegment>* mapVal = m_map.find(start);
                if (mapVal == nullptr) {
                    vector<StreetSegment> newValue;
                    newValue.push_back(startSeg);
                    m_map.associate(start, newValue);
                }
                else {
                    mapVal->push_back(startSeg);
                    m_map.associate(start, *mapVal);
                }
                
                mapVal = m_map.find(end);
                if (mapVal == nullptr) {
                    vector<StreetSegment> newValue;
                    newValue.push_back(endSeg);
                    m_map.associate(start, newValue);
                }
                else {
                    mapVal->push_back(endSeg);
                    m_map.associate(start, *mapVal);
                }
            }
        }
    }
    
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const {
    if (m_map.find(gc) != nullptr) {
        segs.clear();
        segs = *(m_map.find(gc));
        return true;
    }
    return false;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap() {
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap() {
    delete m_impl;
}

bool StreetMap::load(string mapFile) {
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const {
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
