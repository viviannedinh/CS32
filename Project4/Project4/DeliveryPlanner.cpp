#include "provided.h"
#include <vector>
#include <list>
using namespace std;

class DeliveryPlannerImpl {
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    DeliveryOptimizer m_do;
    PointToPointRouter m_ptpr;
    void commandMaker(const list<StreetSegment> route, vector<DeliveryCommand>& commands) const;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm): m_do(sm), m_ptpr(sm) {
    
}

DeliveryPlannerImpl::~DeliveryPlannerImpl() {
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const {
    
    double oldCD, newCD;
    vector<DeliveryRequest> del(deliveries);
    m_do.optimizeDeliveryOrder(depot, del, oldCD, newCD);
    
    // depot to first delivery
    list<StreetSegment> routeToFirst;
    double distTraveled;
    m_ptpr.generatePointToPointRoute(depot, del[0].location, routeToFirst, distTraveled);
    totalDistanceTravelled = distTraveled;
    
    commands.clear();
    commandMaker(routeToFirst, commands);
    DeliveryCommand deliverCommand;
    deliverCommand.initAsDeliverCommand(del[0].item);
    commands.push_back(deliverCommand);
    
    // middle deliveries
    for (int i = 0; i < deliveries.size()-1; i++) {
        list<StreetSegment> routeToNext;
        double distance;
        m_ptpr.generatePointToPointRoute(del[i].location, del[i+1].location, routeToNext, distance);
        totalDistanceTravelled += distance;
        
        commandMaker(routeToNext, commands);
        deliverCommand.initAsDeliverCommand(del[i+1].item);
        commands.push_back(deliverCommand);
    }
    
    // last delivery to depot
    list<StreetSegment> routeToDepot;
    double distance;
    m_ptpr.generatePointToPointRoute(del[del.size()-1].location, depot, routeToDepot, distance);
    totalDistanceTravelled += distance;
    
    commandMaker(routeToDepot, commands);
    // do command shit
    
    return NO_ROUTE;  // Delete this line and implement this function correctly
}

void DeliveryPlannerImpl::commandMaker(const list<StreetSegment> route, vector<DeliveryCommand>& commands) const {
    list<StreetSegment>::const_iterator it = route.begin();
    while(it != route.end()) {
        string streetName = it->name;
        
        double streetAngle = angleOfLine(*it);
        string streetDir;
        if (streetAngle >= 0 && streetAngle < 22.5)
            streetDir  = "east";
        else if (streetAngle >= 22.5 && streetAngle < 67.5)
            streetDir = "northeast";
        else if (streetAngle >= 67.5 && streetAngle < 112.5)
            streetDir = "north";
        else if (streetAngle >= 112.5 && streetAngle < 157.5)
            streetDir = "northwest";
        else if (streetAngle >= 157.5 && streetAngle < 202.5)
            streetDir = "west";
        else if (streetAngle >= 202.5 && streetAngle < 247.5)
            streetDir = "southwest";
        else if (streetAngle >= 247.5 && streetAngle < 292.5)
            streetDir = "south";
        else if (streetAngle >= 292.5 && streetAngle < 337.5)
            streetDir = "southeast";
        else if (streetAngle >= 337.5)
            streetDir = "east";
        
        double streetDist = distanceEarthMiles(it->start, it->end);
        list<StreetSegment>::const_iterator sameStreet = it;
        sameStreet++;
        while (sameStreet->name == streetName) {
            streetDist += distanceEarthMiles(sameStreet->start, sameStreet->end);
            sameStreet++;
            it++;
        }
        
        DeliveryCommand proceedCommand;
        proceedCommand.initAsProceedCommand(streetDir, streetName, streetDist);
        commands.push_back(proceedCommand);
        
        sameStreet--;
        it++;
        if (it == route.end())
            return;
        
        double turnAngle = angleBetween2Lines(*sameStreet, *it);
        string turnDir;
        if (turnAngle < 1 || turnAngle > 359)
            continue;
        else if (turnAngle >= 1 && turnAngle < 180)
            turnDir = "left";
        else if (turnAngle >= 180 && turnAngle <= 359)
            turnDir = "right";
        
        DeliveryCommand turnCommand;
        turnCommand.initAsTurnCommand(turnDir, it->name);
        commands.push_back(turnCommand);
    }
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm) {
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner() {
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const {
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
