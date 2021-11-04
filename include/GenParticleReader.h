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
  const double m_electron_mass; // electron invariant mass in GeV

  std::vector<GenParticle>& m_gps;

  std::vector<int> *m_mu_id;
  std::vector<float> *m_mu_pt;
  std::vector<float> *m_mu_eta;
  std::vector<float> *m_mu_phi;
  std::vector<int> *m_el_id;
  std::vector<float> *m_el_pt;
  std::vector<float> *m_el_eta;
  std::vector<float> *m_el_phi;

  std::vector<size_t> idx_of_cleaned_collection(const std::vector<float>& pts, const std::vector<float>& etas) const;
  bool is_equal(float a, float b) const;
};

GenParticleReader::GenParticleReader(std::vector<GenParticle> &gps)
  : m_muon_mass(0.105658), m_electron_mass(0.0005), m_gps(gps) {
  m_mu_id     = 0;
  m_mu_pt     = 0;
  m_mu_eta    = 0;
  m_mu_phi    = 0;
  m_el_id     = 0;
  m_el_pt     = 0;
  m_el_eta    = 0;
  m_el_phi    = 0;
}

void GenParticleReader::init() {
  m_chain->SetBranchAddress("gen_muon_id",&m_mu_id);
  m_chain->SetBranchAddress("gen_muon_pt",&m_mu_pt);
  m_chain->SetBranchAddress("gen_muon_eta",&m_mu_eta);
  m_chain->SetBranchAddress("gen_muon_phi",&m_mu_phi);
  m_chain->SetBranchAddress("gen_electron_id",&m_el_id);
  m_chain->SetBranchAddress("gen_electron_pt",&m_el_pt);
  m_chain->SetBranchAddress("gen_electron_eta",&m_el_eta);
  m_chain->SetBranchAddress("gen_electron_phi",&m_el_phi);
}

void GenParticleReader::read() {
  m_gps.clear();

  // HACK: clean gen muon collection from duplicates
  std::vector<size_t> idxs = idx_of_cleaned_collection(*m_mu_pt,*m_mu_eta);

  for(size_t i = 0; i < idxs.size(); ++i) {
    const size_t idx = idxs.at(i);

    GenParticle gp;

    ROOT::Math::PtEtaPhiMVector v4( m_mu_pt->at(idx),
				    m_mu_eta->at(idx),
				    m_mu_phi->at(idx),
				    m_muon_mass
				    );
    gp.set_pt(     v4.Pt()  );
    gp.set_eta(    v4.Eta() );
    gp.set_phi(    v4.Phi() );
    gp.set_energy( v4.E()   );
    gp.set_pdgId( m_mu_id->at(idx) );

    // Store GenParticle object in collection written to output file
    m_gps.push_back(gp);
  }  
  
  for(size_t i = 0; i < m_el_pt->size(); ++i) {
    GenParticle gp;

    ROOT::Math::PtEtaPhiMVector v4( m_el_pt->at(i),
  				    m_el_eta->at(i),
  				    m_el_phi->at(i),
  				    m_electron_mass
  				    );
    gp.set_pt(     v4.Pt()  );
    gp.set_eta(    v4.Eta() );
    gp.set_phi(    v4.Phi() );
    gp.set_energy( v4.E()   );
    gp.set_pdgId( m_el_id->at(i) );

    // Store GenParticle object in collection written to output file
    m_gps.push_back(gp);
  }  
}


// HOTFIX
// For some reason, we have duplicated entries in the gen_muons
// Return indices of cleaned (=duplicates removed) collection
std::vector<size_t> GenParticleReader::idx_of_cleaned_collection(const std::vector<float>& pts, const std::vector<float>& etas) const {

  std::vector<size_t> idx;
  for(size_t i = 0; i < pts.size(); ++i) {
    const float pt  = pts.at(i);
    const float eta = etas.at(i);
    bool is_duplicate = false;
    for(size_t j = 0; j < idx.size(); ++j) {
      if( is_equal(pt,pts.at(idx.at(j))) && is_equal(eta,etas.at(idx.at(j))) ) {
	is_duplicate = true;
	break;
      }
    }
    if( !is_duplicate ) {
      idx.push_back(i);
    }
  }

  return idx;
}

bool GenParticleReader::is_equal(float a, float b) const {
  return std::abs((a-b)/a) < 0.05;
}
