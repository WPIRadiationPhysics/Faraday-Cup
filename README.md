Simulation of Faraday Cup Prototypes via Geant4
===============================================


Table of Contents:
------------------
1. GEOMETRY DEFINITION
2. PHYSICS LIST
3. ACTION INITALIZATION
4. PRIMARY GENERATOR
5. RUNS and EVENTS
6. DETECTOR RESPONSE
7. HISTOGRAMS
8. HOW TO BUILD/RUN
9. VISUALIZATION
10. USER INTERFACES


1 GEOMETRY DEFINITION
---------------------

DetectorConstruction.cc defines geometry and logical volumes.  Copper cylinder (r=3cm, h=10cm) residing within a Kapton insulation of thicknesses 59, 100, and 200 microns in air.  A thin layer of silver paint 12 microns thick is applied upon this solid, with a final layer of Kapton film 60 microns thick.  The beam is aligned to hit the Faraday Cup in the (+z)-direction, a distance *d* away from the center of the circular face.  The world which contains the beam and cup is composed of air material.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Air
                      Ag + Kapton

                    ..===============================++++.
                  .~/~   :`.                      .~`    `~.
                 ./~   Cu + Kapton               .~       `~.
                ./~        ..                   '~         `~.
                ||         .:                   ||          ||
 proton beam    ||         :.                   ||          ||
 ===========>   ||         ..                   ||          ||
                ||         .,                   ||          ||
                ||        ,.                    ||          .~
                ~~       ..`                     ~`         .~
                 ``     ,.`                       ~`.      .~
                  ``~================================+++++~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2 PHYSICS LIST
--------------
 
The particle's type and the physic processes which will be available are set in the FTFP\_BERT2.0 physics list. This physics list requires data files for electromagnetic and hadronic processes. See more on installation of the datasets in Geant4 Installation Guide,

Chapter 3.3: Note On Geant4 Datasets:  
[http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch03s03.html](http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch03s03.html)

 
3 ACTION INITALIZATION
----------------------

User action classes are defined in ActionInitialization.cc.  While in sequential mode the action classes are instatiated just once, via invoking the method `ActionInitialization::Build()` in multi-threading mode the same method is invoked for each thread worker and so all user action classes are defined thread-local.  A run action class is instantiated both thread-local and global; its instance is also created in the method `ActionInitialization::BuildForMaster()`, which is invoked only in multi-threading mode.


4 PRIMARY GENERATOR
------------------
 
The primary beam consists of high energy protons (> 50 MeV).  Specific runs are listed in the predefined macro file run.in. The type of the particle and its energy are set in the PrimaryGeneratorAction.cc class, and can be changed via the G4 built-in commands of the G4ParticleGun class.
 	

5 RUNS and EVENTS
-----------------
 
A run is a set of events.  The user can choose the frequency of printing via the Geant4 interactive command, for example:

```   
/run/printProgress 100
```

6 DETECTOR RESPONSE
-------------------

Both deposition and leakage of the charged particles are recorded on an event by event basis within the copper and Kapton logical volumes.  These quantities are data members of the EventAction class; they are collected step by step in `SteppingAction::UserSteppingAction()`.  In `EventAction::EndOfEventAction()`, these quantities are printed and filled in histogram datasets to accumulate statistic and compute dispersion.


7 HISTOGRAMS
------------

The analysis tools are used to accumulate statistics and compute the distribution of the energy deposition and track lengths of the charged particles.  The accumulated statistic and computed dispersion is printed at the end of run, in `RunAction::EndOfRunAction().`  When running in multi-threading mode, the histograms accumulated on threads are automatically merged in a single output file, while the ntuple is written in files per thread.


8 HOW TO BUILD/RUN
------------------

Generate a Makefile, then compile the project into an executable:

    % mkdir build
    % cd build
    % cmake -DGeant4\_DIR=/path/to/G4COMP ..
    % make
   
The program can be run with the following optional arguments:

    % ./FaradayCup [-m run.in ] [-u UIsession] [-t nThreads]
   
The -t option is available only in multi-threading mode and it allows the user to override the Geant4 default number of threads. The number of threads can be also set via G4FORCENUMBEROFTHREADS environment variable which has the top priority.

Execute FaradayCup in the 'interactive mode' with visualization

    % ./FaradayCup

and type in the commands from run1.mac line by line:

    Idle> /tracking/verbose 1 
    Idle> /run/beamOn 1
    Idle> ...
    Idle> exit

or

    Idle> /control/execute run100.mac
    ....
    Idle> exit

Execute FaradayCup in the 'batch' mode from macro files (without visualization)

    % ./FaradayCup -m run100.mac
    % ./FaradayCup -m FaradayCup.in > FaradayCup.out

Execute FaradayCup in the 'interactive mode' with a selected UI session, e.g. tcsh

    % ./FaradayCup -u tcsh 


9 VISUALIZATION
---------------

The visualization manager is set via the G4VisExecutive class in the `main()` function in FaradayCup.cc. The initialisation of the drawing is done via a set of `/vis/` commands in the macro vis.mac. This macro is automatically read from the main function when used in interactive running mode.

By default, vis.mac opens an OpenGL viewer (`/vis/open OGL`). The user can change the initial viewer by commenting out this line and instead uncommenting one of the other `/vis/open` statements, such as HepRepFile or DAWNFILE (which produce files that can be viewed with the HepRApp and DAWN viewers, respectively).  Note that one can always open new viewers at any time from the command line.  For example, if you already have a view in, say, an OpenGL window with a name "viewer-0", then

    /vis/open DAWNFILE

then to get the same view

    /vis/viewer/copyView viewer-0

or to get the same view _plus_ scene-modifications

    /vis/viewer/set/all viewer-0

then to see the result

    /vis/viewer/flush

The DAWNFILE, HepRepFile drivers are always available (since they require no external libraries), but the OGL driver requires that the Geant4 libraries have been built with the OpenGL option.  For more information on visualization, including information on how to install and run DAWN, OpenGL and HepRApp, see the visualization tutorials:

[http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4DAWNTutorial.html](http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4DAWNTutorial.html)  
[http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4OpenGLTutorial.html](http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4OpenGLTutorial.html)  
[http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4HepRAppTutorial.html](http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4HepRAppTutorial.html)

The tracks are automatically drawn at the end of each event, accumulated for all events and erased at the beginning of the next run.


10 USER INTERFACES
------------------

The user command interface is set via the G4UIExecutive class in the `main()` function in FaradayCup.cc. The selection of the user command interface is then done automatically according to the Geant4 configuration or it can be done explicitly via the third argument of the G4UIExecutive constructor (see FaradayCup.cc).
