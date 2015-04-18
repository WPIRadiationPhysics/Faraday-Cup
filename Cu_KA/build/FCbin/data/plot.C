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
gROOT->Reset();
gStyle->SetPalette(1);
gROOT->SetStyle("Plain");
TNtuple* ntuple;

// Construct Canvas	
TH2F frame("frame","S59-200 Net Gain", 0, 60, 260, 0, 0.95, 1.01);
gStyle->SetOptStat(0);
frame.Draw();

// S59 gain data
TFile f1("S59/gainData.root"); 
n1 = (TNtuple*)f1->Get("gainDat");
n1->SetMarkerColor(1);
n1->SetMarkerStyle(24);
n1->SetFillColor(4);
n1->SetLineColor(3);
n1->Draw("beamGain:beamEnergy", "", "same");


// S100 gain data
TFile f2("S100/gainData.root"); 
n2 = (TNtuple*)f2->Get("gainDat");
n2->SetMarkerColor(1);
n2->SetMarkerStyle(25);
n2->SetFillColor(4);
n2->SetLineColor(3);
n2->Draw("beamGain:beamEnergy", "", "same");

// S200 gain data
TFile f3("S200/gainData.root"); 
n3 = (TNtuple*)f3->Get("gainDat");
n3->SetMarkerColor(1);
n3->SetMarkerStyle(26);
n3->SetFillColor(4);
n3->SetLineColor(3);
n3->Draw("beamGain:beamEnergy", "", "same");

// Draw legend
Double_t xl1=.15, yl1=0.75, xl2=xl1+.3, yl2=yl1+.125;
TLegend* leg = new TLegend(xl1, yl1, xl2, yl2);
leg->AddEntry(n1, "S59", "p");
leg->AddEntry(n2, "S100", "p");
leg->AddEntry(n3, "S200", "p");
leg->Draw();

//c1->Update();
//c1->Print();
/*
  htemp->GetXaxis()->SetLabelSize(0.025);
  htemp->GetYaxis()->SetLabelSize(0.025);
  htemp->GetZaxis()->SetLabelSize(0.025);
  htemp->GetXaxis()->SetTitleSize(0.035);
  htemp->GetYaxis()->SetTitleSize(0.035);
  htemp->GetZaxis()->SetTitleSize(0.035);
  htemp->GetXaxis()->SetTitleOffset(1.6);
  htemp->GetYaxis()->SetTitleOffset(1.6);
  htemp->GetZaxis()->SetTitleOffset(1.6);
  htemp->GetXaxis()->SetTitle("z (micrometer)");
  htemp->GetYaxis()->SetTitle("x (nanometer)");
  htemp->GetZaxis()->SetTitle("y (nanometer)");
  htemp->SetTitle("Track Structure in liquid water");
*/  
}
