using namespace RooFit;
 
void fitGaus3()
{
 TH1F *hist = new TH1F("hist", "hist", 20, -10, 10);
 
  Double_t tree_x;
  TTree *tree = new TTree("tree", "tree");
  tree->Branch("x", &tree_x, "x/D");
 
  TRandom3 rndm(0);
  for(int ii = 0; ii < 100; ii++) {
    float rndm_x = rndm.Gaus(0, 1);
    hist->Fill(rndm_x);
    tree_x = rndm_x;
    tree->Fill();
  }
 RooWorkspace w("w", kTRUE);
  w.factory("Gaussian::gauss(x[-10, 10], mean[0, -10, 10], sigma[1, -10, 10])");
   
  RooRealVar *x = w.var("x");
  RooAbsPdf *pdf = w.pdf("gauss");
 
  RooDataHist data_h("data_h", "binned dataset", *x, hist);
  RooDataSet data_t("data_t", "unbinned dataset", tree, *x);
 
  RooPlot* xframe = x->frame();
  data_h.plotOn(xframe);
 
  pdf->fitTo(data_h);
  pdf->plotOn(xframe);
 
  pdf->fitTo(data_t);
  pdf->plotOn(xframe,LineColor(kRed));
 
  xframe->Draw();
 w.writeToFile("GaussModel.root");
 }
