#pragma once

#include <vector>

#include "Math/Vector4D.h"

#include "UHH2/core/include/Electron.h"
#include "UHH2/Delphes2UHH2Converter/include/Reader.h"


class ElectronReader : public Reader {
public:
  ElectronReader(std::vector<Electron> &electrons);

  void read();

protected:
  void init();
  
private:
  const double m_electron_mass; // electron invariant mass in GeV

  std::vector<Electron>& m_electrons;

  std::vector<float> *m_electron_pt;
  std::vector<float> *m_electron_eta;
  std::vector<float> *m_electron_phi;
};

ElectronReader::ElectronReader(std::vector<Electron> &electrons)
  : m_electron_mass(0.0005), m_electrons(electrons) {
  m_electron_pt     = 0;
  m_electron_eta    = 0;
  m_electron_phi    = 0;
}

void ElectronReader::init() {
  m_chain->SetBranchAddress("electron_pt",&m_electron_pt);
  m_chain->SetBranchAddress("electron_eta",&m_electron_eta);
  m_chain->SetBranchAddress("electron_phi",&m_electron_phi);
}

void ElectronReader::read() {
  m_electrons.clear();
  
  for(size_t i = 0; i < m_electron_pt->size(); ++i) {
    Electron electron;

    // Kinematic properties, well defined
    ROOT::Math::PtEtaPhiMVector v4( m_electron_pt->at(i),
				    m_electron_eta->at(i),
				    m_electron_phi->at(i),
				    m_electron_mass
				    );
    electron.set_pt(     v4.Pt()  );
    electron.set_eta(    v4.Eta() );
    electron.set_phi(    v4.Phi() );
    electron.set_energy( v4.E()   );

    // Store Electron object in collection written to output file
    m_electrons.push_back(electron);
  }  
}
