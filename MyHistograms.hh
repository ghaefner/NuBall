#ifndef __MY_HISTOGRAMS_HH__
# define __MY_HISTOGRAMS_HH__

// This class stores all the histograms that we will use. They are public, so
// it is easy to fill them

#include <iostream>

#include <TFile.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TH3S.h>

#include "Lookup.hh"

class MyHistograms {
 private:
   Lookup *lookup;
   std::string rootfile;
   const int NCHANS = 2000;
   const int NDETS = 255;
      
 public:
   TH2F *HK;
   TH1D *E_Ge;
   TH1D *Ec_Ge;
   TH1D *T_Ge;
   TH1D *T_BGO;
   TH1D *T_LaBr;
   TH1D *dE_Ge;
   TH1D *pMult_LaBr3;
   TH1D *dMult_LaBr3;
   TH1D *dMult_GeBGO;
   TH1D *pMult_total;
   TH1D *cMult_Ge;
   TH1D *E_total;
   TH1D *E_BGO;
   TH1D *E_LaBr3;
   TH2F *TE_Ge;
   TH2F *TE_LaBr3;
   TH2F *TE_BGO;
   TH2F *pEpE_m4_Ge;
   TH2F *pEpE_m5_Ge;
   TH2F *pEpE_m6_Ge;
   TH2F *dEdE_100_400_Ge;
   TH2F *dEdE_400_2800_Ge;
   TH2F *dd200;
   TH2F *dd100;
   TH2F *pEdE_100_400_Ge;
   //TH2F *dp200;
   TH3S *dEdET_100_400_Ge;
   TH2F *dEtotaldE_100_400_Ge;
   TH1D *pileup_Ge;
   TH1D **tSpecs;
   TH1D **eSpecs;
   TH1D **pSpecs;

   //--------------------------------------------------------------------------
   // Constructor - store the pointer to the lookup table, and the name of the
   // output root file and create all the histograms
   MyHistograms(Lookup *_lookup, const std::string _rootfile) {
      lookup = _lookup;
      rootfile = _rootfile;

      HK = new TH2F("HK","Multiplicity vs. total energy",25,0,25,250,0,25000); // 100 keV/bin
      E_Ge = new TH1D("E_Ge","All dirty Ge",8192,0,8192); //All dirty Ge 
      Ec_Ge = new TH1D("Ec_Ge","All clean Ge (suppressed anti-coincidences)",4000,0,4000); //All clean Ge (suppressed anti-coincidences)

      T_Ge = new TH1D("T_Ge","All Ge times",2800,0,2800); //All Ge times
      T_BGO = new TH1D("T_BGO","All BGO times",2800,0,2800); //All BGO times
      T_LaBr = new TH1D("T_LaBr","All LaBr3 times",2800,0,2800); //All Labr3 times

      dE_Ge = new TH1D("dE_Ge","All delayed ge",4096,0,4096); //All delayed Ge 
      dMult_GeBGO = new TH1D("dMult_GeBGO","Delayed multiplicity",50,0,50); //All Ge times
      pMult_total = new TH1D("pMult_total","Total event multiplicity",50,0,50); //Event Mult 
      cMult_Ge = new TH1D("cMult_Ge","Clean Ge multlicity",50,0,50); //Clean Ge Mult 
      E_total = new TH1D("E_total","Event sum energy",512,0,51200); //Event Sum Energy (100keV/bin)
      pMult_LaBr3 = new TH1D("pMult_LaBr3","Prompt LaBr multlicity",50,0,50); //Prompt LaBr Mult
      dMult_LaBr3 = new TH1D("dMult_LaBr3","Delayed LaBr multlicity",50,0,50); //Delayed LaBr Mult
      

      E_BGO = new TH1D("E_BGO","All BGO energies",4000,0,4000); //All E_BGO 
      E_LaBr3 = new TH1D("E_LaBr3","All laBr3 energies",8192,0,8192); //All E_LaBr3 

      TE_Ge = new TH2F("TE_Ge","Energy vs. time for all Ge",2800,0,2800,4096,0,4096);
      TE_LaBr3 = new TH2F("TE_LaBr3","Energy vs. time for all LaBr3",1200,0,1200,4096,0,4096);
      TE_BGO = new TH2F("TE_BGO","Energy vs. time for all BGO",1200,0,1200,4096,0,4096);

      pEpE_m4_Ge = new TH2F("pEpE_m4_Ge","Prompt gamma-gamma matrix (CMult >= 4)",4096,0,4096,4096,0,4096); //
      pEpE_m5_Ge = new TH2F("pEpE_m5_Ge","Prompt gamma-gamma matrix (CMult >= 5)",4096,0,4096,4096,0,4096); //
      pEpE_m6_Ge = new TH2F("pEpE_m6_Ge","Prompt gamma-gamma matrix (CMult >= 6)",4096,0,4096,4096,0,4096); //

      dEdE_100_400_Ge = new TH2F("dEdE_100_400_Ge","Delayed gamma-gamma matrix (short)",4096,0,4096,4096,0,4096); //
      dEdE_400_2800_Ge = new TH2F("dEdE_400_2800_Ge","Delayed gamma-gamma matrix (long)",4096,0,4096,4096,0,4096); //
      //dd200 = new TH2F("dd200","dd200",4096,0,4096,4096,0,4096); //
      //dd100 = new TH2F("dd100","dd100",4096,0,4096,4096,0,4096); //

      pEdE_100_400_Ge = new TH2F("pEdE_100_400_Ge","Prompt matrix (time < 400 ns)",4096,0,4096,4096,0,4096);
      //dp200 = new TH2F("dp200","Prompt matrix (time < 200 ns)",4096,0,4096,4096,0,4096); 
      dEdET_100_400_Ge = new TH3S("dEdET_100_400_Ge","Delayed energy-energy-time cube",4096,0,4096,4096,0,4096,28,0,28); 
      dEtotaldE_100_400_Ge = new TH2F("dEtotaldE_100_400_Ge","Delayed sum energy vs. delayed Ge energy matrix",4096,0,4096,4096,0,4096);       
      pileup_Ge = new TH1D("pileup_Ge","All dirty Ge but with pileup bit set",4000,0,4000); //All dirty ge but with pileup bit set


      tSpecs = new TH1D*[NDETS+1];
      eSpecs = new TH1D*[NDETS+1];
      pSpecs = new TH1D*[NDETS+1];
      for (int i = 1; i <= NDETS; i++) {
         TString name, title;
         // RAW ENERGY AND TIME SINGLES SPECTRA
         name = Form("e%d", i); //Spectrum i should be fasterchan here for coherent numbering
         title = Form("Energy for channel %d", i);
         eSpecs[i] = new TH1D(name.Data(), title.Data(), NCHANS, 0, NCHANS);
         //time spectra
         name = Form("t%d", i);
         title = Form("Time relative to beam pulse for channel %d", i);
         tSpecs[i] = new TH1D(name.Data(), title.Data(), 400, 0, 400);
         //pileup spectra
         name = Form("p%d", i);
         title = Form("Pileup spectrum for channel %d", i);
         pSpecs[i] = new TH1D(name.Data(), title.Data(), NCHANS, 0, NCHANS);
      }
   };

   //--------------------------------------------------------------------------
   // Destructor - delete all the histograms
   ~MyHistograms() {
      delete HK;
      delete E_Ge;
      delete Ec_Ge;
      delete T_Ge;
      delete T_BGO;
      delete T_LaBr;
      delete dE_Ge;
      delete pMult_LaBr3;
      delete dMult_LaBr3;
      delete dMult_GeBGO;
      delete pMult_total;
      delete cMult_Ge;
      delete E_total;
      delete E_BGO;
      delete E_LaBr3;
      delete TE_Ge;
      delete TE_LaBr3;
      delete TE_BGO;
      delete pEpE_m4_Ge;
      delete pEpE_m5_Ge;
      delete pEpE_m6_Ge;
      delete dEdE_100_400_Ge;
      delete dEdE_400_2800_Ge;
      //delete dd200;
      //delete dd100;
      delete pEdE_100_400_Ge;
      //delete dp200;
      delete dEdET_100_400_Ge;
      delete dEtotaldE_100_400_Ge;
      delete pileup_Ge;
      for (int i = 1; i <= NDETS; i++) {
         delete eSpecs[i];
         delete tSpecs[i];
         delete pSpecs[i];
      }
      delete [] eSpecs;
      delete [] tSpecs;
      delete [] pSpecs;
   };

   //--------------------------------------------------------------------------
   // Write all histograms to the output root file and as radware spectra
   void WriteAll() const {
      auto f = new TFile(rootfile.c_str(), "RECREATE");
      Write();
      f->Close();
      //WriteRadware();
   };
   
   //--------------------------------------------------------------------------
   // Write to root file
   void Write() const {
      HK->Write();
      E_Ge->Write();
      Ec_Ge->Write();
      T_Ge->Write();
      T_BGO->Write();
      T_LaBr->Write();
      dE_Ge->Write();
      pMult_LaBr3->Write();
      dMult_LaBr3->Write();
      dMult_GeBGO->Write();
      pMult_total->Write();
      cMult_Ge->Write();
      E_total->Write();
      E_BGO->Write();
      E_LaBr3->Write();
      TE_Ge->Write();
      TE_LaBr3->Write();
      TE_BGO->Write();
      pEpE_m4_Ge->Write();
      pEpE_m5_Ge->Write();	
      pEpE_m6_Ge->Write();	
      dEdE_100_400_Ge->Write();	
      dEdE_400_2800_Ge->Write();
      //dd200->Write();
      //dd100->Write();
      pEdE_100_400_Ge->Write();
      //dp200->Write();
      dEdET_100_400_Ge->Write();
      dEtotaldE_100_400_Ge->Write();
      pileup_Ge->Write();
      
      int nGe = 0;
      int nBGO = 0;
      int nLaBr3 = 0;
      for (int i = 1; i < NDETS; i++) {
         if (eSpecs[i]->Integral() <= 10) continue;
         eSpecs[i]->Write();
         tSpecs[i]->Write();
         if (lookup->IsGe(i)) {nGe++; pSpecs[i]->Write();}
         if (lookup->IsBGO(i)) {nBGO++;}
         if (lookup->IsLaBr3(i)) {nLaBr3++;}
      }
      std::cout << nGe << "/106 Ge (" << 100*nGe/106.0 <<"%) crystals working" << std::endl;
      std::cout << nBGO << "/58 BGO (" << 100*nBGO/58.0 <<"%) crystals working" << std::endl;
      std::cout << nLaBr3 << "/20 LaBr3 (" << 100*nLaBr3/20.0 <<"%) crystals working" << std::endl;
  
   };

   //--------------------------------------------------------------------------
   // Write as radware
   void WriteRadware() const {
      WriteRadwareMatrix(pEpE_m4_Ge);
      WriteRadwareMatrix(pEpE_m5_Ge);
      WriteRadwareMatrix(pEpE_m6_Ge);
      WriteRadwareMatrix(dEdE_400_2800_Ge);
      WriteRadwareMatrix(dEdE_100_400_Ge);
      WriteRadwareMatrix(pEdE_100_400_Ge);
      WriteRadwareMatrix(dEtotaldE_100_400_Ge);
      WriteRadwareSpectrum(E_Ge);
      WriteRadwareSpectrum(pileup_Ge);
      WriteRadwareSpectrum(Ec_Ge);
      WriteRadwareSpectrum(E_BGO);
      WriteRadwareSpectrum(E_LaBr3);
      WriteRadwareSpectrum(T_Ge);
      WriteRadwareSpectrum(T_BGO);
      WriteRadwareSpectrum(T_LaBr);
      WriteRadwareSpectrum(cMult_Ge);
      WriteRadwareSpectrum(pMult_total);
      WriteRadwareSpectrum(dMult_GeBGO);
      WriteRadwareSpectrum(E_total);
      WriteRadwareSpectrum(dE_Ge);
   };

 private:
   //--------------------------------------------------------------------------
   // Write a single radware matrix
   void WriteRadwareMatrix(TH2 *h) const {
      // Open file
      auto fp = fopen(Form("%s.m4b", h->GetName()), "wb");
      if (!fp) return;

      // Loop over lines
      for (Int_t i = 0; i < h->GetNbinsY(); i++) {

         // Loop over bins in line
         int buffer[h->GetNbinsX()];
         for (Int_t j = 0; j < h->GetNbinsX(); j++)
           buffer[j] = h->GetBinContent(i, j);

         // Write line
         Int_t status = fwrite(buffer, sizeof(buffer), 1, fp);
         if (status != 1) break;
      }

      // Close file
      fclose(fp);

      std::cout << "Wrote Radware matrix " << h->GetName() << ".m4b" << std::endl;
   };

   //--------------------------------------------------------------------------
   // Write a Radware spectrum
   void WriteRadwareSpectrum(TH1 *h) const {

      // Put data into a float buffer
      Int_t nchans = h->GetNbinsX();
      Float_t buffer[nchans];
      for (Int_t i = 0; i < nchans; i++)
        buffer[i] = float(h->GetBinContent(i));

      // Generate gf2 header
      std::string s = Form("%s.spe", h->GetName());
      for (Int_t i = 0; i < 8; i++) s += '\0';
      int header[] = {24,0,0,nchans,1,1,1,24,nchans*(int)sizeof(float)};
      for (Int_t i = 0; i < 8; i++) ((char *)(header + 1))[i] = s.c_str()[i];

      // Open file
      auto fp = fopen(Form("%s.spe", h->GetName()), "wb");

      // Write header
      int status = fwrite(header, sizeof(header), 1, fp);
      if (status != 1) std::cerr << "Error writing spectrum" << std::endl;

      // Write data
      status = fwrite(buffer, sizeof(buffer), 1, fp);
      if (status != 1) std::cerr << "Error writing spectrum" << std::endl;

      // Write trailer
      status = fwrite(header+8, sizeof(Int_t), 1, fp);
      if (status != 1) std::cerr << "Error writing spectrum" << std::endl;

      // Close file
      fclose(fp);
      std::cout << "Wrote Radware spectrum " << h->GetName() << ".spe" << std::endl;
   };

};

#endif
