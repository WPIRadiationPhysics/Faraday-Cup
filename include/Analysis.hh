#ifndef Analysis_h
#define Analysis_h 1

#include "globals.hh"
#include "g4root.hh"
#include "Analysis.hh"

class Analysis {

  public:

    Analysis();
    virtual ~Analysis();

    // Acquire instance
    static Analysis* GetAnalysis()
    { static Analysis the_analysis; return &the_analysis; }

    // Nullify experiment parameters
    void nullExperiments() { fMeasureGain = 0; fMeasureKAAxialCharge = 0; }

    // Get/Set experiment types
    void measureGain() { fMeasureGain = 1; }
    void measureKAAxialCharge() { fMeasureKAAxialCharge = 1; }
    G4int isMeasureGain() { return fMeasureGain; }
    G4int isMeasureKAAxialCharge() { return fMeasureKAAxialCharge; }

    // Locate track files and read through
    virtual void analyzeTracks(G4int nThreads, G4int nEnergies);

  private:

    // Measurement functions
    void measureGainPreload();
    void measureKAAxialChargePreload(G4int nEnergies);

    G4int fMeasureGain;
    G4int fMeasureKAAxialCharge;
};

#endif
