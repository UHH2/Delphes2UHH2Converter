#pragma once

#include "TChain.h"


class Reader {
public:
  Reader() 
    : m_chain(0) {}

  void set_chain(TChain* chain) {
    m_chain = chain;
    init();
  }
  virtual void read() = 0;

  
protected:
  virtual void init() = 0;

  TChain* m_chain;
};
