#ifndef DmpAlgFdigiTest_H
#define DmpAlgFdigiTest_H

#include "DmpVAlg.h"
//#include "DmpEvtMCPrimaryParticle.h"
#include "DmpEvtMCBgo.h"
#include "DmpEvtFdigiBgo.h"
#include "TH1F.h"
class DmpAlgFdigiTest : public DmpVAlg{
/*
 *  DmpAlgFdigiTest
 *
 */
public:
  DmpAlgFdigiTest();
  ~DmpAlgFdigiTest();

  //void Set(const std::string &type,const std::string &value);
  // if you need to set some options for your algorithm at run time. Overload Set()
  bool Initialize();
  bool ProcessThisEvent();    // only for algorithm
  bool Finalize();
public:
//DmpEvtMCPrimaryParticle *fEvtMCPrimaryParticle;
DmpEvtMCBgo *fEvtMCBgo;
DmpEvtFdigiBgo *fEvtFdigiBgo;
std::map<short,TH1F*> fFdigiMipsHist;
};

#endif
