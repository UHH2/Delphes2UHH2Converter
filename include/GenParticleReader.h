#pragma once

#include <vector>

#include "Math/Vector4D.h"

#include "UHH2/core/include/GenParticle.h"
#include "UHH2/Delphes2UHH2Converter/include/Reader.h"


class GenParticleReader : public Reader {
public:
  GenParticleReader(std::vector<GenParticle> &gps);

  void read();

protected:
  void init();
  
private:
  const double m_muon_mass; // muon invariant mass in GeV

  std::vector<GenParticle>& m_gps;

  std::vector<float> *m_gp_pt;
  std::vector<float> *m_gp_eta;
  std::vector<float> *m_gp_phi;
};

GenParticleReader::GenParticleReader(std::vector<GenParticle> &gps)
  : m_muon_mass(0.105658), m_gps(gps) {
  m_gp_pt     = 0;
  m_gp_eta    = 0;
  m_gp_phi    = 0;
}

void GenParticleReader::init() {
  m_chain->SetBranchAddress("gen_muon_pt",&m_gp_pt);
  m_chain->SetBranchAddress("gen_muon_eta",&m_gp_eta);
  m_chain->SetBranchAddress("gen_muon_phi",&m_gp_phi);
}

void GenParticleReader::read() {
  m_gps.clear();
  
  for(size_t i = 0; i < m_gp_pt->size(); ++i) {
    GenParticle gp;

    // Kinematic properties, well defined
    ROOT::Math::PtEtaPhiMVector v4( m_gp_pt->at(i),
				    m_gp_eta->at(i),
				    m_gp_phi->at(i),
				    m_muon_mass
				    );
    gp.set_pt(     v4.Pt()  );
    gp.set_eta(    v4.Eta() );
    gp.set_phi(    v4.Phi() );
    gp.set_energy( v4.E()   );

    // Store GenParticle object in collection written to output file
    m_gps.push_back(gp);
  }  
}
