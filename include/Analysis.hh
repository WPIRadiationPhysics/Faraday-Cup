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

    // Get/Set global vars (modelID, runEnergy, RunID, nThreads, nEnergies)
    void SetRunID(G4int runid) { RunID = runid; }
    void SetRunEnergy(G4double runenergy) { runEnergy = runenergy; }
    void SetNThreads(G4int nthreads) { nThreads = nthreads; }
    void SetNEnergies(G4int nenergies) { nEnergies = nenergies; }
    G4int GetRunID() { return RunID; }
    G4double GetRunEnergy() { return runEnergy; }
    G4int GetNThreads() { return nThreads; }
    G4int GetNEnergies() { return nEnergies; }

    // Get/Set data directory
    G4String GetAnalysisDIR() { return analysisDIR; }
    void SetAnalysisDIR(G4String dir_i) { analysisDIR = dir_i; }

    // Nullify experiment parameters
    void nullExperiments() {
      runGain = 0;
      fMeasureGain = 0;
      fMeasureGainHistoCu = 0;
      fMeasureGainHistoKA = 0;
      analysisDIR = "data";
    }

    // Get/Set experiment types
    void measureGain() { fMeasureGain = 1; }
    void measureGainHistoCu() { fMeasureGainHistoCu = 1; }
    void measureGainHistoKA() { fMeasureGainHistoKA = 1; }
    G4int isMeasureGain() { return fMeasureGain; }
    G4int isMeasureGainHistoCu() { return fMeasureGainHistoCu; }
    G4int isMeasureGainHistoKA() { return fMeasureGainHistoKA; }

    // Append/Recall energy and gain measurement
    void appendRunGain(G4double trackSignal) { runGain += trackSignal; }
    G4double recallRunGain() { return runGain; }

    // Locate gain track files and read through
    virtual void analyzeGainTracks();

    // Locate cascade track files and read through
    virtual void analyzeCascadeTracks();

  private:

    // Data analysis directory
    G4String analysisDIR;

    // vars
    G4double runEnergy, runGain;
    G4int nThreads, nEnergies, RunID;

    // Measurement functions
    void measureGainPreload();
    void measureGainHistoCuPreload();
    void measureGainHistoKAPreload();

    // Measurement toggles
    G4int fMeasureGain;
    G4int fMeasureGainHistoCu;
    G4int fMeasureGainHistoKA;
};

#endif
