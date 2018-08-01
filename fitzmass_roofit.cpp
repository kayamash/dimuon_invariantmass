using namespace RooFit;
 
void fitzmass_roofit(){
  TCanvas *c1 = new TCanvas("dimuon_invariant_mass[GeV]","dimuon_invariant_mass[GeV]",1600*1.1,900*1.1);
  TTree *tree = new TTree("tree","tree");
  int nevt = tree->ReadFile("Data.txt","x");
  RooWorkspace w("w");
  w.factory("x[0.5,120]");  // invariant mass draw range
  w.factory("nbkg[30000, 0, 500000]"); // the number of background
  w.var("nbkg")->setVal(nevt);
  w.var("nbkg")->setMin(0.1*nevt);
  w.var("nbkg")->setMax(10*nevt);
  
  //create exponential model as two components
  w.factory("a1[ 7.5, -500, 500]");
  w.factory("a2[-1.5, -500, 500]");
  w.factory("expr::z('-(a1*x/100 + a2*(x/100)^2)', a1, a2, x)");
  w.factory("Exponential::model_bkg(z, 1)");

  // signal model of z 
  w.factory("nsigz[5000, 0., 100000.0]");// the number of signal
  w.factory("massz[90, 70, 100]");// mean?
  w.factory("widthz[1, 0.5,10]");//sigma?
  w.factory("Gaussian::model_sigz(x, massz, widthz)");
 
  RooAbsPdf * model_sig_z = w.pdf("model_sigz");

  // signal model of jp 
  w.factory("nsigjp[5000, 0., 100000.0]");// the number of signal
  w.factory("massjp[3, 1, 5]");// mean?
  w.factory("widthjp[1, 0.5,10]");//sigma?
  w.factory("Gaussian::model_sigjp(x, massjp, widthjp)");
 
  RooAbsPdf * model_sig_jp = w.pdf("model_sigjp");

  // signal model of Y
  w.factory("nsigy[5000, 0., 100000.0]");// the number of signal
  w.factory("massy[9, 8, 10]");// mean?
  w.factory("widthy[1, 0.5,10]");//sigma?
  w.factory("Gaussian::model_sigy(x, massy, widthy)");
 
  RooAbsPdf * model_sig_y1s = w.pdf("model_sigy1s");

  // signal model of some1
  w.factory("nsigsome1[5000, 0., 100000.0]");// the number of signal
  w.factory("masssome1[15, 12, 18]");// mean?
  w.factory("widthsome1[1, 0.5,10]");//sigma?
  w.factory("Gaussian::model_sigsome1(x, masssome1, widthsome1)");
 
  RooAbsPdf * model_sig_some1 = w.pdf("model_sigsome1");

  // signal model of some2
  w.factory("nsigsome2[5000, 0., 100000.0]");// the number of signal
  w.factory("masssome2[20, 18, 24]");// mean?
  w.factory("widthsome2[1, 0.5,10]");//sigma?
  w.factory("Gaussian::model_sigsome2(x, masssome2, widthsome2)");
 
  RooAbsPdf * model_sig_some2 = w.pdf("model_sigsome2");

  // signal model of some3
  w.factory("nsigsome3[5000, 0., 100000.0]");// the number of signal
  w.factory("masssome3[35, 30, 40]");// mean?
  w.factory("widthsome3[1, 0.5,10]");//sigma?
  w.factory("Gaussian::model_sigsome3(x, masssome3, widthsome3)");
 
  RooAbsPdf * model_sig_some3 = w.pdf("model_sigsome3");

  w.factory("SUM::model(nbkg*model_bkg, nsigz*model_sigz, nsigjp*model_sigjp,nsigy*model_sigy, nsigsome1*model_sigsome1, nsigsome2*model_sigsome2, nsigsome3*model_sigsome3)");
  RooAbsPdf * model = w.pdf("model");//the pdf model of signal and background
 
  // create RooDataSet
  RooDataSet data("data","data", tree, *w.var("x"));
  RooFitResult *r = model->fitTo(data);
 
  // plot data and function
  RooPlot * plot = w.var("x")->frame();
  data.plotOn(plot);
  model->plotOn(plot);
  model->plotOn(plot, Components("model_bkg"),LineStyle(kDashed));
  model->plotOn(plot, Components("model_sigz"),LineColor(kRed));
  model->plotOn(plot, Components("model_sigjp"),LineColor(kGray));
  model->plotOn(plot, Components("model_sigy"),LineColor(kGreen));
  model->plotOn(plot, Components("model_sigsome1"),LineColor(kOrange));
  model->plotOn(plot, Components("model_sigsome2"),LineColor(kYellow));
  plot->SetXTitle("dimuon_invariant_mass[GeV]");
  plot->SetTitle("dimuon_invariant_mass");
  plot->SetTitleSize(0.03,"X");
  plot->SetTitleSize(0.03,"Y");
  plot->SetTitleOffset(1.5,"X");
  plot->SetTitleOffset(1.7,"Y");
  plot->Draw();

  w.Print();


  TFile *file = new TFile("fresult.root","RECREATE");
  file->cd();
  c1->Write();
  w.writeToFile("GaussModel.root");
  delete c1;

}
