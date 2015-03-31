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
    virtual void POST_Gain(G4int nThreads, G4int geo_i);
};

#endif
