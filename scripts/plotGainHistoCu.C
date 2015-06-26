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
       gainHistoName[50], gainHistoFileName[30],
       figTitle[50], pngName[30], syscmd[50];

  // Create histogram directory
  sprintf(syscmd, "mkdir -p gainHistos");
  system((const char*)syscmd);

  /// histogram constructions ///

  // Particle and energy iteration
  for ( int particle_i = 0; particle_i < 5; particle_i++ ) {

    // Create particle directory
    sprintf(syscmd, "mkdir -p gainHistos/%cCu/", particleName[particle_i]);
    system((const char*)syscmd);
 
    for ( int energy_i = 0; energy_i < 7; energy_i++ ) {

      // Set data file
      sprintf(gainHistoFileName, "trackData-%d.root", energy_i);
      TFile f((const char*)gainHistoFileName);

      // Define Tree names
      sprintf(gainHistoName, "%cGainHistoCu", particleName[particle_i]);

      // Acquire Deposition Histogram Trees
      h2 = (TH2F*) f->Get((const char*)gainHistoName);

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
      sprintf(pngName, "gainHistos/%cCu/%dMeV.png", particleName[particle_i], runEnergy[energy_i]);
      histoCanvas->Print((const char*)pngName);
    }
  }
}
