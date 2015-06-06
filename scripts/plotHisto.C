// *********************************************************************
// To execute this macro under ROOT after your simulation ended, 
//   1 - launch ROOT (usually type 'root' at your machine's prompt)
//   2 - type '.X plot.C' at the ROOT session prompt
// *********************************************************************
{
  // TDirectory file method syntax requires const char names
  // Therefore no looping.  Thanks, CERN.

  // Init vars
  TH2F* h2;
  gStyle->SetOptStat(0);

  // Construct Canvas
  TCanvas histoCanvas("histoCanvas","histoCanvas");

  // Set data file
  TFile f1("S59/Analysis.root");
  TFile f2("S100/Analysis.root");
  TFile f3("S200/Analysis.root");

  /// histogram construction ///

    // S59
    // eKAzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eKAzr_in;4");
    h2->SetTitle("e- inter S59 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/eKAzr_in4.png");

    // eKAzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eKAzr_out;4");
    h2->SetTitle("e- inter S59 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/eKAzr_out4.png");

    // eKAzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eKAzr_inner;4");
    h2->SetTitle("e- intra S59 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/eKAzr_inner4.png");

    // eKAzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eKAzr_outer;4");
    h2->SetTitle("e- intra S59 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/eKAzr_outer4.png");

    // S100
    // eKAzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eKAzr_in;4");
    h2->SetTitle("e- inter S100 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/eKAzr_in4.png");

    // eKAzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eKAzr_out;4");
    h2->SetTitle("e- inter S100 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/eKAzr_out4.png");

    // eKAzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eKAzr_inner;4");
    h2->SetTitle("e- intra S100 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/eKAzr_inner4.png");

    // eKAzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eKAzr_outer;4");
    h2->SetTitle("e- intra S100 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/eKAzr_outer4.png");

    // S200
    // eKAzr_in
    // Get and format histogram
    h2 = (TH2F*)f2.Get("eKAzr_in;4");
    h2->SetTitle("e- inter S200 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data
    histoCanvas->Modified();
    histoCanvas->Update();

    // Save canvas as png image to file
    histoCanvas->Print("S200/histos/eKAzr_in4.png");

    // eKAzr_out
    // Get and format histogram
    h2 = (TH2F*)f2.Get("eKAzr_out;4");
    h2->SetTitle("e- inter S200 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/eKAzr_out4.png");

    // eKAzr_inner
    // Get and format histogram
    h2 = (TH2F*)f2.Get("eKAzr_inner;4");
    h2->SetTitle("e- intra S200 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/eKAzr_inner4.png");

    // eKAzr_outer
    // Get and format histogram
    h2 = (TH2F*)f2.Get("eKAzr_outer;4");
    h2->SetTitle("e- intra S200 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/eKAzr_outer4.png");
}
