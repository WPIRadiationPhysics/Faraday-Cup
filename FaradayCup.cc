#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"
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
    G4cerr << " FaradayCup [-m macroFile ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   Note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

int main(int argc,char** argv) {
  // Evaluate arguments
  if ( argc > 9 ) {
    PrintUsage();
    return 1;
  }  
  G4String macroFile;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macroFile = argv[i+1];
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

  // Set mandatory initialization classes
  DetectorConstruction* detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  runManager->SetUserInitialization(physicsList);
  
  ActionInitialization* actionInitialization
     = new ActionInitialization(detConstruction);
  runManager->SetUserInitialization(actionInitialization);
  runManager->Initialize();

  // Simulation analysis object
  Analysis* simulationAnalysis = Analysis::GetAnalysis();

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // batch mode  
  //if ( macroFile.size() ) {

    // 5 micrometer particle track cuts
    G4String cutCommand = "/run/setCut 0.005 mm";
    UImanager->ApplyCommand(cutCommand);
    UImanager->ApplyCommand("/gun/particle proton");
	
    // Constant vars, declarations
    G4double KA_thickness[3] = {59, 100, 200};
    G4double energies[7] = {70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250.00};
    G4int nThicknesses = (int)sizeof(KA_thickness)/sizeof(G4double);
    G4int nEnergies = (int)sizeof(energies)/sizeof(G4double);
    std::ostringstream data_dirStream, syscmdStream, filmDirStream;
    G4String data_dir, dirCommand, syscmd, filmDir;

    // Initialize main data directory
    system("mkdir -p data");

    simulationAnalysis->SetNThreads(nThreads);
    simulationAnalysis->SetNEnergies(nEnergies);

    // Model loop
    for ( G4int model_i = 0; model_i < 3; model_i++ ) {

      // Assign geometric configuration
      detConstruction->ModelConfiguration(model_i);

      // Unsheathed Cu primary model
      if ( model_i == 0 ) {
        for ( G4int energy_i = 0; energy_i < nEnergies; energy_i++ ) {

          // Nullify experiments
          simulationAnalysis->nullExperiments();

          // Set data directory
          data_dir = "data/model0/";
          simulationAnalysis->SetAnalysisDIR(data_dir);

          // Set run energy for analysis
          simulationAnalysis->SetRunEnergy(energies[energy_i]);

          // Run experimental beam energies
          syscmdStream.str(""); syscmdStream << "/gun/energy " << energies[energy_i] << " MeV";
          syscmd = syscmdStream.str(); UImanager->ApplyCommand(syscmd);
          UImanager->ApplyCommand("/run/beamOn 100");

          // Cascade Analysis
          simulationAnalysis->analyzeCascadeTracks();

          // Move plot scripts to data directory
          syscmd = "cp plotGainCu.C " + data_dir; system(syscmd);
          syscmd = "cp plotSpectra.C " + data_dir; system(syscmd);
          syscmd = "cp plotDepHistoCu.C " + data_dir; system(syscmd);
          syscmd = "cp plotCascadeHisto.m " + data_dir; system(syscmd);
        }
               
      // Kapton (layer 1) thickness iteration for secondary models
      } else { for ( G4int KA_i = 0; KA_i < nThicknesses; KA_i++ ) {

        // Assign thickness
        detConstruction->IterateKaptonThickness(KA_thickness[KA_i]);

        for ( G4int energy_i = 0; energy_i < nEnergies; energy_i++ ) {

          // Nullify experiments
          simulationAnalysis->nullExperiments();

          // Reference KA thickness for calculations
          simulationAnalysis->SetRunKAThickness(KA_thickness[KA_i]);

          // Set data directory
          data_dirStream.str(""); data_dirStream << "data/model" << model_i << "/S" << KA_thickness[KA_i] << "/";
          data_dir = data_dirStream.str(); simulationAnalysis->SetAnalysisDIR(data_dir);

          // Set run energy for analysis
          simulationAnalysis->SetRunEnergy(energies[energy_i]);

          // Run experimental beam energies
          syscmdStream.str(""); syscmdStream << "/gun/energy " << energies[energy_i] << " MeV";
          syscmd = syscmdStream.str(); UImanager->ApplyCommand(syscmd);
          UImanager->ApplyCommand("/run/beamOn 100");

          // Cascade Analysis
          simulationAnalysis->analyzeCascadeTracks();

          // Move plot scripts to data directory
          syscmd = "cp plotGain.C " + data_dir; system(syscmd);
          syscmd = "cp plotSpectra.C " + data_dir; system(syscmd);
          syscmd = "cp plotDepHistoCuKA.C " + data_dir; system(syscmd);
          syscmd = "cp plotCascadeHisto.m " + data_dir; system(syscmd);
      }}}
    }

  /*
  } else {

    // interactive mode : define UI session
    G4String cutCommand = "/run/setCut 0.01 mm";
    UImanager->ApplyCommand(cutCommand);

    // Constant vars
    G4int KA_thickness[3] = {59, 100, 200};
    G4String data_dir = "data/";

    for ( G4int thickness_i=0; thickness_i<3; thickness_i++ ) {

      // Create data directory and leave thickness flag for SteppingAction
      std::ostringstream raw_dirCommand;
      raw_dirCommand << "mkdir -p " << data_dir << "; echo " << thickness_i << " > " << data_dir << ".flag";
      G4String dirCommand = raw_dirCommand.str();
      system(dirCommand);
	  
      // Run experimental beam energies
      G4String command = "/control/execute ";
      UImanager->ApplyCommand(command+macroFile);

      // Remove run logs (to be placed in RunAction when confirmed single
      // worker thread to be in control of file I/O)
      G4String runRm = "rm " + data_dir + "run*";
      system(runRm);
	  
      // Save completed dataset as film iteration
      std::ostringstream raw_film_file; raw_film_file << "S" << KA_thickness[thickness_i] << "_gain.txt";
      G4String film_file = raw_film_file.str();
      G4String filmcmd = "mv " + data_dir + "gain.txt " + data_dir + film_file;
      system(filmcmd);
    }
    
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
*/

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
