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
		if (currentTank->m_tankID == tankID) {
			break;
		}
		previousTank = currentTank;
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

	Pump* currentPump = currentTank->m_pumps;
	Pump* nextPump = nullptr;

	while (currentPump != nullptr) {
		nextPump = currentPump->m_next;
		removePump(currentTank->m_tankID, currentPump->m_pumpID);
		currentPump = nextPump;
	}

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
	if (findTank(tankID) && findTank(targetTank)) {
		if (!findPump(getTank(tankID), pumpID)) {
			Tank* tank = getTank(tankID);

			if (tank->m_pumps == nullptr) {
				tank->m_pumps = new Pump(pumpID, targetTank);
				return true;
			}

			Pump* endPump = getEndPump(tank);
			endPump->m_next = new Pump(pumpID, targetTank);
			return true;
		}
	}

	return false;
}

/*
 * Function: removePump
 * --------------------
 * 
 */
bool FuelSys::removePump(int tankID, int pumpID) {
	Tank* targetTank = getTank(tankID);

	if (targetTank == nullptr || targetTank->m_pumps == nullptr) {
		return false;
	}

	Pump* currentPump = targetTank->m_pumps;
	Pump* previousPump = targetTank->m_pumps;

	while (currentPump != nullptr) {
		if (currentPump->m_pumpID == pumpID) {
			break;
		}
		previousPump = currentPump;
		currentPump = currentPump->m_next;
	}

	if (currentPump == nullptr) {
		return false;
	}

	if (previousPump->m_pumpID == pumpID) {
		targetTank->m_pumps = currentPump->m_next;
	}
	else {
		previousPump->m_next = currentPump->m_next;
	}

	delete currentPump;
	currentPump = nullptr;
	
	return true;
}

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
bool FuelSys::findPump(Tank* tank, int pumpID) {
	Pump* currentPump = tank->m_pumps;

	while (currentPump != nullptr) {
		if (currentPump->m_pumpID == pumpID) {
			return true;
		}

		currentPump = currentPump->m_next;
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
		currentPump = currentPump->m_next;
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

		currentTank = currentTank->m_next;
	}

	return nullptr;
}

/*
int FuelSys::totalFuel() const {

}
*/


/*
 * Function: dumpSys
 * -----------------
 * Outputs the list of tanks and their info
 */
void FuelSys::dumpSys() const {
	Tank* currentTank = m_current;

	cout << "Tank List:\n";

	while (currentTank != nullptr) {
		cout << "Tank: " << currentTank->m_tankID << " Capacity: " << currentTank->m_tankCapacity << "\n";
		dumpPumps(currentTank->m_pumps);
		currentTank = currentTank->m_next;
	}
}

/*
 * Function: dumpPumps
 * -------------------
 * pumps: The head of a tank's pump list
 * 
 * Outputs the list of pumps and their target tanks
 */
void FuelSys::dumpPumps(Pump* pumps) const {
	while (pumps != nullptr) {
		cout << "Pumps: " << pumps->m_pumpID << " Target Tank: " << pumps->m_target << "\n";
		pumps = pumps->m_next;
	}
}