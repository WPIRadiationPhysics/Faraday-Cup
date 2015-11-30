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

    // Get/Set global vars
    // (runid)
    void SetRunID(G4int runid) { RunID = runid; }
    G4int GetRunID() { return RunID; }
    // (runEnergy)
    void SetRunEnergy(G4double runenergy) { runEnergy = runenergy; }
    G4double GetRunEnergy() { return runEnergy; }
    // (runBeamFWHM)
    void SetRunBeamFWHM(G4double runfwhm) { runBeamFWHM = runfwhm; }
    G4double GetRunBeamFWHM() { return runBeamFWHM; }
    // (runKA_thickness)
    void SetRunKAThickness(G4double runka_thickness) { runKA_thickness = runka_thickness; }
    G4int GetRunKAThickness() { return runKA_thickness; }
    // (nThreads)
    void SetNThreads(G4int nthreads) { nThreads = nthreads; }
    G4int GetNThreads() { return nThreads; }
    // (nEnergies)
    void SetNEnergies(G4int nenergies) { nEnergies = nenergies; }
    G4int GetNEnergies() { return nEnergies; }
    // (analysisDIR)
    G4String GetAnalysisDIR() { return analysisDIR; }
    void SetAnalysisDIR(G4String analysisdir) { analysisDIR = analysisdir; }

    // Nullify experimental parameters
    void resetExperiment() {

      // Gain Measurement
      runGain = 0;
      measuregain = false;

      // Profile gain/loss, gain/loss-square and gain/loss entries histograms
      for ( G4int nprofilez = 0; nprofilez < 100; nprofilez++ ) {
      for ( G4int nprofiler = 0; nprofiler < 30; nprofiler++ ) {
        gainprofilecu[nprofilez][nprofiler] = 0;
        gainsquareprofilecu[nprofilez][nprofiler] = 0;
        gainentriesprofilecu[nprofilez][nprofiler] = 0;
        lossprofilecu[nprofilez][nprofiler] = 0;
        losssquareprofilecu[nprofilez][nprofiler] = 0;
        lossentriesprofilecu[nprofilez][nprofiler] = 0;
      }}
      for ( G4int nprofilez = 0; nprofilez < 100; nprofilez++ ) {
      for ( G4int nprofiler = 0; nprofiler < 31; nprofiler++ ) {
        gainprofileka[nprofilez][nprofiler] = 0;
        gainsquareprofileka[nprofilez][nprofiler] = 0;
        gainentriesprofileka[nprofilez][nprofiler] = 0;
      }}

      // Branching ratios
      for ( G4int npro = 0; npro < 7; npro++ ) {
      for ( G4int nneu = 0; nneu < 7; nneu++ ) {
        runBranchingPN[npro][nneu] = 0;
      }}

      // Cascade histogram
      for ( G4int binr = 0; binr < 100; binr++ ) {
      for ( G4int binz = 0; binz < 100; binz++ ) {
        eE_r[binr][binz] = 0, eE_z[binr][binz] = 0,
        pE_r[binr][binz] = 0, pE_z[binr][binz] = 0,
        oE_r[binr][binz] = 0, oE_z[binr][binz] = 0,
        nE_r[binr][binz] = 0, nE_z[binr][binz] = 0,
        gE_r[binr][binz] = 0, gE_z[binr][binz] = 0;
      }}
    }

    // Gain/Loss Measurement
    // Measurement boolean
    void measureGain() { measuregain = true; }
    G4bool isMeasureGain() { return measuregain; }
    //
    // Append/Recall gain measurement
    void appendRunGain(G4double trackSignal) { runGain += trackSignal; }
    G4double recallRunGain() { return runGain; }
    //
    // Append Cu, Kapton gain/loss, gain/loss-square and gain/loss error profile values
    void appendGainProfileCu(G4double gainz, G4double gainr, G4double signal) { gainprofilecu[(G4int)gainz][(G4int)gainr] += signal; }
    void appendGainSquareProfileCu(G4double gainz, G4double gainr, G4double signal) { gainsquareprofilecu[(G4int)gainz][(G4int)gainr] += signal; }
    void appendGainEntriesProfileCu(G4double gainz, G4double gainr) { gainentriesprofilecu[(G4int)floor(gainz)][(G4int)gainr]++; }
    void appendLossProfileCu(G4double gainz, G4double gainr, G4double signal) { lossprofilecu[(G4int)gainz][(G4int)gainr] += signal; }
    void appendLossSquareProfileCu(G4double gainz, G4double gainr, G4double signal) { losssquareprofilecu[(G4int)gainz][(G4int)gainr] += signal; }
    void appendLossEntriesProfileCu(G4double gainz, G4double gainr) { lossentriesprofilecu[(G4int)floor(gainz)][(G4int)gainr]++; }
    //
    void appendGainProfileKA(G4double gainz, G4double gainr, G4double signal) { gainprofileka[(G4int)gainz][(G4int)gainr] += signal; }
    void appendGainSquareProfileKA(G4double gainz, G4double gainr, G4double signal) { gainsquareprofileka[(G4int)gainz][(G4int)gainr] += signal; }
    void appendGainEntriesProfileKA(G4double gainz, G4double gainr) { gainentriesprofileka[(G4int)floor(gainz)][(G4int)gainr]++; }

    // Measure Cascade
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

    // Measure Branching Ratios
    // Append/Recall/nullify event protons/neutrons
    void appendEventBranchingPN(G4int numprotons, G4int numneutrons, G4int workerID)
      { eventProtons[workerID] += numprotons; eventNeutrons[workerID] += numneutrons; }
    G4int recallEventBranchingPN(G4int protonneutronType, G4int workerID)
      { if ( protonneutronType == 1 ) { return eventProtons[workerID]; } else { return eventNeutrons[workerID]; } }
    void nullEventBranchingPN(G4int workerID) { eventProtons[workerID] = 0; eventNeutrons[workerID] = 0; }
    // Append/Recall proton-neutron branching matrix
    void appendRunBranchingPN(G4int numprotons, G4int numneutrons) {
      if ( numprotons > 6 ) { numprotons = 6; }
      if ( numneutrons > 6 ) { numneutrons = 6; }
      runBranchingPN[numprotons][numneutrons] += 1;
    }
    G4double recallRunBranchingPN(G4int npro, G4int nneu)
      { return runBranchingPN[npro][nneu]; }

    // Supplemental analysis methods
    virtual void appendGainFile();
    virtual void writeGainProfileCu(G4int energyi);
    virtual void writeLossProfileCu(G4int energyi);
    virtual void writeGainProfileKA(G4int energyi);
    virtual void ntupleizeGainFile();
    virtual void analyze(G4int energyi);
    virtual void analyzeCascade();
    virtual void analyzeBranchingRatiosPN();

  private:

    // Global vars
    G4String analysisDIR;
    G4double runEnergy, runKA_thickness, runBeamFWHM;
    G4int nThreads, nEnergies, RunID;

    // Gain Measurement
    // vars
    G4bool measuregain;
    G4double runGain;
    //
    // Cu gain/loss, square-gain/loss and gain/loss entries matrices (100x30 bins z vs r)
    G4double gainprofilecu[100][30],
             gainsquareprofilecu[100][30],
             gainentriesprofilecu[100][30],
             lossprofilecu[100][30],
             losssquareprofilecu[100][30],
             lossentriesprofilecu[100][30];
    //
    // Kapton insulator gain and gain error profile
    G4double gainprofileka[100][31], // 100x31 bins z vs r
             gainsquareprofileka[100][31],
             gainentriesprofileka[100][31];

    // Energy deposition directional data
    G4double eE_r[100][100], eE_z[100][100],
             pE_r[100][100], pE_z[100][100],
             oE_r[100][100], oE_z[100][100],
             nE_r[100][100], nE_z[100][100],
             gE_r[100][100], gE_z[100][100];

    // Branching Ratios Measurement (p,NpMn) matrix vars
    G4int eventProtons[4], eventNeutrons[4]; // 4 core threading vars
    G4int runBranchingPN[7][7]; // 0, 1, 2, 3, 4, 5, >5 foreach
};

#endif
