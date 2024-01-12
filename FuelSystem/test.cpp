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
    
    void displayTanks(FuelSys & sys) {
        Tank* displayTank = sys.m_current;

        cout << "Tank List\n";

        while (displayTank != nullptr) {
            cout << "Tank: " << displayTank->m_tankID + 1 << " Capacity: " << displayTank->m_tankCapacity << "\n";
            Pump* displayPump = displayTank->m_pumps;
            while (displayPump != nullptr) {
                cout << "Pump: " << displayPump->m_pumpID + 1 << " Target Tank: " << displayPump->m_target << "\n";
                displayPump = displayPump->m_next;
            }
            displayTank = displayTank->m_next;
        }
    }

    bool addNormalTank(FuelSys & sys, Random & randCap, int numTanks) {
        bool result = true;

        for(int tankID = 0; tankID < numTanks; tankID++){
            result = sys.addTank(tankID, randCap.getRandNum());
        }

        displayTanks(sys);

        return result;
    }

    bool removeNormalTank(FuelSys& sys, int numTanks) {
        bool result = true;

        for (int tankID = 0; tankID < numTanks; tankID++) {
            result = sys.removeTank(tankID);
        }

        displayTanks(sys);

        return result;
    }

    bool addNormalPump(FuelSys& sys, int numTanks, int numPumps) {
        bool result = true;

        for (int tankID = 0; tankID < numTanks; tankID++) {
            for (int pumpID = 0; pumpID < numPumps; pumpID++) {
                int targetID = (tankID + 1) % numTanks;
                result = sys.addPump(tankID, pumpID, targetID);
            }
        }

        displayTanks(sys);

        return result;
    }

    bool removeNormalPump(FuelSys& sys, int numTanks, int numPumps) {
        bool result = true;

        for (int tankID = 0; tankID < numTanks; tankID++) {
            for (int pumpID = 0; pumpID < numPumps; pumpID++) {
                if (pumpID % 2 == 0) {
                    result = sys.removePump(tankID, pumpID);
                }
            }
        }

        displayTanks(sys);

        return result;
    }

};

int main() {
    Tester test;
    FuelSys sys;
    Random randCap(MINCAP, DEFCAP);
    int numTanks = 10;
    int numPumps = 5;

    if (test.addNormalTank(sys, randCap, numTanks)) {
        cout << "addNormalTank test returned successful\n";
    }
    else {
        cout << "addNormalTank test returned unsuccessful\n";
    }

    if (test.addNormalPump(sys, numTanks, numPumps)) {
        cout << "addNormalPump test returned successsful\n";
    }
    else {
        cout << "addNormalPump test returned unsuccessful\n";
    }

    if (test.removeNormalPump(sys, numTanks, numPumps)) {
        cout << "removeNormalPump test returned successful\n";
    }
    else {
        cout << "removeNormalPump test returned unsuccessful\n";
    }

    if (test.removeNormalTank(sys, numTanks)) {
        cout << "removeNormalTank test returned successful\n";
    }
    else {
        cout << "removeNormalTank test returned unsuccessful\n";
    }

    return 0;
}