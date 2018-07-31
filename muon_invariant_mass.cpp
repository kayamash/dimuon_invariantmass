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

using namespace RooFit;

const double min_jp = 2900;
const double max_jp = 3300;
const double min_Y1S = 9000;
const double max_Y1S = 9700;
const double min_Y2S = 9800;
const double max_Y2S = 10400;
const double min_Z = 85000;
const double max_Z = 100000;

void muon_invariant_mass(){
  double mean_JP = 0;
  double mean_Y1S = 0;
  double mean_Y2S = 0;
  double mean_Z = 0;
  double err_JP = 0;
  double err_Y1S = 0;
  double err_Y2S = 0;
  double err_Z = 0;

  const char* title = "Zboson_invariant_mass_MeV";
  TFile *file = new TFile("hist.root");//file name
  TTree *tr = (TTree*)file->Get("tree");
  TH1D* hist = new TH1D(title,title,1500,0.,150000.);
  double im = 0;
  tree->SetBranchAddress("muon_invariant_mass",&im);
  int nEntry = tree->GetEntries();
  for(int iEntry = 0;iEntry < nEntry;++iEntry){
   tree->GetEntry(iEntry);
   if(im > 0){
    hist->Fill(im*1000);
   }
  }
  TFile *ofile = new TFile("fresult.root","RECREATE");//file name
  ofile.cd();
  
  RooRealVar x("x", "x", 0, 150000);
  RooRealVar mean("mean", "Mean of Gaussian", 0, -10, 10);
  RooRealVar sigma("sigma", "Width of Gaussian", 1, -10, 10);
 
  RooGaussian gauss("gauss", "gauss(x, mean, sigma)", x, mean, sigma);
 
  RooDataHist data_h("data_h", "binned dataset", x, hist);   // binned data
  RooDataSet data_t("data_t", "unbinned dataset", tree, x);  // unbinned data
 
  RooPlot* xframe = x.frame();
  data_h.plotOn(xframe);
 
  gauss.fitTo(data_h);
  gauss.plotOn(xframe);
 
  gauss.fitTo(data_t);
  gauss.plotOn(xframe,LineColor(kRed));
 
  xframe->Draw();

  
}
