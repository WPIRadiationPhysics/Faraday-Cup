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

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " runFaradayCup [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

int main(int argc,char** argv) {
  // Evaluate arguments
  if ( argc > 7 ) {
    PrintUsage();
    return 1;
  }  
  G4String macro;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
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

  // Initialize G4 kernel
  runManager->Initialize();
  
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
  if ( macro.size() ) {
	// Kapton AND Copper Optimization problems - 4D data
	// micrometer particle track cuts
    G4String cutCommand = "/run/setCut 0.005 mm";
    UImanager->ApplyCommand(cutCommand);
      
	G4int KA_thickness[3] = {25, 50, 75};
	G4int Cu_radius[7] = {2, 3, 4, 5, 6, 8, 10};
	for ( G4int KA_i=0; KA_i<3; KA_i++ ) {
      for ( G4int Cu_i=0; Cu_i<7; Cu_i++ ) {
	    // Assign dimensions, update geometry
	    detConstruction->RedimensionIteration(Cu_i, KA_i);
	    runManager->GeometryHasBeenModified();
	    
        // Create data directory and leave thickness flag for SteppingAction
	    G4String data_dir = "data/"; std::ostringstream raw_dirCommand;
	    raw_dirCommand << "mkdir -p " << data_dir << "; echo " << KA_i << " > " << data_dir << ".film";
	    G4String dirCommand = raw_dirCommand.str();
        system(dirCommand);
	  
	    // Run experimental beam energies
	    G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        // Remove run logs (to be placed in RunAction when confirmed single
        // worker thread to be in control of file I/O)
        G4String runRm = "rm " + data_dir + "run*";
        system(runRm);
        
        // Define film file
        std::ostringstream raw_film_file; raw_film_file << data_dir << "KA" << KA_thickness[KA_i] << "_signalError.txt";
	    G4String film_file = raw_film_file.str();
	    std::ofstream filmFile;
        
        // Insert radius to film file
	    filmFile.open (film_file, std::ios::app);
        filmFile << Cu_radius[Cu_i] << "cm "; filmFile.close();
        
        // Save completed dataset as film file line
	    G4String filmcmd = "tail -1 " + data_dir + "error.txt >> " + film_file;
	    system(filmcmd);
	    
	    // Remove temporary calculation file
	    filmcmd = "rm " + data_dir + "error.txt";
	    system(filmcmd);
	  }
    }
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv, session);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    if (ui->IsGUI())
      UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
#endif
  }

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
