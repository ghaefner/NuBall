#ifndef __LOOKUP_HH__
#define __LOOKUP_HH__

// This class handles the lookup table. This gives us functions IsGe, IsBGO,
// IsLaBr3, IsBad and GetModule, which we can use to find out what a given
// channel is. There are a whole load of other things filled, but they aren't
// used in the code, so we haven't provided a public method to access them.

#include <map>
#include "idmap_238u.hxx"

class Lookup {

 private:
   std::map<int, bool> isGe;
   std::map<int, bool> isBGO;
   std::map<int, bool> isLaBr3;
   std::map<int, bool> isBad;
   std::map<int, int> module;

   // We don't actually use the following
   std::map<int, int> ring;
   std::map<int, int> alveole;
   std::map<int, bool> isThere;
   std::map<int, bool> isPhase1;
   std::map<int, bool> isPhase1_Ge;
   std::map<int, bool> isPhase1_BGO;
   std::map<int, bool> isClover;
   std::map<int, bool> isClover_Ge;
   std::map<int, bool> isClover_BGO;
   std::map<int, bool> isMadrid;
   std::map<int, bool> isEDEN;
   std::map<int, bool> isPulse;
   std::map<int, bool> bigWalk;
   std::map<int, bool> isRing1;
   std::map<int, bool> isRing2;
   std::map<int, bool> isRing3;
   std::map<int, bool> isRing4;

 public:


   Lookup() {
      //List of detectors to remove from the stream
      isBad[8] = true; //Bad resolution
      isBad[70] = true; //Bad signal

      for (int i=0; i < 255; i++) {
         int fasterChan=idmap[i][0]; //The nth faster channel detTypeber in the list
         if (fasterChan > 0) {
            int detID=idmap[i][1]; // The ID e.g. 40810 (phase 1 bgo)
            isThere[fasterChan]=true;
            int ringNr=(detID/10000);
            int posNr=((detID/100) % 100);
            int detType=((detID/10) % 10); //0 = ge, 1 = bgo, 1 or 2 for LaBr3
            int segm=(detID % 10);
            ring[fasterChan]=ringNr;
            alveole[fasterChan]=posNr;
            if (ringNr == 1) isRing1[fasterChan]=true;
            if (ringNr == 2) isRing2[fasterChan]=true;
            if (ringNr == 3) isRing3[fasterChan]=true;
            if (ringNr == 4) isRing4[fasterChan]=true;

            // Ge and BGO first
            if ((ringNr == 2) || (ringNr == 3)) {
               isClover[fasterChan]=true;
               if (detType==0) {isGe[fasterChan]=true; isClover_Ge[fasterChan]=true;}
               else {
                  isClover_BGO[fasterChan]=true; isBGO[fasterChan]=true; isGe[fasterChan]=false;
                  if (fasterChan == 5) {isClover_BGO[fasterChan]=false; isBGO[fasterChan]=false;} //turn off bgo 5
               }
               module[fasterChan]=(ringNr-2)*12+posNr;
            }
            
            if (ringNr == 4) {
               if (detType==0) {isGe[fasterChan]=true; isPhase1_Ge[fasterChan]=true;}
               isClover[fasterChan]=true;
               if (detType==1) {isPhase1_BGO[fasterChan]=true; isBGO[fasterChan]=true;}
               module[fasterChan]=(ringNr-2)*12+posNr;
            }
            
            if (ringNr == 1) {
               isLaBr3[fasterChan]=true;
               if (segm == 2) {isMadrid[fasterChan]=true;}
            }
         }
         
      }

      isEDEN[250]=true;
      isPulse[251]=true;
      bigWalk[3]=true;
      bigWalk[5]=true;
      bigWalk[7]=true;
      bigWalk[9]=true;
      bigWalk[13]=true;
      bigWalk[15]=true;
      bigWalk[19]=true;


      std::map<int,int> idmapr;
      std::map<int,int> arraypos;
      for (int fasterChan=1; fasterChan < 255; fasterChan++) {
         //which posNrtion in the list of ADC's and QDC's is the fasterChan?
         for (int i=0; i < 255; i++) {
            if (idmap[i][0]==fasterChan) {idmapr[fasterChan]=i; arraypos[fasterChan]=idmap[i][1];}
         }
         //cout << fasterChan << "  " << idmapr[fasterChan] << endl;
      }

      std::map<int,int> modpos;
      for (int i=1; i <=34; i++) {
         int ringNr=((i-1)/12)+2;
         int pos=(i % 12);
         modpos[i]=ringNr*10000+pos*100;
         //cout << i << " "<< modpos[i]<<endl;
      }

   };

   Bool_t IsGe(UInt_t id) {
      return(isGe[id]);
   };
   
   Bool_t IsBGO(UInt_t id) {
      return(isBGO[id]);
   };
   
   Bool_t IsLaBr3(UInt_t id) {
      return(isLaBr3[id]);
   };

   Int_t GetModule(UInt_t id) {
      return(module[id]);
   };

   Bool_t IsBad(UInt_t id) {
      return(isBad[id]);
   };
};

#endif
