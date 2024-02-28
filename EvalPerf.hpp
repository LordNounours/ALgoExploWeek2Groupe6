#ifndef EVALPERF_H
#define EVALPERF_H

#include "iostream"
#include "chrono"

#include <x86intrin.h> // comptage des cycles

using namespace std;

class EvalPerf
{
    private :

    public :

    // Constructeur
        EvalPerf();

        void start();
        void stop();
        long long nbCycles();
        
        double get_seconds();
        double get_seconds2();
        double get_milliseconds();
        float CPI(double N);
        float IPC(double N);


        uint64_t cycleStart, cycleEnd ;
        std::chrono::time_point<std::chrono::high_resolution_clock> perfStart, perfEnd;

        


    

};
#endif