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

 
    /// Kapton

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

    // S59
    // pKAzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pKAzr_in;4");
    h2->SetTitle("p+ inter S59 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/pKAzr_in4.png");

    // pKAzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pKAzr_out;4");
    h2->SetTitle("p+ inter S59 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/pKAzr_out4.png");

    // pKAzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pKAzr_inner;4");
    h2->SetTitle("p+ intra S59 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/pKAzr_inner4.png");

    // pKAzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pKAzr_outer;4");
    h2->SetTitle("p+ intra S59 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/pKAzr_outer4.png");

    // S100
    // pKAzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pKAzr_in;4");
    h2->SetTitle("p+ inter S100 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/pKAzr_in4.png");

    // pKAzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pKAzr_out;4");
    h2->SetTitle("p+ inter S100 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/pKAzr_out4.png");

    // pKAzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pKAzr_inner;4");
    h2->SetTitle("p+ intra S100 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/pKAzr_inner4.png");

    // pKAzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pKAzr_outer;4");
    h2->SetTitle("p+ intra S100 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/pKAzr_outer4.png");

    // S200
    // pKAzr_in
    // Get and format histogram
    h2 = (TH2F*)f2.Get("pKAzr_in;4");
    h2->SetTitle("p+ inter S200 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data
    histoCanvas->Modified();
    histoCanvas->Update();

    // Save canvas as png image to file
    histoCanvas->Print("S200/histos/pKAzr_in4.png");

    // pKAzr_out
    // Get and format histogram
    h2 = (TH2F*)f2.Get("pKAzr_out;4");
    h2->SetTitle("p+ inter S200 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/pKAzr_out4.png");

    // pKAzr_inner
    // Get and format histogram
    h2 = (TH2F*)f2.Get("pKAzr_inner;4");
    h2->SetTitle("p+ intra S200 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/pKAzr_inner4.png");

    // pKAzr_outer
    // Get and format histogram
    h2 = (TH2F*)f2.Get("pKAzr_outer;4");
    h2->SetTitle("p+ intra S200 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/pKAzr_outer4.png");

    // S59
    // oKAzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oKAzr_in;4");
    h2->SetTitle("ion inter S59 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/oKAzr_in4.png");

    // oKAzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oKAzr_out;4");
    h2->SetTitle("ion inter S59 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/oKAzr_out4.png");

    // oKAzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oKAzr_inner;4");
    h2->SetTitle("ion intra S59 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/oKAzr_inner4.png");

    // oKAzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oKAzr_outer;4");
    h2->SetTitle("ion intra S59 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/oKAzr_outer4.png");

    // S100
    // oKAzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oKAzr_in;4");
    h2->SetTitle("ion inter S100 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/oKAzr_in4.png");

    // oKAzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oKAzr_out;4");
    h2->SetTitle("ion inter S100 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/oKAzr_out4.png");

    // oKAzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oKAzr_inner;4");
    h2->SetTitle("ion intra S100 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/oKAzr_inner4.png");

    // oKAzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oKAzr_outer;4");
    h2->SetTitle("ion intra S100 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/oKAzr_outer4.png");

    // S200
    // oKAzr_in
    // Get and format histogram
    h2 = (TH2F*)f2.Get("oKAzr_in;4");
    h2->SetTitle("ion inter S200 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data
    histoCanvas->Modified();
    histoCanvas->Update();

    // Save canvas as png image to file
    histoCanvas->Print("S200/histos/oKAzr_in4.png");

    // oKAzr_out
    // Get and format histogram
    h2 = (TH2F*)f2.Get("oKAzr_out;4");
    h2->SetTitle("ion inter S200 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/oKAzr_out4.png");

    // oKAzr_inner
    // Get and format histogram
    h2 = (TH2F*)f2.Get("oKAzr_inner;4");
    h2->SetTitle("ion intra S200 Kapton deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/oKAzr_inner4.png");

    // oKAzr_outer
    // Get and format histogram
    h2 = (TH2F*)f2.Get("oKAzr_outer;4");
    h2->SetTitle("ion intra S200 Kapton removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/oKAzr_outer4.png");


    /// Copper


    // S59
    // eCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_in;4");
    h2->SetTitle("e- inter S59 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/eCuzr_in4.png");

    // eCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_out;4");
    h2->SetTitle("e- inter S59 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/eCuzr_out4.png");

    // eCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_inner;4");
    h2->SetTitle("e- intra S59 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/eCuzr_inner4.png");

    // eCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_outer;4");
    h2->SetTitle("e- intra S59 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/eCuzr_outer4.png");

    // S100
    // eCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_in;4");
    h2->SetTitle("e- inter S100 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/eCuzr_in4.png");

    // eCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_out;4");
    h2->SetTitle("e- inter S100 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/eCuzr_out4.png");

    // eCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_inner;4");
    h2->SetTitle("e- intra S100 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/eCuzr_inner4.png");

    // eCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("eCuzr_outer;4");
    h2->SetTitle("e- intra S100 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/eCuzr_outer4.png");

    // S200
    // eCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f2.Get("eCuzr_in;4");
    h2->SetTitle("e- inter S200 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data
    histoCanvas->Modified();
    histoCanvas->Update();

    // Save canvas as png image to file
    histoCanvas->Print("S200/histos/eCuzr_in4.png");

    // eCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f2.Get("eCuzr_out;4");
    h2->SetTitle("e- inter S200 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/eCuzr_out4.png");

    // eCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f2.Get("eCuzr_inner;4");
    h2->SetTitle("e- intra S200 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/eCuzr_inner4.png");

    // eCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f2.Get("eCuzr_outer;4");
    h2->SetTitle("e- intra S200 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/eCuzr_outer4.png");

    // S59
    // pCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_in;4");
    h2->SetTitle("p+ inter S59 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/pCuzr_in4.png");

    // pCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_out;4");
    h2->SetTitle("p+ inter S59 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/pCuzr_out4.png");

    // pCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_inner;4");
    h2->SetTitle("p+ intra S59 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/pCuzr_inner4.png");

    // pCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_outer;4");
    h2->SetTitle("p+ intra S59 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/pCuzr_outer4.png");

    // S100
    // pCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_in;4");
    h2->SetTitle("p+ inter S100 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/pCuzr_in4.png");

    // pCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_out;4");
    h2->SetTitle("p+ inter S100 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/pCuzr_out4.png");

    // pCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_inner;4");
    h2->SetTitle("p+ intra S100 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/pCuzr_inner4.png");

    // pCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("pCuzr_outer;4");
    h2->SetTitle("p+ intra S100 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/pCuzr_outer4.png");

    // S200
    // pCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f2.Get("pCuzr_in;4");
    h2->SetTitle("p+ inter S200 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data
    histoCanvas->Modified();
    histoCanvas->Update();

    // Save canvas as png image to file
    histoCanvas->Print("S200/histos/pCuzr_in4.png");

    // pCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f2.Get("pCuzr_out;4");
    h2->SetTitle("p+ inter S200 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/pCuzr_out4.png");

    // pCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f2.Get("pCuzr_inner;4");
    h2->SetTitle("p+ intra S200 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/pCuzr_inner4.png");

    // pCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f2.Get("pCuzr_outer;4");
    h2->SetTitle("p+ intra S200 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/pCuzr_outer4.png");

    // S59
    // oCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_in;4");
    h2->SetTitle("ion inter S59 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/oCuzr_in4.png");

    // oCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_out;4");
    h2->SetTitle("ion inter S59 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/oCuzr_out4.png");

    // oCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_inner;4");
    h2->SetTitle("ion intra S59 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/oCuzr_inner4.png");

    // oCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_outer;4");
    h2->SetTitle("ion intra S59 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S59/histos/oCuzr_outer4.png");

    // S100
    // oCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_in;4");
    h2->SetTitle("ion inter S100 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/oCuzr_in4.png");

    // oCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_out;4");
    h2->SetTitle("ion inter S100 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/oCuzr_out4.png");

    // oCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_inner;4");
    h2->SetTitle("ion intra S100 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/oCuzr_inner4.png");

    // oCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f1.Get("oCuzr_outer;4");
    h2->SetTitle("ion intra S100 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S100/histos/oCuzr_outer4.png");

    // S200
    // oCuzr_in
    // Get and format histogram
    h2 = (TH2F*)f2.Get("oCuzr_in;4");
    h2->SetTitle("ion inter S200 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data
    histoCanvas->Modified();
    histoCanvas->Update();

    // Save canvas as png image to file
    histoCanvas->Print("S200/histos/oCuzr_in4.png");

    // oCuzr_out
    // Get and format histogram
    h2 = (TH2F*)f2.Get("oCuzr_out;4");
    h2->SetTitle("ion inter S200 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/oCuzr_out4.png");

    // oCuzr_inner
    // Get and format histogram
    h2 = (TH2F*)f2.Get("oCuzr_inner;4");
    h2->SetTitle("ion intra S200 Copper deposition distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/oCuzr_inner4.png");

    // oCuzr_outer
    // Get and format histogram
    h2 = (TH2F*)f2.Get("oCuzr_outer;4");
    h2->SetTitle("ion intra S200 Copper removal distribution @ 160 MeV");
    h2->GetXaxis()->SetTitle("depth_percentile (z-axis)");
    h2->GetYaxis()->SetTitle("depth_percentile (r-axis)");
    h2->Draw("COLZ");

    // Update canvas data and save as png to file
    histoCanvas->Modified();
    histoCanvas->Update();
    histoCanvas->Print("S200/histos/oCuzr_outer4.png");
}
