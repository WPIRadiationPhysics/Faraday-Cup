#ifndef Analysis_h
#define Analysis_h 1

#include "globals.hh"
#include "g4root.hh"
#include "Analysis.hh"

class G4Analyze;

class Analysis {
  public:
    Analysis();
    ~Analysis();

    static Analysis* GetAnalysis();
    virtual void Analyze_Gain(G4int nThreads);
};

#endif
