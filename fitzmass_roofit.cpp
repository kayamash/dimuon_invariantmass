using namespace RooFit;
 
void fitzmass_roofit(){
  std::cout<<"------------"<<endl;
  TTree *tree = new TTree("tree","tree");
  int nevt = tree->ReadFile("Data.txt","x");
  std::cout<<"----------------"<<endl; 
  RooWorkspace w("w");
  w.factory("x[60000,120000]");  // invariant mass
  std::cout<<"-------------------"<<endl;
  //w.factory("nbkg[120000, 0, 120000]");
  //w.var("nbkg")->setVal(nevt);
  //w.var("nbkg")->setMin(0.1*nevt);
  //w.var("nbkg")->setMax(10*nevt);
  
  // create exponential model as two components
  //w.factory("a1[ 7.5, -5000, 5000]");
  //w.factory("a2[-1.5, -5000, 5000]");
  //w.factory("expr::z('-(a1*x/100 + a2*(x/100)^2)', a1, a2, x)");
  //w.factory("Exponential::model_bkg(z, 1)");
 
  // signal model  
  //w.factory("nsig[100000, 0., 100000.0]");
  w.factory("mass[90000, 60000, 120000]");
  w.factory("width[1, 0.5,100]");
  w.factory("Gaussian::model_sig(x, mass, width)");
 
  RooAbsPdf * model_sig = w.pdf("model_sig");
 
  //w.factory("SUM::model(nbkg*model_bkg, nsig*model_sig)");
  RooAbsPdf * model = w.pdf("model");
 
  // create RooDataSet
  RooDataSet data("data","data", tree, *w.var("x"));
 
  RooFitResult *r = model->fitTo(data);
 
  // plot data and function
  RooPlot * plot = w.var("x")->frame();
  data.plotOn(plot);
  model->plotOn(plot);
 // model->plotOn(plot, Components("model_bkg"),LineStyle(kDashed));
  model->plotOn(plot, Components("model_sig"),LineColor(kRed));
  plot->Draw();
}
