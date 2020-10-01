#include "CarMap.h"
#include <iostream>

CarMap::CarMap() { }

bool CarMap::addCar(std::string license)
{
    return m_cars.insert(license, 0);
}

double CarMap::miles(std::string license) const
{
    double dis;
    if (m_cars.get(license, dis))
        return dis;
    else
        return -1;
}

bool CarMap::drive(std::string license, double distance)
{
    double dist;
    if (m_cars.get(license, dist) && distance > 0)
    {
        dist += distance;
        m_cars.update(license, dist);
        return true;
    }
        
    return false;
}

int CarMap::fleetSize() const
{
    return m_cars.size();
}

void CarMap::print() const
{
    std::string license;
    double distance;
    for (int i = 0; i < fleetSize(); i++)
    {
        m_cars.get(i, license, distance);
        std::cout << license << " " << distance << std::endl;
    }
}


