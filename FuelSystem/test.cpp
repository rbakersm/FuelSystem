#include "fuel.h"
#include <random>

enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL };
class Random {
public:
    Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50, int stdev = 20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL) {
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean, stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min, max);
        }
        else {
            //the case of UNIFORMREAL to generate real number
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min, (double)max);
        }
    }
    void setSeed(int seedNum) {
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum() {
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if (m_type == NORMAL) {
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while (result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT) {
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum() {
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result * 100.0) / 100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester {
public:

    /*
     * Function: addNormalTank
     * -----------------------
     * sys: Fuel system object
     * randCap: Random capacity for each tank
     * numTanks: Number of tanks to add to the system
     * 
     * Adds tanks with valid ids and capacities to the fuel system
     * 
     * return: True if all tanks were added, false otherwise
     */
    bool addNormalTank(FuelSys& sys, Random & randCap, int numTanks) {
        bool result = true;

        for(int tankID = 1; tankID <= numTanks; tankID++){
            result = result && sys.addTank(tankID, randCap.getRandNum());
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: addErrorTank
     * ----------------------
     * sys: Fuel System object
     * numTanks: Number of invalid tanks
     * 
     * Attempts to add invalid tanks to the system
     * 
     * return: True if no tanks were added, false otherwise
     */
    bool addErrortank(FuelSys& sys, Random& randCap, int numTanks) {
        bool result = true;

        //Attempts to add tanks with valid capacities but invalid IDs
        for (int tankID = -1; tankID > -numTanks; tankID--) {
            result = result && !sys.addTank(tankID, randCap.getRandNum());
        }

        //Attemps to add tanks with valid IDs but invalid capacities
        for (int tankID = numTanks; tankID < numTanks * 2; tankID++) {
            result = result && !sys.addTank(tankID, MINCAP - 1);
        }

        //Attemps to add tanks with existing IDs
        for (int tankID = numTanks; tankID < numTanks; tankID++) {
            result = result && !sys.addTank(tankID, randCap.getRandNum());
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: addEdgeTank
     * ---------------------
     * sys: Fuel system object
     * randCap: Capacity for each tank
     * numTanks: Number of tanks in the system
     * 
     * Adds tanks with minimum ID and capacity
     * 
     * return: True if each tank was added, false otherwise
     */
    bool addEdgeTank(FuelSys& sys, Random& randCap, int numTanks) {
        bool result = true;

        result = result && sys.addTank(0, randCap.getRandNum());
        result = result && sys.addTank(numTanks + 1, MINCAP);

        sys.dumpSys();

        return result;
    }

    /*
     * Function: fillNormalTank
     * ------------------------
     * sys: Fuel system object
     * numTanks: Number of tanks in the system
     * 
     * Fill existing tanks in the list
     * 
     * return: True if each chosen tank is filled, false otherwise
     */
    bool fillNormalTank(FuelSys& sys, int numTanks) {
        bool result = true;

        for (int tankID = 1; tankID <= numTanks; tankID++) {
            if (tankID % 2 == 0) {
                //Fill partial then the rest
                result = result && sys.fill(tankID, MINCAP / 2);
                result = result && sys.fill(tankID, DEFCAP);
            }
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: fillErrorTank
     * -----------------------
     * sys: Fuel System object
     * 
     * Attempts to fill invalid tanks and fuel amounts
     * 
     * return: True if no tank was filled
     */
    bool fillErrorTank(FuelSys& sys, int numTanks) {
        bool result = true;

        //Attempt to fill valid tanks with invalid fuel amount
        for (int tankID = 1; tankID <= numTanks; tankID++) {
            result = result && !sys.fill(tankID, -1);
        }

        //Attemps to fill invalid tanks with valid fuel amount
        for (int tankID = -1; tankID > -numTanks; tankID--) {
            result = result && !sys.fill(tankID, MINCAP);
        }

        //Attemps to fill a full tank
        sys.fill(6, DEFCAP);
        result = result && !sys.fill(6, 0);
        result = result && !sys.fill(6, 1);

        sys.dumpSys();

        return result;
    }

    /*
     * Function: fillEdgeTank
     * ----------------------
     * sys: Fuel System object
     * 
     * Fills tanks to full and by none
     * 
     * return: True if each tank was filled to max or not at all, false otherwise
     */
    bool fillEdgeTank(FuelSys& sys) {
        bool result = true;

        result = result && sys.fill(0, DEFCAP); //Fill from 0 to max
        result = result && sys.fill(1, 0); //Fill by 0

        sys.dumpSys();

        return result;
    }

    /*
     * Function: drainNormalTank
     * -------------------------
     * sys: Fuel system object
     * numTanks: Number of tanks in the system
     * numPumps: Number of pumps in the system
     * 
     * Drain existing tanks in the list and fills the target tanks
     * 
     * return: True if each chosen tank is drained and the other tank filled, false otherwise
     */
    bool drainNormalTank(FuelSys& sys, int numTanks, int numPumps) 
    {
        bool result = true;

        for (int tankID = 1; tankID <= numTanks; tankID++) {
            if (tankID % 2 == 0) {
                for (int pumpID = 1; pumpID <= numPumps; pumpID++) {
                    result = result && sys.drain(tankID, pumpID, MINCAP / numPumps);
                }
            }
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: drainErrorTank
     * ------------------------
     * sys: Fuel system object
     * numTanks: Tanks in the system
     * numPumps: Pumps in the system
     * 
     * Attempts to drain tanks with invalid IDs, pumps, and fuel ammounts
     * 
     * return: True if no fuel is drained, false otherwise
     */
    bool drainErrorTank(FuelSys& sys, int numTanks, int numPumps) {
        bool result = true;

        //Attempt to drain valid fuel from valid tanks with invalid pumps 
        for (int tankID = 1; tankID <= numTanks; tankID++) {
            if (tankID % 2 == 0) {
                for (int pumpID = -1; pumpID > numPumps; pumpID--) {
                    result = result && !sys.drain(tankID, pumpID, 1);
                }
            }
        }

        //Attempt to drain invalid fuel from valid tanks with valid pumps
        for (int tankID = 1; tankID <= numTanks; tankID++) {
            if (tankID % 2 == 0) {
                for (int pumpID = 1; pumpID <= numPumps; pumpID++) {
                    result = result && !sys.drain(tankID, pumpID, -1);
                }
            }
        }

        //Attempt to drain valid fuel from invalid tanks with valid pumps
        for (int tankID = -1; tankID > numTanks; tankID--) {
            if (-tankID % 2 == 0) {
                for (int pumpID = 1; pumpID <= numPumps; pumpID++) {
                    result = result && !sys.drain(tankID, pumpID, 1);
                }
            }
        }

        //Attempt to drain fuel to a full tank
        sys.fill(8, DEFCAP);
        result = result && !sys.drain(7, 2, DEFCAP);

        sys.dumpSys();

        return result;
    }

    /*
     * Function: drainEdgeTank
     * -----------------------
     * sys: Fuel System object
     * 
     * return: True if all fuel was transfered from one tank to another, false otherwise
     */
    bool drainEdgeTank(FuelSys& sys) {
        bool result = true;

        //Drain all fuel from source and fill an empty destination completely
        sys.addPump(10, 7, 11);
        sys.fill(10, DEFCAP);
        result = result && sys.drain(10, 7, DEFCAP);

        sys.dumpSys();

        return result;
    }

    /*
     * Function: removeNormalTank
     * --------------------------
     * sys: Fuel system object
     * numTanks: Number of tanks in the system
     * 
     * Remove existing tanks from the list
     * 
     * return: True if each tank chosen was removed, false otherwise
     */
    bool removeNormalTank(FuelSys& sys, int numTanks) {
        bool result = true;

        for (int tankID = 1; tankID <= numTanks; tankID++) {
            if (tankID % 2 == 0) {
                result = result && sys.removeTank(tankID);
            }
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: removeErrorTank
     * -------------------------
     * sys: Fuel System object
     * numTanks: Number of tanks in the system
     * 
     * Attempt to remove invalid tanks
     * 
     * return: True if no tanks were removed, false otherwise
     */
    bool removeErrorTank(FuelSys& sys, int numTanks) {
        bool result = true;

        //Attempt to remove a non existing tank
        for (int tankID = 1; tankID <= numTanks; tankID++) {
            if (tankID % 2 == 0) {
                result = result && !sys.removeTank(tankID);
            }
        }

        //Attempt to remove invalid tank ids
        for (int tankID = -1; tankID > numTanks; tankID--) {
            result = result && !sys.removeTank(tankID);
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: removeEdgeTank
     * ------------------------
     * sys: Fuel System object
     * 
     * Removes the first and last tanks in the system
     * 
     * return: True if the ends were removed
     */
    bool removeEdgeTank(FuelSys& sys) {
        bool result = true;

        result = result && sys.removeTank(7);
        result = result && sys.removeTank(0);

        sys.dumpSys();

        return result;
    }

    /*
     * Function: findNormalTank
     * ------------------------
     * sys: Fuel system object
     * numTanks: Number of tanks in the system
     * 
     * Finds existing tanks from the list
     * 
     * return: True if existing tanks are found and moved, false otherwise
     */
    bool findNormalTank(FuelSys& sys, int numTanks) {
        bool result = true;

        result = result && sys.findTank(0);
        result = result && sys.findTank(0);
        result = result && sys.findTank(4);
        sys.dumpSys();

        return result;
    }

    /*
     * Function: findErrorTank
     * -----------------------
     * sys: Fuel system object
     * numTanks: Number of invalid tanks
     * 
     * Attempts to find invalid tanks
     * 
     * return: True if no tank was found, false otherwise
     */
    bool findErrorTank(FuelSys& sys, int numTanks) {
        bool result = true;

        for (int tankID = -1; tankID < -numTanks; tankID++) {
            result = result && sys.findTank(tankID);
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: findEdgeTank
     * ----------------------
     * sys: Fuel system with one tank
     * 
     * The tank found should stay in the first position
     * 
     * return: True if the tank is found and is the head of the list, false otherwise
     */
    bool findEdgeTank(FuelSys& sys) {
        bool result = true;

        result = sys.findTank(1);

        sys.dumpSys();

        return result;
    }

    /*
     * Function: addNormalPump
     * -----------------------
     * sys: Fuel system object
     * numTanks: Number of tanks in the system
     * numPumps: Number of pumps to add to each tank
     * 
     * Adds pumps with valid ids and targets to each tank
     * 
     * return: True if every pump was added, false otherwise
     */
    bool addNormalPump(FuelSys& sys, int numTanks, int numPumps) {
        bool result = true;

        for (int tankID = 1; tankID <= numTanks; tankID++) {
            for (int pumpID = 1; pumpID <= numPumps; pumpID++) {
                int targetID = (tankID + 1) % numTanks;
                result = result && sys.addPump(tankID, pumpID, targetID);
            }
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: addErrorPump
     * ----------------------
     * sys: Fuel system object
     * numTanks: Number of tanks in the system
     * numPumps: Number of pumps to add to the system
     * 
     * Attempts to add invalid pumps to valid tanks and valid pumps to invalid tanks
     * 
     * return: True if no pumps were added, false otherwise
     */
    bool addErrorPump(FuelSys& sys, int numTanks, int numPumps) {
        bool result = true;

        //Attempt to add invalid pumps to valid tanks with valid target IDs
        for (int tankID = 1; tankID <= numTanks; tankID++) {
            for (int pumpID = -1; pumpID > -numPumps; pumpID--) {
                int targetID = (tankID + 1) % numTanks;
                result = result && !sys.addPump(tankID, pumpID, targetID);
            }
        }

        //Attempt to add valid pumps to valid tanks with invalid target IDs
        for (int tankID = 1; tankID <= numTanks; tankID++) {
            for (int pumpID = 1; pumpID <= numPumps; pumpID++) {
                int targetID = -((tankID + 1) % numTanks);
                result = result && !sys.addPump(tankID, pumpID, targetID);
            }
        }

        //Attempt to add valid pumps to invalid tanks with valid target IDs
        for (int tankID = -1; tankID > numTanks; tankID--) {
            for (int pumpID = 1; pumpID <= numPumps; pumpID++) {
                int targetID = (-tankID + 1) % numTanks;
                result = result && !sys.addPump(tankID, pumpID, targetID);
            }
        }

        result = result && !sys.addPump(1, 6, 1); //Attempt to add a new pump to a tank with the target as itself

        sys.dumpSys();

        return result;
    }

    /*
     * Function: addEdgePump
     * ---------------------
     * sys: Fuel System object
     * 
     * Add a pump with the min pump ID
     * 
     * return: True if pump was added, false otherwise
     */
    bool addEdgePump(FuelSys& sys) {
        bool result = true;

        result = result && sys.addPump(0, 0, 1);

        sys.dumpSys();

        return result;
    }

    /*
     * Function: removeNormalPump
     * --------------------------
     * sys: Fuel system object
     * numTanks: Number of tanks in the system
     * numPumps: Number of pumps in the system
     * 
     * Remove existing pumps from the list of tanks
     * 
     * return: True if each chosen pump is removed, false otherwise
     */
    bool removeNormalPump(FuelSys& sys, int numTanks, int numPumps) {
        bool result = true;

        for (int tankID = 1; tankID <= numTanks; tankID++) {
            for (int pumpID = 1; pumpID <= numPumps; pumpID++) {
                if (pumpID % 2 == 0) {
                    result = result && sys.removePump(tankID, pumpID);
                }
            }
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: removeErrorPump
     * -------------------------
     * sys: Fuel system object
     * numTanks: Number of tanks in the system
     * numPumps: Number of pumps in the system
     * 
     * Attempt to remove invalid pumps
     * 
     * return: True if no pumps were removed, false otherwise
     */
    bool removeErrorPump(FuelSys& sys, int numTanks, int numPumps) {
        bool result = true;

        //Attempts to remove an invalid pump from a valid tank
        for (int tankID = 1; tankID <= numTanks; tankID++) {
            for (int pumpID = -1; pumpID > -numPumps; pumpID--) {
                result = result && !sys.removePump(tankID, pumpID);
            }
        }

        //Attempts to remove a valid pump from an invalid tank
        for (int tankID = -1; tankID > numTanks; tankID--) {
            for (int pumpID = 1; pumpID <= numPumps; pumpID++) {
                result = result && !sys.removePump(tankID, pumpID);
            }
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: removeEdgePump
     * ------------------------
     * sys: Fuel system object
     * numTanks: Number of tanks in the system
     * 
     * Remove the first and last pumps in each tank
     * 
     * return: True if the pumps were removed
     */
    bool removeEdgePump(FuelSys& sys, int numTanks) {
        bool result = true;

        for (int tankID = 1; tankID <= numTanks; tankID++) {
            cout << tankID << "\n";
            result = result && sys.removePump(tankID, 1);
            result = result && sys.removePump(tankID, 5);
        }

        sys.dumpSys();

        return result;
    }

    /*
     * Function: copyNormalSys
     * ------------------------
     * source: Fuel system to copy tanks from
     * destination: Fuel system to copy tanks to
     * numTanks: Number of tanks in source system
     * 
     * return: True if the source system was completely copied
     */
    bool copyNormalSys(FuelSys& source, FuelSys& destination, int numTanks) {
        bool result = true;

        destination = source;

        for (int tankID = 1; tankID <= numTanks; tankID++) {
            result = result && source.findTank(tankID);
            result = result && destination.findTank(tankID);
        }

        destination.dumpSys();

        return result;
    }

    /*
     * Function: copyEdgeSys
     * ---------------------
     * source: Fuel system to copy tanks to and from
     * numTanks: Number of tanks in source system
     *
     * Copy a system to itself and copy an empty system to a non empty system
     * 
     * return: True if no changes were made to the first assignment and if the second assignment becomes empty, false otherwise
     */
    bool copyEdgeSys(FuelSys& source, int numTanks) {
        bool result = true;

        source = source;

        for (int tankID = 0; tankID <= numTanks + 1; tankID++) {
            result = result && source.findTank(tankID);
        }

        FuelSys emptySys;

        source = emptySys;

        for (int tankID = 0; tankID <= numTanks + 1; tankID++) {
            result = result && !source.findTank(tankID);
        }

        source.dumpSys();

        return result;
    }
};

int main() {
    Tester test;
    FuelSys sys;
    FuelSys singleSys; //System with one tank
    Random randCap(MINCAP, DEFCAP); //Range for valid test capacities
    int numTanks = 10; //Number of tanks in the system
    int numPumps = 5; //Number of pumps in the system

    //Tests adding tanks
    if (test.addNormalTank(sys, randCap, numTanks)) {
        cout << "addNormalTank test returned successful\n";
    }
    else {
        cout << "addNormalTank test returned unsuccessful\n";
    }

    if (test.addErrortank(sys, randCap, numTanks)) {
        cout << "addErrorTank test returned successful\n";
    }
    else {
        cout << "addErrorTank test returned unsuccessul\n";
    }

    if (test.addEdgeTank(sys, randCap, numTanks)) {
        cout << "addEdgeTank test returned successful\n";
    }
    else {
        cout << "addEdgeTank test returned unsuccessful\n";
    }

    //Tests finding tanks
    if (test.findNormalTank(sys, numTanks)) {
        cout << "findNormalTank test returned successful\n";
    }
    else {
        cout << "findNormalTank test returned unsuccessful\n";
    }

    if (test.findErrorTank(sys, numTanks)) {
        cout << "findErrorTank test returned successful\n";
    }
    else {
        cout << "findErrorTank test returned unsuccessful\n";
    }

    singleSys.addTank(1, MINCAP);

    if (test.findEdgeTank(singleSys)) {
        cout << "findEdgeTank test returned successful\n";
    }
    else {
        cout << "findEdgeTank test returned unsuccessful\n";
    }

    //Tests adding pumps
    if (test.addNormalPump(sys, numTanks, numPumps)) {
        cout << "addNormalPump test returned successsful\n";
    }
    else {
        cout << "addNormalPump test returned unsuccessful\n";
    }

    if (test.addErrorPump(sys, numTanks, numPumps)) {
        cout << "addErrorPump test returned successful\n";
    }
    else {
        cout << "addErrorPump test returned unsuccessful\n";
    }

    if (test.addEdgePump(sys)) {
        cout << "addEdgePump test returned successful\n";
    }
    else {
        cout << "addEdgePump test returned unsuccessful\n";
    }

    //Tests filling tanks
    if (test.fillNormalTank(sys, numTanks)) {
        cout << "fillNormalTank test returned successful\n";
    }
    else {
        cout << "fillNormalTank test returned unsuccesful\n";
    }

    if (test.fillErrorTank(sys, numTanks)) {
        cout << "fillErrorTank test returned successful\n";
    }
    else {
        cout << "fillErrorTank test returned unsuccessful\n";
    }

    if (test.fillEdgeTank(sys)) {
        cout << "fillEdgeTank test returned successful\n";
    }
    else {
        cout << "fillEdgeTank test returned unsuccessful\n";
    }

    //Tests draining tanks
    if (test.drainNormalTank(sys, numTanks, numPumps)) {
        cout << "drainNormalTank test returned successful\n";
    }
    else {
        cout << "drainNormalTank test returned unsuccessful\n";
    }

    if (test.drainErrorTank(sys, numTanks, numPumps)) {
        cout << "drainErrorTank test returned successful\n";
    }
    else {
        cout << "drainErrorTank test returned unsuccessful\n";
    }

    if (test.drainEdgeTank(sys)) {
        cout << "drainEdgeTank test returned successful\n";
    }
    else {
        cout << "drainEdgeTank test returned unsuccessful\n";
    }

    //Tests copying fuel systems
    if (test.copyNormalSys(sys, singleSys, numTanks)) {
        cout << "copyNormalSys test returned successful\n";
    }
    else {
        cout << "copyNormalSys test returned unsuccessful\n";
    }

    if (test.copyEdgeSys(singleSys, numTanks)) {
        cout << "copyEdgeSys test returned successful\n";
    }
    else {
        cout << "copyEdgeSys test returned unsuccessful\n";
    }

    //Tests removing pumps
    if (test.removeNormalPump(sys, numTanks, numPumps)) {
        cout << "removeNormalPump test returned successful\n";
    }
    else {
        cout << "removeNormalPump test returned unsuccessful\n";
    }

    if (test.removeErrorPump(sys, numTanks, numPumps)) {
        cout << "removeErrorPump test returned successful\n";
    }
    else {
        cout << "removeErrorPump test returned unsuccessful\n";
    }

    if (test.removeEdgePump(sys, numTanks)) {
        cout << "removeEdgePump test returned successful\n";
    }
    else {
        cout << "removeEdgePump test returned unsuccessful\n";
    }

    //Tests removing tanks
    if (test.removeNormalTank(sys, numTanks)) {
        cout << "removeNormalTank test returned successful\n";
    }
    else {
        cout << "removeNormalTank test returned unsuccessful\n";
    }

    if (test.removeErrorTank(sys, numTanks)) {
        cout << "removeErrorTank test returned successful\n";
    }
    else {
        cout << "removeErrorTank test returned unsuccessful\n";
    }

    if (test.removeEdgeTank(sys)) {
        cout << "removeEdgeTank test returned successful\n";
    }
    else {
        cout << "removeEdgeTank test returned unsuccessful\n";
    }

    return 0;
}