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
	if (findTank(targetTank) && findTank(tankID)) {
		if (!findPump(m_current->m_next , pumpID)) {
			Tank* tank = m_current->m_next;

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
 * Function: fill
 * --------------
 * tankID:
 * fuel:
 * 
 * Adds 
 * 
 * return: true if some amount of fuel was added to the tank, false if the tank could be found or it's full
 */
bool FuelSys::fill(int tankID, int fuel) {
	if (findTank(tankID)) {
		Tank* fillTank = m_current->m_next;
		int neededFuel = fillTank->m_tankCapacity - fillTank->m_tankFuel;
		//Tank is full
		if (neededFuel != 0) {
			if (fuel > neededFuel) {
				fillTank->m_tankFuel += neededFuel;
			}
			else {
				fillTank->m_tankFuel += fuel;
			}

			return true;
		}
	}

	return false;
}

/*
 * Function: drain
 * ---------------
 * tankID:
 * pumpID:
 * fuel:
 * 
 * 
 * 
 * return:
 */
bool FuelSys::drain(int tankID, int pumpID, int fuel) {
	if (findTank(tankID)) {
		Tank* sourceTank = m_current->m_next;
		if (fuel > sourceTank->m_tankFuel) {
			fuel = sourceTank->m_tankFuel;
		}
		if (findPump(sourceTank, pumpID)) {
			Pump* sourcePump = getPump(sourceTank, pumpID);
			Tank* destinationTank = getTank(sourcePump->m_target);
			int neededFuel = destinationTank->m_tankCapacity - destinationTank->m_tankFuel;
			
			if (neededFuel != 0){
				if (fuel > neededFuel) {
					sourceTank->m_tankFuel -= neededFuel;
					return fill(destinationTank->m_tankID, neededFuel);
				}
				else {
					sourceTank->m_tankFuel -= fuel;
					return fill(destinationTank->m_tankID, fuel);
				}
			}
		}
	}

	return false;
}

/*
 * Function: findTank
 * ------------------
 * tankID: ID of the target tank
 *
 * Searches the list of tank to find the target ID. If found, sets it to the next of the current tank.
 * 
 * return: true if the tank is found, false otherwise
 */
bool FuelSys::findTank(int tankID) {
	Tank* currentTank = m_current;
	Tank* previousTank = m_current;

	while (currentTank != nullptr) {
		if (currentTank->m_tankID == tankID) {
			if (m_current->m_tankID == tankID) {
				Tank* temp = m_current->m_next;
				m_current->m_next = temp->m_next;
				temp->m_next = m_current;
				m_current = temp;
			}
			else if (m_current->m_next->m_tankID != tankID) {
				previousTank->m_next = currentTank->m_next;
				currentTank->m_next = m_current->m_next;
				m_current->m_next = currentTank;
			}
			return true;
		}

		previousTank = currentTank;
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
 * Function: getPump
 * -----------------
 * tank:
 * pumpID:
 * 
 * 
 * 
 * return:
 */
Pump* FuelSys::getPump(Tank* tank, int pumpID) {
	Pump* currentPump = tank->m_pumps;

	while (currentPump != nullptr) {
		if (currentPump->m_pumpID == pumpID) {
			return currentPump;
		}

		currentPump = currentPump->m_next;
	}

	return nullptr;
}

/*
 * Function: totalFuel
 * -------------------
 * Adds up the fuel from all the tanks in the system
 * 
 * return: The total fuel in the system
 */
int FuelSys::totalFuel() const {
	Tank* currentTank = m_current;
	int totalFuel = 0;

	while (currentTank != nullptr) {
		totalFuel += currentTank->m_tankFuel;
		currentTank = currentTank->m_next;
	}

	return totalFuel;
}

/*
 * Function: dumpSys
 * -----------------
 * Outputs the list of tanks and their info
 */
void FuelSys::dumpSys() const {
	Tank* currentTank = m_current;

	cout << "Tank List:\n";

	while (currentTank != nullptr) {
		cout << "Tank: " << currentTank->m_tankID << " Capacity: " << currentTank->m_tankCapacity << " Current Fuel: " << currentTank->m_tankFuel << "\n";
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