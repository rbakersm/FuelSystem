#include "fuel.h"

FuelSys::FuelSys() {
	m_current = nullptr;
}

FuelSys::~FuelSys() {
	while (m_current != nullptr) {
		removeTank(m_current->m_tankID);
	}
}

/*
 * Function: operator=
 * -------------------
 * rhs: Existing Fuel System
 * 
 * Copy the tanks and pumps from an existing fuel system to this system
 */ 
const FuelSys& FuelSys::operator=(const FuelSys& rhs) {
	//If it's the same system, no change
	if (this == &rhs)
	{
		return *this;
	}

	//Clears this system
	while (m_current != nullptr) {
		removeTank(m_current->m_tankID);
	}

	Tank* currentCopyTank = rhs.m_current;

	//Creates tanks in this system using existing IDs, capacities, and current fuel
	while (currentCopyTank != nullptr) {
		int tankID = currentCopyTank->m_tankID;
		int capacity = currentCopyTank->m_tankCapacity;
		addTank(tankID, capacity);

		Tank* currentTank = getTank(tankID);
		currentTank->m_tankFuel = currentCopyTank->m_tankFuel;

		Pump* currentCopyPump = currentCopyTank->m_pumps;

		//Creates pumps in respective tanks using existing IDs and targets
		while (currentCopyPump != nullptr) {
			int pumpID = currentCopyPump->m_pumpID;
			int targetID = currentCopyPump->m_target;

			if (currentTank->m_pumps == nullptr) {
				currentTank->m_pumps = new Pump(pumpID, targetID);
			}
			else {
				Pump* endPump = getEndPump(currentTank);
				endPump->m_next = new Pump(pumpID, targetID);
			}

			currentCopyPump = currentCopyPump->m_next;
		}

		currentCopyTank = currentCopyTank->m_next;
	}

	return *this;
}

/* Function: addTank
 * -----------------
 * tankID: Unique ID to assign new tank
 * capacity: How much fuel the tank can hold
 * tankFuel: Current amount of fuel in the tank at add
 *
 * Add a new tank to the list
 *
 * return: True if a tank was successfully created else false
 */
bool FuelSys::addTank(int tankID, int capacity) {
	if (tankID < 0 || capacity < MINCAP) {
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
 * tankID: Potential new tank ID
 *
 * Checks the list of existing tanks for duplicate ID
 *
 * return: Last tank in the list if no duplicate ID else nullptr
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
 * tankID: ID of the tank to remove
 * 
 * Removes target tank and each of its pumps
 * 
 * return: True if the tank is removed from the list, else false
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

	//Delete each of the pumps
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
 * tankID: Tank to add the pump to
 * pumpID: Unique ID to assign the new pump
 * targetID: Tank that the pump drains to
 * 
 * Add a pump to an existing tank
 * 
 * return: True if the pump was added, else false
 */
bool FuelSys::addPump(int tankID, int pumpID, int targetTank) {
	//Checks both the existing tank and the target are in the list
	if (findTank(targetTank) && findTank(tankID)) {
		Tank* currentTank = nullptr;
		//Checks that the ID does not exist
		if (m_current->m_next == nullptr) {
			currentTank = m_current;
		}
		else {
			currentTank = m_current->m_next;
		}
		if (!findPump(currentTank, pumpID)) {
			Tank* tank = currentTank;

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
 * tankID: Tank to remove the pump from
 * pumpID: Pump to be removed
 * 
 * Removes a pump from the tank it's attached to
 * 
 * return: True if the pump is removed, else false
 */
bool FuelSys::removePump(int tankID, int pumpID) {
	Tank* targetTank = getTank(tankID);

	if (targetTank == nullptr || targetTank->m_pumps == nullptr) {
		return false;
	}

	Pump* currentPump = targetTank->m_pumps;
	Pump* previousPump = targetTank->m_pumps;

	//Find the pump
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

	//Remove the gap from getting rid of the pump
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
 * tankID: Tank to add fuel to
 * fuel: Amount of fuel to add
 * 
 * Adds fuel to a tank
 * 
 * return: True if some amount of fuel was added to the tank, false if the tank could be found or it's full
 */
bool FuelSys::fill(int tankID, int fuel) {
	if (findTank(tankID)) {
		Tank* fillTank = nullptr;
		if (m_current->m_next == nullptr) {
			fillTank = m_current;
		}
		else {
			fillTank = m_current->m_next;
		}
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
 * tankID: Source tank to drain fuel from
 * pumpID: Pump used to transfer from source to destination tank
 * fuel: Amount of fuel to take from the source
 * 
 * Transfers fuel from a tank to the pump's target tank
 * 
 * return: True if fuel was transferred
 */
bool FuelSys::drain(int tankID, int pumpID, int fuel) {
	if (findTank(tankID)) {
		Tank* sourceTank = nullptr;

		if (m_current->m_next == nullptr) {
			sourceTank = m_current;
		}
		else {
			sourceTank = m_current->m_next;
		}
		//Decrease the amount of fuel if there is not enough in the tnak
		if (fuel > sourceTank->m_tankFuel) {
			fuel = sourceTank->m_tankFuel;
		}
		if (findPump(sourceTank, pumpID)) {
			Pump* sourcePump = getPump(sourceTank, pumpID);
			Tank* destinationTank = getTank(sourcePump->m_target);
			int neededFuel = destinationTank->m_tankCapacity - destinationTank->m_tankFuel;
			
			if (neededFuel != 0){
				//Decrease the amount of fuel if there is not enough space available
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
 * return: True if the tank is found, false otherwise
 */
bool FuelSys::findTank(int tankID) {
	Tank* currentTank = m_current;
	Tank* previousTank = m_current;

	while (currentTank != nullptr) {
		if (currentTank->m_tankID == tankID) {
			//If it's the first tank, swap it with the second if it exists
			if (m_current->m_tankID == tankID) {
				if (m_current->m_next != nullptr) {
					Tank* temp = m_current->m_next;
					m_current->m_next = temp->m_next;
					temp->m_next = m_current;
					m_current = temp;
				}
			}
			//If the tank is past the second tank, set the previous tank's next to this tanks next
			//Set this tank's next to the third tank and set the first tank's next to this
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
 * return: Last pump in the list
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
 * tank: Tank to get the pump from
 * pumpID: ID of the pump to get
 * 
 * Search the tank's pump list for the pump ID
 * 
 * return: Pump object if found in the tank, else null
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

	cout << "Total Fuel: " << totalFuel() << "\n";
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