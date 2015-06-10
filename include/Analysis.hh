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
    void nullExperiments() { fMeasureGain = 0; fMeasureCuCharge = 0; fMeasureKACharge = 0; }

    // Get/Set experiment types
    void measureGain() { fMeasureGain = 1; }
    void measureCuCharge() { fMeasureCuCharge = 1; }
    void measureKACharge() { fMeasureKACharge = 1; }
    G4int isMeasureGain() { return fMeasureGain; }
    G4int isMeasureCuCharge() { return fMeasureCuCharge; }
    G4int isMeasureKACharge() { return fMeasureKACharge; }

    // Locate track files and read through
    virtual void analyzeTracks(G4int nThreads, G4int nEnergies);

  private:

    // Measurement functions
    void measureGainPreload();
    void measureCuChargePreload(G4int nEnergies);
    void measureKAChargePreload(G4int nEnergies);

    G4int fMeasureGain;
    G4int fMeasureCuCharge;
    G4int fMeasureKACharge;
};

#endif
