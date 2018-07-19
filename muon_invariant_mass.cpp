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

Double_t fitf(Double_t *x,Double_t *par){
  Double_t arg=0;
  if(par[4]!=0){arg=(x[0]-par[3])*(x[0]-par[3])/(2*par[4]*par[4]);
  }
  Double_t fitval= par[0]*TMath::Exp(-par[1]*x[0]) + par[2]*TMath::Exp(-arg);
    return fitval;
  }

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
  TFile *file = new TFile("hist.root");//file name
  TTree *tr = (TTree*)file->Get("tree");
  TCanvas *c1 = new TCanvas("c1","c1",1600,900);
  TH1D* hist = new TH1D(title,title,120,30.,150.);
  double im = 0;
  tree->SetBranchAddress("muon_invariant_mass",&im);
  int nEntry = tree->GetEntries();
  for(int iEntry = 0;iEntry < nEntry;++iEntry){
  	tree->GetEntry(iEntry);
  	if(im > 0)hist->Fill(im);
  }
  hist->SetXTitle("Zboson_invariant_mass[GeV]");
  hist->SetYTitle("Events");
  hist->SetLabelSize(0.03,"y");
  hist->SetTitle("Zboson_invariant_mass");
  hist->Draw();
  gStyle->SetOptStat(1022);
  TF1 * f1 = new TF1("f1","[0]*TMath::Exp(-[1]*x)",0,150);
  TF1 * f2 = new TF1("f2","gaus",0,150);
  f1->SetParameters(a,b);
  f2->SetParameters(h,m,w); 
  hist->Fit("f1","R","",min1,max1);
  hist->Fit("f2","R","",min,max);
  a=f1->GetParameter(0);
  b=f1->GetParameter(1);
  m=f2->GetParameter(0);
  h=f2->GetParameter(1);
  w=f2->GetParameter(2);
  TF1 *f3=new TF1("f3","f1+f2",30,150);
  f3->SetParameters(a,b,m,h,w);
  f3->SetParNames("background_constants","background_slope","resonance_constants","resonance_mean","resonance_sigma");
  hist->Fit("f3","R","",30,150);
  gStyle->SetOptFit(1111);
  c1->SaveAs("zboson_invariant_mass.png");
  //c1->Close();
  //delete c1;
  //delete hist;

}