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
  std::vector<GenParticle>& m_gps;

  std::vector<int> *m_id;
  std::vector<int> *m_status;
  std::vector<float> *m_pt;
  std::vector<float> *m_eta;
  std::vector<float> *m_phi;
  std::vector<float> *m_mass;
};

GenParticleReader::GenParticleReader(std::vector<GenParticle> &gps)
  : m_gps(gps) {
  m_id     = 0;
  m_status = 0;
  m_pt     = 0;
  m_eta    = 0;
  m_phi    = 0;
  m_mass   = 0;
}

void GenParticleReader::init() {
  m_chain->SetBranchAddress("gen_particle_id",&m_id);
  m_chain->SetBranchAddress("gen_particle_status",&m_status);
  m_chain->SetBranchAddress("gen_particle_pt",&m_pt);
  m_chain->SetBranchAddress("gen_particle_eta",&m_eta);
  m_chain->SetBranchAddress("gen_particle_phi",&m_phi);
  m_chain->SetBranchAddress("gen_particle_mass",&m_mass);
}

void GenParticleReader::read() {
  m_gps.clear();

  for(size_t i = 0; i < m_id->size(); ++i) {
    if( m_status->at(i) == 1 ) { // stable generator particles
      GenParticle gp;

      const ROOT::Math::PtEtaPhiMVector
	v4( m_pt->at(i), m_eta->at(i), m_phi->at(i), m_mass->at(i) );
      gp.set_pt(     v4.Pt()  );
      gp.set_eta(    v4.Eta() );
      gp.set_phi(    v4.Phi() );
      gp.set_energy( v4.E()   );
      gp.set_pdgId( m_id->at(i) );
      
      // Store GenParticle object in collection written to output file
      m_gps.push_back(gp);
    }
  }  
  
}
