#pragma once

#include <vector>

#include "Math/Vector4D.h"

#include "UHH2/core/include/Jet.h"
#include "UHH2/Delphes2UHH2Converter/include/Reader.h"


class JetReader : public Reader {
public:
  JetReader(std::vector<Jet> &jets);

  void read();

protected:
  void init();
  
private:
  std::vector<Jet>& m_jets;

  std::vector<float> *m_jet_pt;
  std::vector<float> *m_jet_eta;
  std::vector<float> *m_jet_phi;
  std::vector<float> *m_jet_mass;
  std::vector<int>   *m_jet_btag;
  std::vector<int>   *m_jet_flavor;
};

JetReader::JetReader(std::vector<Jet> &jets)
  : m_jets(jets) {
  m_jet_pt     = 0;
  m_jet_eta    = 0;
  m_jet_phi    = 0;
  m_jet_mass   = 0;
  m_jet_btag   = 0;
  m_jet_flavor = 0;
}

void JetReader::init() {
  m_chain->SetBranchAddress("jet_pt",&m_jet_pt);
  m_chain->SetBranchAddress("jet_eta",&m_jet_eta);
  m_chain->SetBranchAddress("jet_phi",&m_jet_phi);
  m_chain->SetBranchAddress("jet_mass",&m_jet_mass);
  m_chain->SetBranchAddress("jet_btag",&m_jet_btag);
  m_chain->SetBranchAddress("jet_flavor",&m_jet_flavor);
}

void JetReader::read() {
  m_jets.clear();

  for(size_t i = 0; i < m_jet_pt->size(); ++i) {
    Jet jet;
    
    // Kinematic properties
    const ROOT::Math::PtEtaPhiMVector
      v4( m_jet_pt->at(i),
	  m_jet_eta->at(i),
	  m_jet_phi->at(i),
	  m_jet_mass->at(i) 
	  );
    jet.set_pt(     v4.Pt()  );
    jet.set_eta(    v4.Eta() );
    jet.set_phi(    v4.Phi() );
    jet.set_energy( v4.E()   );

    // b-tagging: in Delphes tree, we have 0 or 1 (which tagger?)
    // Put some sensible translation:
    if( m_jet_btag->at(i) == 1 ) {
      jet.set_btag_DeepFlavour_probbb( 0.9 );
      jet.set_btag_DeepFlavour_probb( 0.9 );
      jet.set_btag_DeepFlavour_problepb( 0.9 );
      jet.set_btag_DeepCSV_probb(     0.9 );
      jet.set_btag_combinedSecondaryVertex( 0.9 );
      jet.set_btag_combinedSecondaryVertexMVA( 0.9 );
    } else {
      jet.set_btag_DeepFlavour_probbb( 0.1 );
      jet.set_btag_DeepFlavour_probb( 0.1 );
      jet.set_btag_DeepFlavour_problepb( 0.1 );
      jet.set_btag_DeepCSV_probb(     0.1 );
      jet.set_btag_combinedSecondaryVertex( 0.1 );
      jet.set_btag_combinedSecondaryVertexMVA( 0.1 );
    }

    // Need to check which flavour definition is used in Delphes
    jet.set_pdgId( m_jet_flavor->at(i) );
    jet.set_partonFlavour( m_jet_flavor->at(i) );
    jet.set_hadronFlavour( m_jet_flavor->at(i) );

    // Store Jet object in collection written to output file
    m_jets.push_back(jet);
  } 
}
