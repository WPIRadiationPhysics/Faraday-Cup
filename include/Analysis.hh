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

    // Get/Set global vars (modelID, runEnergy, RunID, nThreads, nEnergies, data_dir)
    void SetRunID(G4int runid) { RunID = runid; }
    G4int GetRunID() { return RunID; }
    //
    void SetRunEnergy(G4double runenergy) { runEnergy = runenergy; }
    G4double GetRunEnergy() { return runEnergy; }
    //
    void SetRunKAThickness(G4double runka_thickness) { runKA_thickness = runka_thickness; }
    G4int GetRunKAThickness() { return runKA_thickness; }
    //
    void SetNThreads(G4int nthreads) { nThreads = nthreads; }
    G4int GetNThreads() { return nThreads; }
    //
    void SetNEnergies(G4int nenergies) { nEnergies = nenergies; }
    G4int GetNEnergies() { return nEnergies; }
    //
    G4String GetAnalysisDIR() { return analysisDIR; }
    void SetAnalysisDIR(G4String analysisdir) { analysisDIR = analysisdir; }

    // Nullify experiment parameters
    void nullExperiments() {

      analysisDIR = "data";
      runKA_thickness = 0;

      runGain = 0;
      for ( G4int npro = 0; npro < 6; npro++ ) {
      for ( G4int nneu = 0; nneu < 6; nneu++ ) {
        runBranchingPN[npro][nneu] = 0; }}
      for ( G4int binr = 0; binr < 100; binr++ ) {
      for ( G4int binz = 0; binz < 100; binz++ ) {
        eE_r[binr][binz] = 0, eE_z[binr][binz] = 0,
        pE_r[binr][binz] = 0, pE_z[binr][binz] = 0,
        oE_r[binr][binz] = 0, oE_z[binr][binz] = 0,
        nE_r[binr][binz] = 0, nE_z[binr][binz] = 0,
        gE_r[binr][binz] = 0, gE_z[binr][binz] = 0;
      }}
    }

    // Append/Recall energy and gain measurement
    void appendRunGain(G4double trackSignal) { runGain += trackSignal; }
    G4double recallRunGain() { return runGain; }

    // Append/Recall Energy Deposition data
    void appendECascade(G4int binr, G4int binz, G4double e_r, G4double e_z) { eE_r[binr][binz] += e_r, eE_z[binr][binz] += e_z; }
    void appendPCascade(G4int binr, G4int binz, G4double e_r, G4double e_z) { pE_r[binr][binz] += e_r, pE_z[binr][binz] += e_z; }
    void appendOCascade(G4int binr, G4int binz, G4double e_r, G4double e_z) { oE_r[binr][binz] += e_r, oE_z[binr][binz] += e_z; }
    void appendNCascade(G4int binr, G4int binz, G4double e_r, G4double e_z) { nE_r[binr][binz] += e_r, nE_z[binr][binz] += e_z; }
    void appendGCascade(G4int binr, G4int binz, G4double e_r, G4double e_z) { gE_r[binr][binz] += e_r, gE_z[binr][binz] += e_z; }
    G4double recallECascade(G4int binr, G4int binz, G4int rorz)
      { if ( rorz == 1 ) { return eE_r[binr][binz]; } else { return eE_z[binr][binz]; } }
    G4double recallPCascade(G4int binr, G4int binz, G4int rorz)
      { if ( rorz == 1 ) { return pE_r[binr][binz]; } else { return pE_z[binr][binz]; } }
    G4double recallOCascade(G4int binr, G4int binz, G4int rorz)
      { if ( rorz == 1 ) { return oE_r[binr][binz]; } else { return oE_z[binr][binz]; } }
    G4double recallNCascade(G4int binr, G4int binz, G4int rorz)
      { if ( rorz == 1 ) { return nE_r[binr][binz]; } else { return nE_z[binr][binz]; } }
    G4double recallGCascade(G4int binr, G4int binz, G4int rorz)
      { if ( rorz == 1 ) { return gE_r[binr][binz]; } else { return gE_z[binr][binz]; } }

    // Append/Recall proton-neutron branching ratios
    void appendRunBranchingPN(G4int numprotons, G4int numneutrons) {
      if ( numprotons > 5 ) { numprotons = 5; }
      if ( numneutrons > 5 ) { numneutrons = 5; }
      runBranchingPN[numprotons][numneutrons] += 1;
    }
    G4double recallRunBranchingPN(G4int npro, G4int nneu)
      { return runBranchingPN[npro][nneu]; }

    // Analysis methods
    virtual void appendGainFile();
    virtual void ntupleizeGainFile();
    virtual void analyzeCascade();
    virtual void analyzeBranchingRatiosPN();

  private:

    // Data analysis directory
    G4String analysisDIR;

    // vars
    G4double runEnergy, runGain, runKA_thickness;
    G4int nThreads, nEnergies, RunID;

    // (p,NpMn) Branching ratios matrix
    G4int runBranchingPN[6][6]; // zero through 5+ foreach

    // Energy Deposition directional data
    G4double eE_r[100][100], eE_z[100][100],
             pE_r[100][100], pE_z[100][100],
             oE_r[100][100], oE_z[100][100],
             nE_r[100][100], nE_z[100][100],
             gE_r[100][100], gE_z[100][100];
};

#endif
