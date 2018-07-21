#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <TF1.h>
#include <TTree.h>
#include <TStyle.h>

using namespace std;


void muon_invariant_mass(){
  double min1 =30;//main minimum
  double max1 =70;//main maximum
  double a =5000 ;//main height
  double b =0.055 ;//main mean
  double min =80;//escape minimum
  double max =100;//escape maximum
  double h = 1000;//escape height
  double m = 0.032;//escape mean
  double w = 0.008;//escape width
  const char* title = "Zboson_invariant_mass";
  const char* title_m = "Zboson_invariant_mass_MeV";
  TFile *file = new TFile("hist.root");//file name
  TTree *tr = (TTree*)file->Get("tree");
  TCanvas *c1 = new TCanvas("c1","c1",1600,900);
  TH1D* hist_g = new TH1D(title,title,1200,30.,150.);
  TH1D* hist_m = new TH1D(title_m,title_m,150000,0.,150000.);
  double im = 0;
  tree->SetBranchAddress("muon_invariant_mass",&im);
  int nEntry = tree->GetEntries();
  for(int iEntry = 0;iEntry < nEntry;++iEntry){
   tree->GetEntry(iEntry);
   if(im > 0){
    hist_g->Fill(im);
    hist_m->Fill(im*1000);
   }
  }
  TFile *ofile = new TFile("fresult.root","RECREATE");//file name
  ofile.cd();
  //GeV Histgram
  hist_g->SetXTitle("Zboson_invariant_mass[GeV]");
  hist_g->SetYTitle("Events");
  hist_g->SetLabelSize(0.03,"y");
  hist_g->SetTitle("Zboson_invariant_mass");
  hist_g->Draw();
  gStyle->SetOptStat(1022);
  TF1 * fb_g = new TF1("fb_g","[0]*TMath::Exp(-[1]*x)",0,150);
  TF1 * fg_g = new TF1("fg_g","gaus",0,150);
  fb_g->SetParameters(a,b);
  fg_g->SetParameters(h,m,w); 
  hist_g->Fit("fb_g","R","",min1,max1);
  hist_g->Fit("fg_g","R","",min,max);
  a=fb_g->GetParameter(0);
  b=fb_g->GetParameter(1);
  m=fg_g->GetParameter(0);
  h=fg_g->GetParameter(1);
  w=fg_g->GetParameter(2);
  TF1 *fc_g=new TF1("fc_g","fb_g+fg_g",30,150);
  fc_g->SetParameters(a,b,m,h,w);
  fc_g->SetParNames("background_constants","background_slope","resonance_constants","resonance_mean","resonance_sigma");
  hist_g->Fit("fc_g","R","",30,150);
  gStyle->SetOptFit(1111);
  c1->SaveAs("zboson_invariant_mass[GeV].png");
  hist_g.Write();
  
  //MeV Histgram
  hist_m->SetXTitle("Zboson_invariant_mass[MeV]");
  hist_m->SetYTitle("Events");
  hist_m->SetLabelSize(0.03,"y");
  hist_m->SetTitle("Zboson_invariant_mass");
  hist_m->Draw();
  gStyle->SetOptStat(1022);
  TF1 * fb_m = new TF1("fb_m","[0]*TMath::Exp(-[1]*x)",0,150);
  TF1 * fg_m = new TF1("fg_m","gaus",0,150);
  fb_m->SetParameters(a,b);
  fg_m->SetParameters(h,m,w); 
  hist_m->Fit("fb_m","R","",min1*1000,max1*1000);
  hist_m->Fit("fg_m","R","",min*1000,max*1000);
  a=fb_m->GetParameter(0);
  b=fb_m->GetParameter(1);
  m=fg_m->GetParameter(0);
  h=fg_m->GetParameter(1);
  w=fg_m->GetParameter(2);
  TF1 *fd_m=new TF1("fc_m","fb_m+fg_m",30000,150000);
  fc_m->SetParameters(a,b,m,h,w);
  fc_m->SetParNames("background_constants","background_slope","resonance_constants","resonance_mean","resonance_sigma");
  hist_m->Fit("fc_m","R","",30000,150000);
  gStyle->SetOptFit(1111);
  c1->SaveAs("zboson_invariant_mass[MeV].png");
  hist_m.Write();
  c1->Close();


  delete hist_g;

}