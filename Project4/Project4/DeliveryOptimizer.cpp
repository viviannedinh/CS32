#include "provided.h"
#include <vector>
using namespace std;

class DeliveryOptimizerImpl {
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    PointToPointRouter m_route;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm): m_route(sm) {
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl() {
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(const GeoCoord& depot, vector<DeliveryRequest>& deliveries, double& oldCrowDistance, double& newCrowDistance) const {
    // calculate old crow distance
    oldCrowDistance = distanceEarthMiles(depot, deliveries[0].location);
    for (int i = 1; i < deliveries.size(); i++)
        oldCrowDistance += distanceEarthMiles(deliveries[i].location, deliveries[i-1].location);
    oldCrowDistance += distanceEarthMiles(deliveries[deliveries.size()-1].location, depot);

    // find closest to depot
    double currClosest = 0;
    int indexOfClosest = 0;
    for (int i = 0; i < deliveries.size(); i++)  {
        double distFromDepot;
        list<StreetSegment> route;
        m_route.generatePointToPointRoute(depot, deliveries[i].location, route, distFromDepot);
        if (distFromDepot < currClosest) {
            indexOfClosest = i;
            currClosest = distFromDepot;
        }
    }
    
    DeliveryRequest temp = deliveries[0];
    deliveries[0] = deliveries[indexOfClosest];
    deliveries[indexOfClosest] = temp;
    
    for(int i = 0; i < deliveries.size()-1; i++)  {
        double closestDist = 0;
        int indexClosest = 0;
        for (int j = i+1; j < deliveries.size(); j++)  {
            double distFromLast;
            list<StreetSegment> route;
            m_route.generatePointToPointRoute(deliveries[i].location, deliveries[j].location, route, distFromLast);
            if (distFromLast < closestDist) {
                indexClosest = j;
                closestDist = distFromLast;
            }
        }
        
        DeliveryRequest temp = deliveries[i+1];
        deliveries[i+1] = deliveries[indexClosest];
        deliveries[indexClosest] = temp;
    }
    
    // calculate new crow distance
    newCrowDistance = distanceEarthMiles(depot, deliveries[0].location);
    for (int i = 1; i < deliveries.size(); i++)
        newCrowDistance += distanceEarthMiles(deliveries[i].location, deliveries[i-1].location);
    newCrowDistance += distanceEarthMiles(deliveries[deliveries.size()-1].location, depot);
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm) {
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer() {
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const {
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
