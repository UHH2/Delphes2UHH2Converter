#pragma once

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TString.h"

#include "UHH2/Delphes2UHH2Converter/include/Reader.h"



class InputTreeProcessor {
public:
  InputTreeProcessor(const TString& file_name, const TString& tree_name);
  
  void add_reader(Reader* reader) {
    reader->set_chain(m_chain);
    m_readers.push_back(reader);
  }

  bool operator()(int n_max_evts = -1) {
    return load_next_event(n_max_evts);
  }
  bool load_next_event(int n_max_evts = -1);
  long entry() const { return m_entry; }


private:
  TChain* m_chain;
  long m_entry;
  std::vector<Reader*> m_readers;
};


InputTreeProcessor::InputTreeProcessor(const TString& file_name, const TString& tree_name)
    : m_entry(-1) {
  std::cout << "Setting up TChain '" << tree_name << "' from '" << file_name << "'" << std::endl;
  m_chain = new TChain(tree_name);
  m_chain->Add(file_name);
}


bool InputTreeProcessor::load_next_event(int n_max_evts) {
  m_entry++;
  if( m_entry == n_max_evts ) {
    return false;
  }
  if( m_entry % 2 == 0 ) {
    std::cout << "Processing event " << m_entry << std::endl;
  }
  if( m_chain->GetEntry(m_entry) ) {
    for(auto& reader: m_readers) {
      reader->read();
    }
    return true;
  } else {
    return false;
  }
}
