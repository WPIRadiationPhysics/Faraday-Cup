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
  char particleName[3] = {'e', 'p', 'o'};
  char* interDepHistoName[30], intraDepHistoName[30],
        figTitleA[50], figTitleB[50], pngNameA[30], pngNameB[30];
  char* syscmd[50];

  // Create histogram directory
  sprintf(syscmd, "mkdir histos");
  system((const char*)syscmd);

  /// histogram constructions ///

  // Set data file
  TFile f("Analysis.root");

  // Particle and energy iteration
  for ( int particle_i = 0; particle_i < 3; particle_i++ ) {

  // Create particle directory
  sprintf(syscmd, "mkdir histos/%cCu/", particleName[particle_i]);
  system((const char*)syscmd);
 
  for ( int energy_i = 0; energy_i < 7; energy_i++ ) {

      // Define Tree names
      sprintf(interDepHistoName, "%cCuzr_in;%d", particleName[particle_i], energy_i+1);

      // Acquire Deposition Histogram Trees
      h2 = (TH2F*) f->Get((const char*)interDepHistoName);

      // Format and output figure A
      sprintf(figTitleA, "%c inter Cu deposition distribution at %d MeV", particleName[particle_i], runEnergy[energy_i]);
      h2->SetTitle((const char*)figTitleA);
      h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
      h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
      h2->Draw("COLZ");

      // Update canvas data
      histoCanvas->Modified();
      histoCanvas->Update();

      // Define output filename and save as png
      sprintf(pngNameA, "histos/%cCu/%dMeV_inter.png", particleName[particle_i], runEnergy[energy_i]);
      histoCanvas->Print((const char*)pngNameA);

      // Define Tree names
      sprintf(intraDepHistoName, "%cCuzr_inner;%d", particleName[particle_i], energy_i+1);

      // Acquire Deposition Histogram Trees
      h2 = (TH2F*) f->Get((const char*)intraDepHistoName);

      // Format and output figure B
      sprintf(figTitleB, "%c intra Cu deposition distribution at %d MeV", particleName[particle_i], runEnergy[energy_i]);
      h2->SetTitle((const char*)figTitleB);
      h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
      h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
      h2->Draw("COLZ");

      // Update canvas data
      histoCanvas->Modified();
      histoCanvas->Update();

      // Define output filename and save as png
      sprintf(pngNameB, "histos/%cCu/%dMeV_intra.png", particleName[particle_i], runEnergy[energy_i]);
      histoCanvas->Print((const char*)pngNameB);
  }}

    /*
    // eCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_in;4");
    h2->SetTitle("e- inter Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/eCuzr_in4.png");

    // eCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_out;4");
    h2->SetTitle("e- inter Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/eCuzr_out4.png");

    // eCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_inner;4");
    h2->SetTitle("e- intra Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/eCuzr_inner4.png");

    // eCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_outer;4");
    h2->SetTitle("e- intra Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/eCuzr_outer4.png");

    // pCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_in;4");
    h2->SetTitle("p+ inter Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/pCuzr_in4.png");

    // pCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_out;4");
    h2->SetTitle("p+ inter Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/pCuzr_out4.png");

    // pCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_inner;4");
    h2->SetTitle("p+ intra Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/pCuzr_inner4.png");

    // pCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_outer;4");
    h2->SetTitle("p+ intra Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/pCuzr_outer4.png");

    // oCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_in;4");
    h2->SetTitle("ion inter Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/oCuzr_in4.png");

    // oCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_out;4");
    h2->SetTitle("ion inter Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/oCuzr_out4.png");

    // oCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_inner;4");
    h2->SetTitle("ion intra Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/oCuzr_inner4.png");

    // oCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_outer;4");
    h2->SetTitle("ion intra Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("histos/oCuzr_outer4.png");
    */
}
