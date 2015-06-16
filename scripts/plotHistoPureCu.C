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
  TFile f1("Analysis.root");

  /// histogram construction ///

 
    /// Copper


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
}
