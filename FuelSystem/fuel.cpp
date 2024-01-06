#include "fuel.h"

FuelSys::FuelSys() {
	m_current = nullptr;
}

FuelSys::~FuelSys() {
	Tank* nextTank = nullptr;

	while (m_current != nullptr) {
		nextTank = m_current->m_next;
		removeTank(m_current->m_tankID);
		m_current = nextTank;
	}
}

/*
const FuelSys& FuelSys::operator=(const FuelSys& rhs) {

}*/

/* Function: addTank
 * -----------------
 * tankID: unique ID
 * capacity: How much fuel the tank can hold
 * tankFuel: Current amount of fuel in the tank at add
 *
 * Tries to add a new tank to the list
 *
 * return: true if a tank was successfully created else false
 */
bool FuelSys::addTank(int tankID, int capacity) {
	if (tankID < 0 || capacity < 0) {
		return false;
	}

	if (m_current == nullptr) {
		m_current = new Tank(tankID, capacity);
		return true;
	}

	Tank* lastTank = findEligibleTank(tankID);
	if (lastTank == nullptr) {
		return false;
	}

	lastTank->m_next = new Tank(tankID, capacity);

	return true;
}

/* Function: findEligibleTank
 * --------------------------
 * tankID: potential new tank ID
 *
 * Checks the list of existing tanks for duplicate ID
 *
 * return: last tank in the list if no duplicate ID else nullptr
 */
Tank* FuelSys::findEligibleTank(int tankID) {
	Tank* currentTank = m_current;

	while (currentTank->m_next != nullptr) {
		if (currentTank->m_tankID == tankID) {
			return nullptr;
		}
		currentTank = currentTank->m_next;
	}

	if (currentTank->m_tankID == tankID) {
		return nullptr;
	}

	return currentTank;
}


/* Function: removeTank
 * --------------------
 * tankID:
 * 
 * 
 */
bool FuelSys::removeTank(int tankID) {
	Tank* findTank = m_current;
	Tank* previousTank = m_current;

	if (findTank == nullptr) {
		return false;
	}

	//Go through the list and find the tankID
	while (findTank != nullptr) {
		previousTank = findTank;
		if (findTank->m_tankID == tankID) {
			break;
		}
		findTank = findTank->m_next;
	}

	if (findTank == nullptr) {
		return false;
	}

	//Check if the tank is the first one in the list
	if (previousTank->m_tankID == tankID) {
		if (findTank->m_next != nullptr) {
			m_current = findTank->m_next;
		}
	}
	else {
		previousTank->m_next = findTank->m_next;
	}

	Pump* deletePump = findTank->m_pumps;
	Pump* nextPump = nullptr;

	while (deletePump != nullptr) {
		nextPump = deletePump->m_next;
		removePump(findTank->m_tankID, deletePump->m_pumpID);
		deletePump = nextPump;
	}

	delete findTank;
	findTank = nullptr;

	return true;
}

/*
bool FuelSys::addPump(int tankID, int pumpID, int targetTank) {

}
*/

bool FuelSys::removePump(int tankID, int pumpID) {

}

/*
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
*/