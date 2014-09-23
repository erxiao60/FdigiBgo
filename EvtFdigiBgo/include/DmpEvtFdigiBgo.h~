/*   $Id: DmpEvtFdigiBgo.h, 2014-08-26 19:44:49+08:00 DAMPE $
 *--------------------------------------------------------
 *  Author(s):
 *  Yong (zhzhy@mail.ustc.edu.cn) 26/08/2014
 *--------------------------------------------------------
*/

#ifndef DmpEvtFdigiBgo_H
#define DmpEvtFdigiBgo_H

#include "TObject.h"

class DmpEvtFdigiBgo : public TObject{
/*
 *  DmpEvtFdigiBgo
 *
 */
public:
  DmpEvtFdigiBgo();
  ~DmpEvtFdigiBgo();

  void Reset();
  const short nHits() const {return fGlobalPmtID.size();}
  const short GetBar(short Gpmt) const {return Gpmt%22;}
  const short GetSide(short Gpmt) const{return ((short)(Gpmt/22))%2;}
  const short GetLayer(short Gpmt) const{return (short)(Gpmt/22/2);}
   
  // void AddFdigiHits(short &id, double &e, double &x,double &y, double &z,double &adc);

public:

  std::vector <short>     fGlobalPmtID;   // unique sensitive detector(minimum detector unit) ID.Pmt End of Bgo bar. fGlobalPmtID =  (layer*2+side)*22+bar;
  std::vector <short>    fADC;        // unit ADC
  
  ClassDef(DmpEvtFdigiBgo,1)

};

#endif
