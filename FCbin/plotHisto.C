// *********************************************************************
// To execute this macro under ROOT after your simulation ended, 
//   1 - launch ROOT (usually type 'root' at your machine's prompt)
//   2 - type '.X plot.C' at the ROOT session prompt
// *********************************************************************
{
// Init vars
gStyle->SetOptStat(0);

// Construct Canvas
TCanvas histoCanvas("histoCanvas","Charge Deposition/Removal in Kaption");

// In Data
TFile f1("S59/stats.root"); 

TH1F* h1 = (TH1F*)f1->Get("eKA_in");
h1->SetFillColor(1);
h1->SetLineColor(1);
h1->SetBarWidth(0.5);
h1->Draw("hist B");

// Out Data, offset x1
TH1F* h2 = (TH1F*)f1->Get("eKA_out");
h2->SetFillColor(3);
h2->SetLineColor(3);
h2->SetBarWidth(0.5);
h2->SetBarOffset(0.75);
h2->Draw("hist same B");

// LATER: Inner Data, offset x2; Outer Data, offset x3 

// Legend
leg_hist = new TLegend(0.5,0.6,0.79,0.79);
leg_hist->SetHeader("Kapton e- Tracking");
leg_hist->AddEntry(h1,"e- Deposit","l");
leg_hist->AddEntry(h2,"e- Removal","l");
leg_hist->Draw("same");

histoCanvas->Modified();
histoCanvas->Update();

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
