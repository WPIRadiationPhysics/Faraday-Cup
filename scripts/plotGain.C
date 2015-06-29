// -------------------------------------------------------------------
// $Id: plot.C 70323 2013-05-29 07:57:44Z gcosmo $
// -------------------------------------------------------------------
//
// *********************************************************************
// To execute this macro under ROOT after your simulation ended, 
//   1 - launch ROOT (usually type 'root' at your machine's prompt)
//   2 - type '.X plot.C' at the ROOT session prompt
// *********************************************************************
{
  // Init vars
  gStyle->SetOptStat(0);
  TNtuple* n1;

  // Construct Frame
  TCanvas gainCanvas("gainCanvas", "gainCanvas");
  TH2F frame("frame", "Net Gain", 0, 60, 260, 0, 0.95, 1.05);
  frame.Draw();

  // Gain data
  TFile f1("modelGain.root"); 
  n1 = (TNtuple*)f1->Get("gainData");
  n1->SetMarkerStyle(24); // circle
  n1->Draw("Gain:Energy", "", "same");

  // Format window
  frame->GetXaxis()->SetTitle("Beam Energy [MeV]");
  frame->GetYaxis()->SetTitle("Gain");

  // Update canvas data
  gainCanvas->Modified();
  gainCanvas->Update();

  // Update canvas data and save as png to file
  gainCanvas->Print("Gain.png");
}
