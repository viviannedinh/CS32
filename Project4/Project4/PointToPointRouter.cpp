#include "provided.h"
#include "ExpandableHashMap.h"
#include <list>
#include <queue>
#include <vector>
#include <cmath>
#include <map>
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* m_streetMap;
    double getDist(StreetSegment seg) const;
    struct Node {
        StreetSegment s;
        double dist;
        bool operator>(const Node& n) {
            return dist < n.dist;
        }
        bool operator<(const Node& n) {
            return dist > n.dist;
        }
        bool operator==(const Node& n) {
            return dist == n.dist;
        }
    };
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm) : m_streetMap(sm) {
}

PointToPointRouterImpl::~PointToPointRouterImpl() {
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(const GeoCoord& start, const GeoCoord& end, list<StreetSegment>& route, double& totalDistanceTravelled) const {
    if (start == end) {
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }
    vector<StreetSegment> segCheck;
    if (!m_streetMap->getSegmentsThatStartWith(start, segCheck) || !m_streetMap->getSegmentsThatStartWith(end, segCheck)) {
        return BAD_COORD;
    }
            
    queue<GeoCoord> coordQueue;
    ExpandableHashMap<GeoCoord, double> cost;
    ExpandableHashMap<GeoCoord, list<StreetSegment>> paths;
            
    coordQueue.push(start);
    cost.associate(start, 0.0);

    while (!coordQueue.empty()) {
        GeoCoord current = coordQueue.front();
        coordQueue.pop();
        if (current == end) {
            totalDistanceTravelled = *(cost.find(end));
            route = *(paths.find(end));
            return DELIVERY_SUCCESS;
        }
        vector<double> pathLengths;
        vector<Node> nodeVec;
        vector<StreetSegment> neighbors;
        m_streetMap->getSegmentsThatStartWith(current, neighbors);
        
        for (int i = 0; i < neighbors.size(); i++) {
            if(neighbors[i] == (*(paths.find(current))).front())
                continue;
            
            double distance = distanceEarthMiles(current, neighbors[i].end) + *(cost.find(current));
            cost.associate(neighbors[i].end, distance);
            pathLengths.push_back(distance);
            Node p;
            p.s = neighbors[i];
            p.dist = distance;
            nodeVec.push_back(p);
            list<StreetSegment>* currPath = paths.find(current);
            if (currPath == nullptr) {
                list<StreetSegment> n;
                n.push_back(neighbors[i]);
                paths.associate(neighbors[i].end, n);
            }
            else {
                currPath->push_back(neighbors[i]);
                paths.associate(neighbors[i].end, *currPath);
            }
            
        }
            sort(pathLengths.begin(), pathLengths.end());
        for (int i = 0; i < pathLengths.size(); i++) {
            for (int j = 0; j < nodeVec.size(); j++) {
                if(pathLengths[i] == nodeVec[j].dist) {
                    coordQueue.push(nodeVec[j].s.end);
                    break;
                }
            }
        }
    }
    
    return NO_ROUTE;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm) {
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter() {
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const {
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
