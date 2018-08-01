 void root_to_txt(){
  TFile file("hist.root");
  TTree *tree = (TTree*)file.Get("tree");
  Double_t mass = 0.;
  tree->SetBranchAddress("muon_invariant_mass",&mass);
  Int_t eventnumber=tree->GetEntries();
  std::ofstream File("Data.txt");
  for(Int_t i = 0;i < eventnumber;i++){
   tree->GetEntry(i);
   if(mass != 0){
    File<<mass<<"\n";
   }
  }
 }
