#pragma once

#include <vector>

#include "Math/Vector4D.h"

#include "UHH2/core/include/Muon.h"
#include "UHH2/Delphes2UHH2Converter/include/Reader.h"


class MuonReader : public Reader {
public:
  MuonReader(std::vector<Muon> &muons);

  void read();

protected:
  void init();
  
private:
  const double m_muon_mass; // muon invariant mass in GeV

  std::vector<Muon>& m_muons;

  std::vector<float> *m_muon_pt;
  std::vector<float> *m_muon_eta;
  std::vector<float> *m_muon_phi;
};

MuonReader::MuonReader(std::vector<Muon> &muons)
  : m_muon_mass(0.105658), m_muons(muons) {
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
    ROOT::Math::PtEtaPhiMVector v4( m_muon_pt->at(i),
				    m_muon_eta->at(i),
				    m_muon_phi->at(i),
				    m_muon_mass
				    );
    muon.set_pt(     v4.Pt()  );
    muon.set_eta(    v4.Eta() );
    muon.set_phi(    v4.Phi() );
    muon.set_energy( v4.E()   );

    // std::cout << std::setprecision(6) << std::fixed;
    // std::cout << "\nPT  : " << v4.Pt()  << " | " << m_muon_pt->at(i)  << std::endl;
    // std::cout << "ETA : " << v4.Eta() << " | " << m_muon_eta->at(i) << std::endl;
    // std::cout << "PHI : " << v4.Phi() << " | " << m_muon_phi->at(i) << std::endl;
    // std::cout << "PZ  : " << v4.Pz()  << std::endl;
    // std::cout << "M   : " << v4.M() << std::endl;
    // std::cout << "E   : " << v4.E() << std::endl;
    // std::cout << "sqrt(E^2 - p^2) = " << sqrt( v4.E()*v4.E() - (v4.Pt()*v4.Pt() + v4.Pz()*v4.Pz()) ) << std::endl;

    // Store Muon object in collection written to output file
    m_muons.push_back(muon);
  }  
}
