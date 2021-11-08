#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TString.h"
#include "TTree.h"

#include "UHH2/core/include/Tags.h"
#include "UHH2/core/include/Electron.h"
#include "UHH2/core/include/GenInfo.h"
#include "UHH2/core/include/GenJet.h"
#include "UHH2/core/include/GenParticle.h"
#include "UHH2/core/include/Jet.h"
#include "UHH2/core/include/MET.h"
#include "UHH2/core/include/Muon.h"
#include "UHH2/core/include/PrimaryVertex.h"
#include "UHH2/common/include/Utils.h"

#include "UHH2/Delphes2UHH2Converter/include/InputTreeProcessor.h"
#include "UHH2/Delphes2UHH2Converter/include/Reader.h"
#include "UHH2/Delphes2UHH2Converter/include/GenJetReader.h"
#include "UHH2/Delphes2UHH2Converter/include/GenParticleReader.h"
#include "UHH2/Delphes2UHH2Converter/include/JetReader.h"
#include "UHH2/Delphes2UHH2Converter/include/MuonReader.h"
#include "UHH2/Delphes2UHH2Converter/include/ElectronReader.h"



void run(const TString& in_file_name,
	 const TString& in_tree_name,
	 const TString& out_file_name,
	 int n_evts) {

  // Define the event content
  bool isRealData = false;
  int run = 1;
  int luminosityBlock = 0;
  long long event = 0;
  std::string year = "2018";
  float rho = 22.6;
  float beamspot_x0 = 0;
  float beamspot_y0 = 0;
  float beamspot_z0 = 0;
  GenInfo genInfo;
  bool passEcalBadCalib = true;
  float prefiringWeight = 1.;
  float prefiringWeightUp = 1.;
  float prefiringWeightDown = 1.;

  std::vector<std::string> triggerNames(1,"dummy_trigger");
  std::vector<bool> triggerResults(1,true);
  std::vector<int> triggerPrescales(1,1);
  std::vector<int> triggerPrescalesL1min(1,1);
  std::vector<int> triggerPrescalesL1max(1,1);
  
  std::vector<PrimaryVertex> pvs;
  std::vector<Electron> electrons;
  std::vector<Muon> muons;
  std::vector<Jet> jets;
  std::vector<GenJet> genjets;
  std::vector<GenParticle> genparticles;
  MET MEt;  
  

  // Configure the output tree
  TTree* out_tree = new TTree("UHH2TreeFromDelphes","UHH2TreeFromDelphes");

  out_tree->Branch("run",&run,"run/I");
  out_tree->Branch("event",&event,"event/L");
  out_tree->Branch("luminosityBlock",&luminosityBlock,"luminosityBlock/I");
  out_tree->Branch("isRealData",&isRealData,"isRealData/O");
  out_tree->Branch("year",&year);
  out_tree->Branch("rho",&rho,"rho/F");
  out_tree->Branch("beamspot_x0",&beamspot_x0,"beamspot_x0/F");
  out_tree->Branch("beamspot_y0",&beamspot_y0,"beamspot_y0/F");
  out_tree->Branch("beamspot_z0",&beamspot_z0,"beamspot_z0/F");
  out_tree->Branch("genInfo",&genInfo);
  out_tree->Branch("passEcalBadCalib",&passEcalBadCalib,"passEcalBadCalib/O");
  out_tree->Branch("prefiringWeight",&prefiringWeight,"prefiringWeight/F");
  out_tree->Branch("prefiringWeightUp",&prefiringWeightUp,"prefiringWeightUp/F");
  out_tree->Branch("prefiringWeightDown",&prefiringWeightDown,"prefiringWeightDown/F");

  out_tree->Branch("triggerNames","std::vector<std::string>",&triggerNames);
  out_tree->Branch("triggerResults","std::vector<bool>",&triggerResults);
  out_tree->Branch("triggerPrescales","std::vector<int>",&triggerPrescales);
  out_tree->Branch("triggerPrescalesL1min","std::vector<int>",&triggerPrescalesL1min);
  out_tree->Branch("triggerPrescalesL1max","std::vector<int>",&triggerPrescalesL1max);

  out_tree->Branch("offlineSlimmedPrimaryVertices","std::vector<PrimaryVertex>",&pvs);
  out_tree->Branch("slimmedElectronsUSER","std::vector<Electron>",&electrons);
  out_tree->Branch("slimmedMuonsUSER","std::vector<Muon>",&muons);
  out_tree->Branch("jetsAk4Puppi","std::vector<Jet>",&jets);
  out_tree->Branch("slimmedGenJets","std::vector<GenJet>",&genjets);
  out_tree->Branch("GenParticles","std::vector<GenParticle>",&genparticles);
  out_tree->Branch("slimmedMETsPuppi",&MEt);


  // Set up the readers
  GenParticleReader gen_particle_reader(genparticles);
  GenJetReader gen_jet_reader(genjets);
  JetReader jet_reader(jets);
  MuonReader muon_reader(muons);
  ElectronReader electron_reader(electrons);

  InputTreeProcessor in_tree_processor(in_file_name,in_tree_name);
  in_tree_processor.add_reader(&gen_particle_reader);
  in_tree_processor.add_reader(&gen_jet_reader);
  in_tree_processor.add_reader(&jet_reader);
  in_tree_processor.add_reader(&muon_reader);
  in_tree_processor.add_reader(&electron_reader);

  
  // Loop over the events
  while( in_tree_processor( n_evts ) ) {

    event++;
    //std::cout << "Arrived at event " << event << std::endl;

    genInfo.clear_weights();
    genInfo.add_weight(1.);

    pvs.clear();
    pvs.push_back(PrimaryVertex());

    out_tree->Fill();
  }

  std::cout << "Done. Writing tree to file" << std::endl;

  TFile out_file(out_file_name,"RECREATE");
  out_file.WriteTObject(out_tree);
  out_file.Close();

}


int main(int argc, char** argv) {

  if( argc < 3 ) {
    std::cout << "No input file given." << std::endl;
    std::cout << "Execute as 'convertDelphes2UHH2 <input_file.root> <output_file.root> [<max. number events>]'." << std::endl;
    return 1;
  }

  const TString delphes_file( argv[1] );
  const TString delphes_tree = "Delphes";
  const TString out_file( argv[2] );
  
  int n_max_evts = -1;
  if( argc > 3 ) {
    try {
      n_max_evts = std::stoi( argv[3] );
    } catch (std::exception const &e) {
      std::cerr << "ERROR: second argument given is not an integer number" << std::endl;
      std::cout << "Execute as 'convertDelphes2UHH2 <input_file.root> <output_file.root> [<max. number events>]'." << std::endl;
      return 1;
    }
  }

  run(delphes_file,delphes_tree,out_file,n_max_evts);

  return 0;
}
