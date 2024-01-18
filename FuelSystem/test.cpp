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
    bool addNormalTank(FuelSys & sys, Random & randCap, int numTanks) {
        bool result = true;

        for(int tankID = 0; tankID < numTanks; tankID++){
            result = sys.addTank(tankID, randCap.getRandNum());
        }

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

        for (int tankID = 0; tankID < numTanks; tankID++) {
            if (tankID % 2 == 0) {
                result = sys.removeTank(tankID);
            }
        }

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

        result = sys.findTank(0);
        result = sys.findTank(0);
        result = sys.findTank(4);
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

        for (int tankID = 0; tankID < numTanks; tankID++) {
            for (int pumpID = 0; pumpID < numPumps; pumpID++) {
                int targetID = (tankID + 1) % numTanks;
                result = sys.addPump(tankID, pumpID, targetID);
            }
        }

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

        for (int tankID = 0; tankID < numTanks; tankID++) {
            for (int pumpID = 0; pumpID < numPumps; pumpID++) {
                if (pumpID % 2 == 0) {
                    result = sys.removePump(tankID, pumpID);
                }
            }
        }

        sys.dumpSys();

        return result;
    }

};

int main() {
    Tester test;
    FuelSys sys;
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

    //Tests finding tanks
    if (test.findNormalTank(sys, numTanks)) {
        cout << "findNormalTank test returned successful\n";
    }
    else {
        cout << "findNormalTank test returned unsuccessful\n";
    }

    //Tests adding pumps
    if (test.addNormalPump(sys, numTanks, numPumps)) {
        cout << "addNormalPump test returned successsful\n";
    }
    else {
        cout << "addNormalPump test returned unsuccessful\n";
    }

    //Tests removing pumps
    if (test.removeNormalPump(sys, numTanks, numPumps)) {
        cout << "removeNormalPump test returned successful\n";
    }
    else {
        cout << "removeNormalPump test returned unsuccessful\n";
    }

    //Tests removing tanks
    if (test.removeNormalTank(sys, numTanks)) {
        cout << "removeNormalTank test returned successful\n";
    }
    else {
        cout << "removeNormalTank test returned unsuccessful\n";
    }

    return 0;
}