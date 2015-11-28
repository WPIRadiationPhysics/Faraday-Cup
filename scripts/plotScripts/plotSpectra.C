// *********************************************************************
// To execute this macro under ROOT after your simulation ended, 
//   1 - launch ROOT (usually type 'root' at your machine's prompt)
//   2 - type '.X plot.C' at the ROOT session prompt
// *********************************************************************
{
  // Construct Canvas
  gStyle->SetOptStat(0);
  TCanvas histoCanvas("histoCanvas", "histoCanvas");

  // Var declaration
  TH1F* h1;
  int* runEnergy[7] = {70, 100, 130, 160, 190, 220, 250};
  char particleName[5] = {'e', 'p', 'o', 'n', 'g'},
       spectraHistoName[30], spectraHistoFileName[80],
       figTitle[50], pngName[30], syscmd[50];

  // Create histogram directory
  system("mkdir -p ROOT/figs");

  /// histogram constructions ///

  // Particle and energy iteration
  for ( int particle_i = 0; particle_i < 5; particle_i++ ) {

    // Create particle directory
    sprintf(syscmd, "mkdir -p ROOT/figs/%cCu/", particleName[particle_i]);
    system((const char*)syscmd);
 
    for ( int energy_i = 0; energy_i < 7; energy_i++ ) {

      // Set data file
      sprintf(spectraHistoFileName, "ROOT/trackData-%d.root", energy_i);
      TFile f((const char*)spectraHistoFileName);

      // Define Tree names
      sprintf(spectraHistoName, "%cSpectra", particleName[particle_i]);

      // Acquire Deposition Histogram Trees
      h1 = (TH1F*) f->Get((const char*)spectraHistoName);

      // Format and output figure
      sprintf(figTitle, "%c Energy Spectrum via proton beam at %d MeV", particleName[particle_i], runEnergy[energy_i]);
      h1->SetTitle((const char*)figTitle);
      h1->GetXaxis()->SetTitle("Kinetic Energy [MeV]");
      h1->GetYaxis()->SetTitle("Intensity [arbitrary units]");
      h1->Draw();

      // Update canvas data
      histoCanvas->Modified();
      histoCanvas->Update();

      // Define output filename and save as png
      sprintf(pngName, "ROOT/figs/%cCu/spectra-%d.png", particleName[particle_i], energy_i);
      histoCanvas->Print((const char*)pngName);
    }
  }
}
