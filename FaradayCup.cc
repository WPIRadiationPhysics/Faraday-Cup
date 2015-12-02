#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT_HP.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Analysis.hh"

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " FaradayCup [-n nEvents ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   Note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

void beamSimulation(G4int nEvents) {

  // Beam parameters and vars
  G4double energies[7] = {70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250.00}; // MeV
  G4double beam_fwhm[7] = {22.8, 15.7, 12.5, 10.6, 8.9, 8.1, 8.1}; // mm
  G4int nEnergies = (int)sizeof(energies)/sizeof(G4double);
  std::ostringstream syscmdStream;
  G4String syscmd;

  // Simulation analysis object
  Analysis* simulationAnalysis = Analysis::GetAnalysis();
  simulationAnalysis->SetNEnergies(nEnergies);

  // Interface manager object
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  for ( G4int energy_i = 0; energy_i < nEnergies; energy_i++ ) {

    // Reset and redefine experiment
    simulationAnalysis->resetExperiment();
    simulationAnalysis->measureGain();
    //simulationAnalysis->measureCascade();
    //simulationAnalysis->measureBranchingRatiosPN();

    // Set run energy and beam width
    simulationAnalysis->SetRunEnergy(energies[energy_i]);
    simulationAnalysis->SetRunBeamFWHM(beam_fwhm[energy_i]);

    // Run experimental beam energies
    syscmdStream.str(""); syscmdStream << "/gun/energy " << energies[energy_i] << " MeV";
    syscmd = syscmdStream.str(); UImanager->ApplyCommand(syscmd);
    syscmdStream.str(""); syscmdStream << "/run/beamOn " << nEvents;
    syscmd = syscmdStream.str(); UImanager->ApplyCommand(syscmd);

    // Perform Analyses
    simulationAnalysis->analyze(energy_i);
  }

  return;
}

int main(int argc, char** argv) {

  // Declare vars
  G4int nEvents = 0;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif

  // Evaluate arguments, don't allow breaking
  if ( argc > 9 ) { PrintUsage(); return 1; }  
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-n" ) nEvents = std::atoi(argv[i+1]);
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }  
  
  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  if ( nThreads > 0 ) { 
    runManager->SetNumberOfThreads(nThreads);
  }  
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  // Initialize Geometry
  DetectorConstruction* detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  // Initialize Physics List
  G4VModularPhysicsList* physicsList = new FTFP_BERT_HP;
  runManager->SetUserInitialization(physicsList);

  // Initialize Action
  ActionInitialization* actionInitialization = new ActionInitialization(detConstruction);
  runManager->SetUserInitialization(actionInitialization);
  runManager->Initialize();

  // Simulation analysis object
  Analysis* simulationAnalysis = Analysis::GetAnalysis();
  simulationAnalysis->SetNThreads(nThreads);

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Proton beam
  UImanager->ApplyCommand("/gun/particle proton");

  // Constant vars, declarations
  G4String models[3] = {"Cu", "CuKA", "CuKA+AgKA"};
  G4double KA_thickness[3] = {50, 100, 200}; // microns
  G4int nThicknesses = (int)sizeof(KA_thickness)/sizeof(G4double);
  std::ostringstream data_dirStream, syscmdStream, filmDirStream;
  G4String data_dir, dirCommand, syscmd, filmDir;

  // Begin simulation
  if ( nEvents > 0 ) {

    // 5 micrometer particle track cuts
    UImanager->ApplyCommand("/run/setCut 0.005 mm");

    // Initialize main data directory
    system("mkdir -p data");

    // Model loop
    for ( G4int model_i = 2; model_i < 3; model_i++ ) { // Prototype (model_i == 2)

      // Assign geometric configuration
      detConstruction->ModelConfiguration(model_i);

      // Create Model data directory
      data_dirStream.str(""); data_dirStream << "mkdir -p data/" << models[model_i];
      data_dir = data_dirStream.str(); system(data_dir);

      // Cu model
      if ( model_i == 0 ) {

        // Set data directory
        simulationAnalysis->SetAnalysisDIR("data/Cu/");

        // Run beam (settings below)
        beamSimulation(nEvents);
      }

      // Kapton (layer 1) thickness iteration for secondary models
      if ( model_i == 1 || model_i == 2 ) {

        for ( G4int KA_i = 0; KA_i < nThicknesses; KA_i++ ) {

        // Assign model insulator thickness
        detConstruction->IterateKaptonThickness(KA_thickness[KA_i]);
        simulationAnalysis->SetRunKAThickness(KA_thickness[KA_i]);

        // Set subdata directory
        data_dirStream.str(""); data_dirStream << "data/" << models[model_i] << "/S" << KA_thickness[KA_i] << "/";
        data_dir = data_dirStream.str(); syscmd = "mkdir -p " + data_dir; system(syscmd);
        simulationAnalysis->SetAnalysisDIR(data_dir);

        // Run beam (settings below)
        beamSimulation(nEvents);
      }}
    }

  } else { // QT Visualizer

    // mm track cuts
    UImanager->ApplyCommand("/run/setCut 1 mm");

    // Prototype (model_i == 2)
    detConstruction->ModelConfiguration(2);

    // Default thickness (S50)
    detConstruction->IterateKaptonThickness(KA_thickness[0]);

    // Default run energy and beam width
    simulationAnalysis->SetRunEnergy(70.03);
    simulationAnalysis->SetRunBeamFWHM(22.8);

    // Initialize visualizer and gui macros
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv, session);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    if (ui->IsGUI()) { UImanager->ApplyCommand("/control/execute gui.mac"); }
    ui->SessionStart();
    delete ui;
#endif
  }

  // Dump manager objects from memory
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
