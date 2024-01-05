#include "fuel.h"

FuelSys::FuelSys() {

}

FuelSys::~FuelSys() {

}

const FuelSys& FuelSys::operator=(const FuelSys& rhs) {

}

bool FuelSys::addTank(int tankID, int capacity, int tankFuel) {

}

Tank* FuelSys::findEligibleTank(int tankID) {

}

bool FuelSys::removeTank(int tankID) {

}

bool FuelSys::addPump(int tankID, int pumpID, int targetTank) {

}

bool FuelSys::removePump(int tankID, int pumpID) {

}

bool FuelSys::fill(int tankID, int fuel) {

}

bool FuelSys::drain(int tankID, int pumpID, int fuel) {

}

bool FuelSys::findTank(int tankID) {

}

int FuelSys::totalFuel() const {

}

void FuelSys::dumpSys() const {

}

void FuelSys::dumpPumps(Pump* pumps) const {
}