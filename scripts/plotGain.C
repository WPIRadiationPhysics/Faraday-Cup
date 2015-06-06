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
  TH2F frame("frame","S59-200 Net Gain", 0, 60, 260, 0, 0.95, 1.01);
  frame.Draw();

  // S59 gain data
  TFile f1("S59/Analysis.root"); 
  n1 = (TNtuple*)f1->Get("gainDat");
  n1->SetMarkerStyle(24); // circle
  n1->Draw("beamGain:beamEnergy", "", "same");

  // S100 gain data
  TFile f2("S100/Analysis.root"); 
  n2 = (TNtuple*)f2->Get("gainDat");
  n2->SetMarkerStyle(25); // square
  n2->Draw("beamGain:beamEnergy", "", "same");

  // S200 gain data
  TFile f3("S200/Analysis.root"); 
  n3 = (TNtuple*)f3->Get("gainDat");
  n3->SetMarkerStyle(26); // triangle
  n3->Draw("beamGain:beamEnergy", "", "same");

  // Draw legend
  Double_t xl1=.15, yl1=0.75, xl2=xl1+.3, yl2=yl1+.125;
  TLegend* leg = new TLegend(xl1, yl1, xl2, yl2);
  leg->AddEntry(n1, "S59", "p");
  leg->AddEntry(n2, "S100", "p");
  leg->AddEntry(n3, "S200", "p");
  leg->Draw();

  // Update canvas data and save as png to file
  frame->Print("Gain.png");
}
