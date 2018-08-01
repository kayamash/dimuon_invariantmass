using namespace RooFit;
 
void fitzmass_roofit(){
  TCanvas *c1 = new TCanvas("dimuon_invariant_mass[GeV]","dimuon_invariant_mass[GeV]",1600*1.1,900*1.1);
  TTree *tree = new TTree("tree","tree");
  int nevt = tree->ReadFile("Data.txt","x");
  RooWorkspace w("w");
  w.factory("x[60,120]");  // invariant mass draw range
  w.factory("nbkg[30000, 0, 50000]"); // the number of background
  w.var("nbkg")->setVal(nevt);
  w.var("nbkg")->setMin(0.1*nevt);
  w.var("nbkg")->setMax(10*nevt);
  
  //create exponential model as two components
  w.factory("a1[ 7.5, -500, 500]");
  w.factory("a2[-1.5, -500, 500]");
  w.factory("expr::z('-(a1*x/100 + a2*(x/100)^2)', a1, a2, x)");
  w.factory("Exponential::model_bkg(z, 1)");
 
  // signal model  
  w.factory("nsig[5000, 0., 10000.0]");// the number of signal
  w.factory("mass[90, 60, 120]");// mean?
  w.factory("width[1, 0.5,10]");//sigma?
  w.factory("Gaussian::model_sig(x, mass, width)");
 
  RooAbsPdf * model_sig = w.pdf("model_sig");
 
  w.factory("SUM::model(nbkg*model_bkg, nsig*model_sig)");
  RooAbsPdf * model = w.pdf("model");//the pdf model of signal and background
 
  // create RooDataSet
  RooDataSet data("data","data", tree, *w.var("x"));
  RooFitResult *r = model->fitTo(data);
 
  // plot data and function
  RooPlot * plot = w.var("x")->frame();
  data.plotOn(plot);
  model->plotOn(plot);
  model->plotOn(plot, Components("model_bkg"),LineStyle(kDashed));
  model->plotOn(plot, Components("model_sig"),LineColor(kRed));
  plot->SetXTitle("dimuon_invariant_mass[GeV]");
  plot->SetTitleSize(0.02,"X");
  plot->SetTitleSize(0.02,"Y");
  plot->Draw();
  
  TFile *file = new TFile("fresult.root","RECREATE");
  file->cd();
  c1->Write();

}
