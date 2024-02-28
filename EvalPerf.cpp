#include "EvalPerf.hpp"
#include <chrono>
#include <iostream>


#include <x86intrin.h> // comptage des cycles

#include "typeinfo"

using namespace std;

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi)) ;
    return ((uint64_t)hi << 32) | lo ;
}



EvalPerf::EvalPerf(){

}

void EvalPerf::start(){

    perfStart = std::chrono::high_resolution_clock::now();
    cycleStart = rdtsc();

}

void EvalPerf::stop(){
    
    perfEnd = std::chrono::high_resolution_clock::now();
    cycleEnd = rdtsc();
}

long long EvalPerf::nbCycles(){

    return (cycleEnd-cycleStart) ;
}

double EvalPerf::get_seconds(){
    std::chrono::duration<double> elapsed_seconds = perfEnd-perfStart ;

    return elapsed_seconds.count() ;
}

double EvalPerf::get_seconds2(){
    std::chrono::duration<double> elapsed_seconds = std::chrono::high_resolution_clock::now()-perfStart ;

    return elapsed_seconds.count() ;
}

double EvalPerf::get_milliseconds(){
    std::chrono::duration<double> elapsed_seconds = perfEnd-perfStart ;
    double seconds=elapsed_seconds.count();
    return seconds*1000;
}

float EvalPerf::CPI(double N){
    return(EvalPerf::nbCycles()/N);
}

float EvalPerf::IPC(double N){
    return(1/EvalPerf::CPI(N));
}