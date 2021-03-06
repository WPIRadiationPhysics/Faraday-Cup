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
  TH2F* h2;
  int* runEnergy[7] = {70, 100, 130, 160, 190, 220, 250};
  char particleName[5] = {'e', 'p', 'o', 'n', 'g'},
       depHistoName[30], depHistoFileName[80],
       figTitle[50], pngName[30], syscmd[50];

  // Create histogram directory
  sprintf(syscmd, "mkdir -p depHistos");
  system((const char*)syscmd);

  /// histogram constructions ///

  // Particle and energy iteration
  for ( int particle_i = 0; particle_i < 5; particle_i++ ) {

    // Create particle directory
    sprintf(syscmd, "mkdir -p depHistos/%cCu/", particleName[particle_i]);
    system((const char*)syscmd);
 
    for ( int energy_i = 0; energy_i < 7; energy_i++ ) {

      // Set data file
      sprintf(depHistoFileName, "trackData-%d.root", energy_i);
      TFile f((const char*)depHistoFileName);

      // Define Tree names
      sprintf(depHistoName, "%cDepHistoCu", particleName[particle_i]);

      // Acquire Deposition Histogram Trees
      h2 = (TH2F*) f->Get((const char*)depHistoName);

      // Format and output figure
      sprintf(figTitle, "%c Cu distribution at %d MeV", particleName[particle_i], runEnergy[energy_i]);
      h2->SetTitle((const char*)figTitle);
      h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
      h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
      h2->Draw("COLZ");

      // Update canvas data
      histoCanvas->Modified();
      histoCanvas->Update();

      // Define output filename and save as png
      sprintf(pngName, "depHistos/%cCu/%dMeV.png", particleName[particle_i], runEnergy[energy_i]);
      histoCanvas->Print((const char*)pngName);
    }
  }

  // Create charge/energy deposition figures
  // Create volume directories
  sprintf(syscmd, "mkdir -p depHistos/gainCu/");
  system((const char*)syscmd);
  sprintf(syscmd, "mkdir -p depHistos/energyCu/");
  system((const char*)syscmd);

  for ( int energy_i = 0; energy_i < 7; energy_i++ ) {

    // Set data file
    sprintf(depHistoFileName, "trackData-%d.root", energy_i);
    TFile f((const char*)depHistoFileName);

    // Define Tree names
    sprintf(depHistoName, "gainDepHistoCu");

    // Acquire Deposition Histogram Trees
    h2 = (TH2F*) f->Get((const char*)depHistoName);

    // Format and output figure A
    sprintf(figTitle, "Cu gain distribution at %d MeV", runEnergy[energy_i]);
    h2->SetTitle((const char*)figTitle);
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data
    histoCanvas->Modified();
    histoCanvas->Update();

    // Define output filename and save as png
    sprintf(pngName, "depHistos/gainCu/%dMeV.png", runEnergy[energy_i]);
    histoCanvas->Print((const char*)pngName);

    // Define Tree names
    sprintf(depHistoName, "energyDepHistoCu");

    // Acquire Deposition Histogram Trees
    h2 = (TH2F*) f->Get((const char*)depHistoName);

    // Format and output figure A
    sprintf(figTitle, "Cu energy distribution at %d MeV",  runEnergy[energy_i]);
    h2->SetTitle((const char*)figTitle);
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data
    histoCanvas->Modified();
    histoCanvas->Update();

    // Define output filename and save as png
    sprintf(pngName, "depHistos/energyCu/%dMeV.png", runEnergy[energy_i]);
    histoCanvas->Print((const char*)pngName);
  }}
}
