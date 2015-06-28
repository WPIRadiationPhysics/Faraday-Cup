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
    void SetRunKAThickness(G4double runka_thickness) { runKA_thickness = runka_thickness; }
    void SetNThreads(G4int nthreads) { nThreads = nthreads; }
    void SetNEnergies(G4int nenergies) { nEnergies = nenergies; }
    G4int GetRunID() { return RunID; }
    G4double GetRunEnergy() { return runEnergy; }
    G4int GetRunKAThickness() { return runKA_thickness; }
    G4int GetNThreads() { return nThreads; }
    G4int GetNEnergies() { return nEnergies; }

    // Get/Set data directory
    G4String GetAnalysisDIR() { return analysisDIR; }
    void SetAnalysisDIR(G4String analysisdir) { analysisDIR = analysisdir; }

    // Nullify experiment parameters
    void nullExperiments() {
      runGain = 0;
      fMeasureGain = 0;
      analysisDIR = "data";
      runKA_thickness = 0;
      for ( G4int npro = 0; npro < 6; npro++ ){
      for ( G4int nneu = 0; nneu < 6; nneu++ ){
        runBranchingPN[npro][nneu] = 0; }}
    }

    // Get/Set experiment types
    void measureGain() { fMeasureGain = 1; }
    G4int isMeasureGain() { return fMeasureGain; }

    // Append/Recall energy and gain measurement
    void appendRunGain(G4double trackSignal) { runGain += trackSignal; }
    G4double recallRunGain() { return runGain; }

    // Append/Recall proton-neutron branching ratios
    void appendRunBranchingPN(G4int numprotons, G4int numneutrons) {
      if ( numprotons > 5 ) { numprotons = 5; }
      if ( numneutrons > 5 ) { numneutrons = 5; }
      runBranchingPN[numprotons][numneutrons] += 1;
    }
    G4double recallRunBranchingPN(G4int npro, G4int nneu)
      { return runBranchingPN[npro][nneu]; }

    // Analysis methods
    virtual void analyzeBranchingRatiosPN();
    virtual void analyzeCascadeTracks();

  private:

    // Data analysis directory
    G4String analysisDIR;

    // vars
    G4double runEnergy, runGain, runKA_thickness;
    G4int nThreads, nEnergies, RunID;

    // (p,Np,Mn) Branching ratios matrix
    G4int runBranchingPN[6][6]; // zero through 5+ foreach

    // Measurement functions
    void measureGainPreload();

    // Measurement toggles
    G4int fMeasureGain;
};

#endif
