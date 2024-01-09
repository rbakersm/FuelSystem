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

	if (!findTank(tankID)) {
		Tank* endTank = getEndTank(tankID);
		endTank->m_next = new Tank(tankID, capacity);
		return true;
	}

	return false;
}

/* Function: getEndTank
 * --------------------------
 * tankID: potential new tank ID
 *
 * Checks the list of existing tanks for duplicate ID
 *
 * return: last tank in the list if no duplicate ID else nullptr
 */
Tank* FuelSys::getEndTank(int tankID) {
	Tank* currentTank = m_current; //Current tank in the list to check against the ID

	while (currentTank->m_next != nullptr) {
		currentTank = currentTank->m_next;
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
	Tank* currentTank = m_current;
	Tank* previousTank = m_current;

	if (currentTank == nullptr) {
		return false;
	}

	//Go through the list and find the tankID
	while (currentTank != nullptr) {
		previousTank = currentTank;
		if (currentTank->m_tankID == tankID) {
			break;
		}
		currentTank = currentTank->m_next;
	}

	if (currentTank == nullptr) {
		return false;
	}

	//Check if the tank is the first one in the list
	if (previousTank->m_tankID == tankID) {
		m_current = currentTank->m_next;
	}
	else {
		previousTank->m_next = currentTank->m_next;
	}

	/*
	Pump* deletePump = findTank->m_pumps;
	Pump* nextPump = nullptr;
	*/

	/*
	while (deletePump != nullptr) {
		nextPump = deletePump->m_next;
		removePump(findTank->m_tankID, deletePump->m_pumpID);
		deletePump = nextPump;
	}
	*/
	delete currentTank;
	currentTank = nullptr;

	return true;
}

/*
 * Function: addPump
 * -----------------
 * 
 */
bool FuelSys::addPump(int tankID, int pumpID, int targetTank) {
	if (findTank(tankID)) {
		if (!findPump(tankID, pumpID)) {
			Tank* tank = getTank(tankID);

			if (tank->m_pumps == nullptr) {
				tank->m_pumps = new Pump(pumpID, targetTank);
				return true;
			}

			Pump* endPump = getEndPump(tank);
			endPump->m_next = new Pump(pumpID, targetTank);

		}
	}

	return false;
}

/*
bool FuelSys::removePump(int tankID, int pumpID) {

}
*/

/*
bool FuelSys::fill(int tankID, int fuel) {

}

bool FuelSys::drain(int tankID, int pumpID, int fuel) {

}
*/

/*
 * Function: findTank
 * ------------------
 * tankID: ID of the target tank
 *
 * Searches the list of tank to find the target ID
 * 
 * return: true if the tank is found, false otherwise
 */
bool FuelSys::findTank(int tankID) {
	Tank* currentTank = m_current;

	while (currentTank != nullptr) {
		if (currentTank->m_tankID == tankID) {
			return true;
		}

		currentTank = currentTank->m_next;
	}

	return false;
}

/*
 * Function: findPump
 * ------------------
 * tankID: target tank ID
 * pumpID: target pump ID
 * 
 * return: true if the pump was found, false otherwise
 */
bool FuelSys::findPump(int tankID, int pumpID) {
	Tank* targetTank = getTank(tankID);
	Pump* currentPump = targetTank->m_pumps;

	while (currentPump != nullptr) {
		if (currentPump->m_pumpID == pumpID) {
			return true;
		}

		currentPump->m_next;
	}

	return false;
}

/*
 * Function: getEndPump
 * --------------------
 */
Pump* FuelSys::getEndPump(Tank* tank) {
	Pump* currentPump = tank->m_pumps;

	while (currentPump->m_next != nullptr) {
		currentPump->m_next;
	}

	return currentPump;
}

/*
 * Function: getTank
 * -----------------
 * tankID: ID of the target tank
 * 
 * Search the tank list for the target ID and return the tank
 * 
 * return: The tank object if found in list, else null
 */
Tank* FuelSys::getTank(int tankID) {
	Tank* currentTank = m_current;

	while (currentTank != nullptr) {
		if (currentTank->m_tankID == tankID) {
			return currentTank;
		}
	}

	return nullptr;
}

/*
int FuelSys::totalFuel() const {

}

void FuelSys::dumpSys() const {

}

void FuelSys::dumpPumps(Pump* pumps) const {
}
*/