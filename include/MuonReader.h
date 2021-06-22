#pragma once

#include <vector>

#include "UHH2/core/include/Muon.h"
#include "UHH2/Delphes2UHH2Converter/include/Reader.h"


class MuonReader : public Reader {
public:
  MuonReader(std::vector<Muon> &muons);

  void read();

protected:
  void init();
  
private:
  std::vector<Muon>& m_muons;

  std::vector<float> *m_muon_pt;
  std::vector<float> *m_muon_eta;
  std::vector<float> *m_muon_phi;
};

MuonReader::MuonReader(std::vector<Muon> &muons)
  : m_muons(muons) {
  m_muon_pt     = 0;
  m_muon_eta    = 0;
  m_muon_phi    = 0;
}

void MuonReader::init() {
  m_chain->SetBranchAddress("muon_pt",&m_muon_pt);
  m_chain->SetBranchAddress("muon_eta",&m_muon_eta);
  m_chain->SetBranchAddress("muon_phi",&m_muon_phi);
}

void MuonReader::read() {
  m_muons.clear();
  
  for(size_t i = 0; i < m_muon_pt->size(); ++i) {
    Muon muon;

    // Kinematic properties, well defined
    muon.set_pt(  m_muon_pt->at(i)     );
    muon.set_eta( m_muon_eta->at(i)    );
    muon.set_phi( m_muon_phi->at(i)    );

    // Store Muon object in collection written to output file
    m_muons.push_back(muon);
  }  
}
