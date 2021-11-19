#pragma once

#include <vector>

#include "Math/Vector4D.h"

#include "UHH2/core/include/GenJet.h"
#include "UHH2/Delphes2UHH2Converter/include/Reader.h"


class GenJetReader : public Reader {
public:
  GenJetReader(std::vector<GenJet> &gen_jets);

  void read();

protected:
  void init();
  
private:
  std::vector<GenJet>& m_genJets;

  std::vector<float> *m_gen_jet_pt;
  std::vector<float> *m_gen_jet_eta;
  std::vector<float> *m_gen_jet_phi;
  std::vector<float> *m_gen_jet_mass;
};

GenJetReader::GenJetReader(std::vector<GenJet> &genJets)
  : m_genJets(genJets) {
  m_gen_jet_pt     = 0;
  m_gen_jet_eta    = 0;
  m_gen_jet_phi    = 0;
  m_gen_jet_mass   = 0;
}

void GenJetReader::init() {
  m_chain->SetBranchAddress("gen_jet_pt",&m_gen_jet_pt);
  m_chain->SetBranchAddress("gen_jet_eta",&m_gen_jet_eta);
  m_chain->SetBranchAddress("gen_jet_phi",&m_gen_jet_phi);
  m_chain->SetBranchAddress("gen_jet_mass",&m_gen_jet_mass);
}

void GenJetReader::read() {
  m_genJets.clear();

  for(size_t i = 0; i < m_gen_jet_pt->size(); ++i) {
    GenJet genJet;
    
    // Kinematic properties
    const ROOT::Math::PtEtaPhiMVector
      v4( m_gen_jet_pt->at(i),
	  m_gen_jet_eta->at(i),
	  m_gen_jet_phi->at(i),
	  m_gen_jet_mass->at(i) 
	  );
    genJet.set_pt(     v4.Pt()  );
    genJet.set_eta(    v4.Eta() );
    genJet.set_phi(    v4.Phi() );
    genJet.set_energy( v4.E()   );

    // Store GenJet object in collection written to output file
    m_genJets.push_back(genJet);
  } 
}
