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
#include <TText.h>

using namespace std;

const double min_jp = 2900;
const double max_jp = 3300;
const double min_Y1S = 9000;
const double max_Y1S = 9700;
const double min_Y2S = 9800;
const double max_Y2S = 10400;
const double min_Z = 85000;
const double max_Z = 100000;

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
  double mean_JP = 0;
  double mean_Y1S = 0;
  double mean_Y2S = 0;
  double mean_Z = 0;
  double err_JP = 0;
  double err_Y1S = 0;
  double err_Y2S = 0;
  double err_Z = 0;

  const char* title = "Zboson_invariant_mass";
  const char* title_m = "Zboson_invariant_mass_MeV";
  TFile *file = new TFile("hist.root");//file name
  TTree *tr = (TTree*)file->Get("tree");
  TCanvas *c1 = new TCanvas("c1","c1",1600,900);
  TH1D* hist_g = new TH1D(title,title,2400,30.,150.);
  TH1D* hist_m = new TH1D(title_m,title_m,1500,0.,150000.);
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
  hist_m->SetStats(0);
  hist_m->Draw();
  gStyle->SetOptStat(0);
  TF1 * fjp_m = new TF1("fjp_m","gaus",0,150);
  hist_m->Fit("fjp_m","+","",min_jp,max_jp);
  mean_JP = fjp_m->GetParameter(1);
  err_JP = fjp_m->GetParameter(2);
  TF1 * fy1s_m = new TF1("fy1s_m","gaus",0,150);
  hist_m->Fit("fy1s_m","+","",min_Y1S,max_Y1S);
  mean_Y1S = fy1s_m->GetParameter(1);
  err_Y1S = fy1s_m->GetParameter(2);
  TF1 * fy2s_m = new TF1("fy2s_m","gaus",0,150);
  hist_m->Fit("fy2s_m","+","",min_Y2S,max_Y2S);
  mean_Y2S = fy2s_m->GetParameter(1);
  err_Y2S = fy2s_m->GetParameter(2);
  TF1 * fz_m = new TF1("fz_m","gaus",0,150);
  hist_m->Fit("fz_m","+","",min_Z,max_Z);
  mean_Z = fz_m->GetParameter(1);
  err_Z = fz_m->GetParameter(2);
  TLatex  latex;
  latex.SetTextSize(0.03);
  latex.SetNDC(1);
  //experiment data
  string label_experiment = " Experimental_value ";
  string label_JP = Form("J/#psi = %d #pm ",static_cast<int>(mean_JP));  
  label_JP += Form("%d [MeV]",static_cast<int>(err_JP));
  string label_Y1S = Form("Y1S = %d #pm ",static_cast<int>(mean_Y1S));  
  label_Y1S += Form("%d [MeV]",static_cast<int>(err_Y1S));
  string label_Y2S = Form("Y2S = %d #pm ",static_cast<int>(mean_Y2S));  
  label_Y2S += Form("%d [MeV]",static_cast<int>(err_Y2S));
  string label_Z = Form("Z = %d #pm ",static_cast<int>(mean_Z));  
  label_Z += Form("%d [MeV]",static_cast<int>(err_Z));
  //literature value
  string label_value = " literature_value ";
  string label_v_JP = "J/#psi = 3096.916 #pm ";  
  label_v_JP += "0.011 [MeV]";
  string label_v_Y1S = "Y1S = 9460.30 #pm ";  
  label_v_Y1S += "0.26 [MeV]";
  string label_v_Y2S = "Y2S = 10023.26 #pm ";  
  label_v_Y2S += "0.31 [MeV]";
  string label_v_Z = "Z = 91187.6 #pm ";  
  label_v_Z += "2.1 [MeV]";
  latex.DrawLatex(0.4,0.75,label_experiment.c_str());
  latex.DrawLatex(0.4,0.70,label_JP.c_str());
  latex.DrawLatex(0.4,0.65,label_Y1S.c_str());
  latex.DrawLatex(0.4,0.6,label_Y2S.c_str());
  latex.DrawLatex(0.4,0.55,label_Z.c_str());
  latex.DrawLatex(0.6,0.75,label_value.c_str());
  latex.DrawLatex(0.6,0.70,label_v_JP.c_str());
  latex.DrawLatex(0.6,0.65,label_v_Y1S.c_str());
  latex.DrawLatex(0.6,0.6,label_v_Y2S.c_str());
  latex.DrawLatex(0.6,0.55,label_v_Z.c_str());
  c1->SaveAs("zboson_invariant_mass[MeV].png");
  hist_m.Write();
  c1->Close();
  delete c1;
  delete hist_g;

}