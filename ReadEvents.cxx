#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <TImage.h>
#include <TCanvas.h>
#include <TArrayD.h>
#include <TROOT.h>
#include <TColor.h>
#include <TAttImage.h>
#include <TEnv.h>
#include <TH2D.h>
#include <TH3S.h>
#include <TF2.h>
#include <TLine.h>
#include <TRandom.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>

#include "MyHistograms.hh"
#include "Lookup.hh"
#include "Raw.hh"
#include "Unpacked.hh"
#include "Analysis.hh"

// ALL THE FOLLOWING ARE NEEDED
// THE INPUT FILE
std::string FileList="evtfiles_iso.dat"; //The list of Event-built files to process
//std::string FileList="filelistCheck.dat"; //The list of Event-built files to process
std::string OutputDirectory="output/";
std::string InputDirectory="/data/Experiments/staging/2018_IPNOrsay_NUBALL_238U/data6/";

//-----------------------------------------------------------------------------
// Unpack the event. Here we read the raw data from the root file and work out
// things like multiplicities. We also fill the simple histograms
inline void UnpackEvent(const Raw &raw, const MyHistograms &histos,
                        Lookup &lookup, Unpacked &u, Analysis &ana) {

   for (UInt_t j = 0; j < raw.GetSize(); j++) {
      auto detid = raw.GetID(j);
      auto cnrj = raw.GetEnergy(j);
      auto timestamp = raw.GetTimestamp(j);
      auto pileup = raw.GetPileup(j);
      

      // Special cases
      if (lookup.IsBad(detid)) {cnrj=-1;} // Do not use these detectors.
      if (detid == 253) {u.TotalEnergy=cnrj;}
      if (detid == 254) {u.TotalMult=cnrj;}

      // Singles spectra      
      histos.eSpecs[detid]->Fill(cnrj);
      histos.tSpecs[detid]->Fill(timestamp);

      // If it is a Ge
      if (lookup.IsGe(detid)) {
         if (pileup) {
            histos.pSpecs[detid]->Fill(cnrj);
            histos.pileup_Ge->Fill(cnrj);
         }
         histos.E_Ge->Fill(cnrj); 
         histos.T_Ge->Fill(timestamp); 
         histos.TE_Ge->Fill(timestamp,cnrj);
      }

      // If it is a BGO
      if (lookup.IsBGO(detid)) {
         histos.E_BGO->Fill(cnrj);
         histos.T_BGO->Fill(timestamp);
         histos.TE_BGO->Fill(timestamp,cnrj);
      }

      // If it is a LaBr3
      if (lookup.IsLaBr3(detid)) {
         histos.E_LaBr3->Fill(cnrj);
         histos.T_LaBr->Fill(timestamp);
         histos.TE_LaBr3->Fill(timestamp,cnrj);
         
         if ( ana.IsLaBrPrompt(timestamp) ) { u.pMultLaBr3++; }
         if ( ana.IsLaBrDel(timestamp) ) { u.dMultLaBr3++ ; u.dEsumLaBr3 += cnrj ;}
    
      }
      
      //Put delayed gamma rays in an array first
      // Prompt gammas: Do Compton supression, addback and pileup reject
      if ((lookup.IsGe(detid)) && (timestamp < 400) && (cnrj < 10000)) {
         int mnum = lookup.GetModule(detid);
         u.pHitGe[mnum]++;
         u.pEmod[mnum] += raw.GetEnergy(j);
         u.pTmod[mnum] += raw.GetTimestamp(j);
         if (pileup) {u.pHitPileup[mnum]=true;} //There was a Ge pileup in the module 
         if (timestamp >= 100) {u.dEsumModule[mnum] += cnrj;}
      }
      if ((lookup.IsBGO(detid)) && (timestamp < 400) && (cnrj < 10000))  {
         int mnum=lookup.GetModule(detid); u.pHitBGO[mnum]++; 
         if (timestamp >= 100) {u.dEsumModule[mnum] += cnrj;}
      }
      
      // Delayed gammas: Do Compton supression, addback and pileup reject
      // We are making the assumption here that if two clover elements fire in the delayed
      // time window from the same clover, then they must have fired "together"
      // i.e. no Ge detector was hit more than once in the window. Sometimes not correct.
      // Ideally "proper" (one Ge, with multiple hits possible) event-building needs 
      // to occur in the delayed window. (But delayed M is very low on average.)
      if ((lookup.IsGe(detid)) && (timestamp > 400) && (cnrj < 10000)) {
         int mnum = lookup.GetModule(detid);
         u.dHitGe[mnum]++; u.dEmod[mnum] += raw.GetEnergy(j); u.dTmod[mnum] += raw.GetTimestamp(j);
         if (pileup) {u.dHitPileup[mnum]=true;} //There was a Ge pileup in the module 
      }
      if ((lookup.IsBGO(detid)) && (timestamp > 400) && (cnrj < 10000)) {
         int mnum = lookup.GetModule(detid); u.dHitBGO[mnum]++;
      }
      
   }// End of j loop, reading the event
}

//-----------------------------------------------------------------------------
// Fill out eGe, idGe, tGe arrays and determine pMultGe
inline void GeLoop(const MyHistograms &histos, Unpacked &u) {
   
   for(int k = 1; k <= 34; k++) { //loop through the Ge modules
      
      // Delayed mult and energy for isomer calorimetry
      if (u.dEsumModule[k] > 5) {
         u.dEmodule += u.dEsumModule[k];
         u.dMultModule++;
      }
      // New total multiplicity
      if (u.pHitBGO[k] || u.pHitGe[k]) {
         u.NewTotalMult++;
      }
      
      // PROMPT: Addback and compton suppression
      if ((u.pEmod[k] > 5) && (u.pHitBGO[k]==0) && (u.pHitGe[k] >= 1) && !u.pHitPileup[k]) { //Clean Ge only
         u.eGe[u.pMultGe] = u.pEmod[k];
         if (u.pHitGe[k] > 1) {
            u.tGe[u.pMultGe] = u.pTmod[k] / double(u.pHitGe[k]);
         } //Clover avg timestamp
         else {
            u.tGe[u.pMultGe] = u.pTmod[k];
         } //Normal case. 1 clover element, or phase1.
         u.idGe[u.pMultGe]=k; //The module number
         u.pMultGe++;
      }
      
      //DELAYED. SUBSEQUENT BURSTS 500-1200 ns. Suppression and addback is separate from first burst	
      // ADD THESE GAMMAS ALSO INTO THE eGe and tGe and idGe arrays	
      if (u.dHitBGO[k] || u.dHitGe[k]) {
         u.dMultGeBGO++;
      }
      if ((u.dEmod[k] > 5) && (u.dHitBGO[k]==0) && (u.dHitGe[k] >= 1) && !u.dHitPileup[k]) { //Clean Ge only
         u.eGe[u.pMultGe] = u.dEmod[k];
         histos.dE_Ge->Fill(u.dEmod[k]); //The real spectrum of long delayed gammas
         if (u.dHitGe[k] > 1) {
            u.tGe[u.pMultGe] = u.dTmod[k] / double(u.dHitGe[k]);
         } //Clover avg timestamp
         else {
            u.tGe[u.pMultGe] = u.dTmod[k];
         } //Normal case. 1 clover element, or phase1.
         u.idGe[u.pMultGe] = k; //The module number
         u.pMultGe++;
      }
   } //end for k loop
}

//-----------------------------------------------------------------------------
// Increment dMult_GeBGO histogram and dEtotaldE_100_400_Ge matrix
inline void IncrementDsum(const MyHistograms &histos, const Unpacked &u) {

   //Fill dMult_GeBGO histogram
   histos.dMult_GeBGO->Fill(u.dMultGeBGO); //How many LONG delayed gammas were there, including BGO?

   // Do nothing if dMultModule is zero
   if (u.dMultModule < 1) return;

   // Now actually fill the dEtotaldE_100_400_Ge matrix
   for (int k = 0; k < u.pMultGe; k++) {

      // All except first 70 lines are eGe vs. dEmodule
      if ((u.tGe[k] > 100) && (u.tGe[k] < 400) && (u.dEmodule > 70)) {
         histos.dEtotaldE_100_400_Ge->Fill(u.eGe[k], u.dEmodule);
      }

      // But the first 70 lines are different: eGe vs. dMultModule
      if ((u.tGe[k] > 100) && (u.tGe[k] < 400) && (u.dMultModule < 70)) {
         histos.dEtotaldE_100_400_Ge->Fill(u.eGe[k], u.dMultModule);
      }
   }
}

//-----------------------------------------------------------------------------
// Increment cMult_Ge, pMult_total, E_total spectra
inline void IncrementMultiplicitySpectra(const MyHistograms &histos, const Unpacked &u) {

   // Fill some histograms
   histos.cMult_Ge->Fill(u.pMultGe);
   histos.pMult_total->Fill(u.NewTotalMult);
   histos.E_total->Fill(u.TotalEnergy);
   histos.HK->Fill(u.NewTotalMult, u.TotalEnergy);
   histos.pMult_LaBr3->Fill(u.pMultLaBr3);
   histos.dMult_LaBr3->Fill(u.dMultLaBr3);
}

//-----------------------------------------------------------------------------
// Increment pEpE_m4_Ge, pEpE_m5_Ge and p6 matrices
inline void IncrementPromptGammaGammaMatrix(const MyHistograms &histos, const Unpacked &u, Analysis &ana) {

   // Increment a prompt gamma-gamma matrix
   for (int k = 0; k < u.pMultGe - 1; k++) {
      for (int l = k + 1; l < u.pMultGe; l++) {
         if ( (ana.IsGePrompt(u.tGe[k])) || (ana.IsGeDelShort(u.tGe[k])) ) { // 0-100 (prompt) or 100-400 (short)
            if (u.NewTotalMult >= 4) { // Multiplicity >= 4
               histos.pEpE_m4_Ge->Fill(u.eGe[k], u.eGe[l]);
               histos.pEpE_m4_Ge->Fill(u.eGe[l], u.eGe[k]);
            }
            if (u.NewTotalMult >= 5) { // Multiplicity >= 5
               histos.pEpE_m5_Ge->Fill(u.eGe[k], u.eGe[l]);
               histos.pEpE_m5_Ge->Fill(u.eGe[l], u.eGe[k]);
            }
            if (u.NewTotalMult >= 6) { // Multiplicity >= 6
               histos.pEpE_m6_Ge->Fill(u.eGe[k], u.eGe[l]);
               histos.pEpE_m6_Ge->Fill(u.eGe[l], u.eGe[k]);
            }
         }
      }
   }
}

//-----------------------------------------------------------------------------
// Increment prompt-delayed (pEdE_100_400_Ge) matrices
void IncrementPromptDelayedMatrices(const MyHistograms &histos, Unpacked &u, Analysis &ana ) {

   // dp asymmetric matrix for  window prompt = 0-100 ns,delayed from 100 to 400
   for (int k = 0; k < u.pMultGe; k++) {
	   // Check if first Ge is prompt, only then look for delayed
    	if ( ana.IsGePrompt(u.tGe[k]) ) {  
	   
			for (int l = 0; l < u.pMultGe; l++) {
         // Time condition is essential for asymmetric matrix
			
			if ( ana.IsGeDelShort(u.tGe[l]) )
				histos.pEdE_100_400_Ge->Fill(u.eGe[k],u.eGe[l]);
				
	 
			}
		}
   }
}

//-----------------------------------------------------------------------------
// Increment delayed (dEdET_100_400_Ge, dEdE_400_2800_Ge, dEdE_100_400_Ge, dd200, dd100 and ddvshort)
//  matrices
inline void IncrementDelayedMatrices(const MyHistograms &histos, Unpacked &u, Analysis &ana) {

   // Symmetrised delayed matrices
   for (int k = 0; k < u.pMultGe - 1; k++) {
      for (int l = k + 1; l < u.pMultGe; l++) {

         // Time difference must be <= 150
         int deltat = abs(u.tGe[k] - u.tGe[l]);
         if (deltat > 150) continue;
	// Calculate average time and shift it back to 
         int avgt = int((((u.tGe[k]+u.tGe[l])/2.0)-40.0)/10.0);
         histos.dEdET_100_400_Ge->Fill(u.eGe[k], u.eGe[l], avgt);
         histos.dEdET_100_400_Ge->Fill(u.eGe[l], u.eGe[k], avgt);
         
         if ( (ana.IsGeDelLong(u.tGe[k])) && (ana.IsGeDelLong(u.tGe[l])) )	{
            histos.dEdE_400_2800_Ge->Fill(u.eGe[k], u.eGe[l]); // LONG
            histos.dEdE_400_2800_Ge->Fill(u.eGe[l], u.eGe[k]);
         }
         if ( (ana.IsGeDelShort(u.tGe[k])) &&  (ana.IsGeDelShort(u.tGe[l])) ){
            histos.dEdE_100_400_Ge->Fill(u.eGe[k], u.eGe[l]); // SHORT
            histos.dEdE_100_400_Ge->Fill(u.eGe[l], u.eGe[k]);
         }
                  
      }
   }
}

//-----------------------------------------------------------------------------
// Treat a single entry
void TreatEntry(const Raw &raw, const MyHistograms &histos, Lookup &lookup, Analysis &ana) {

   // Initialise unpacked data
   Unpacked unpacked;
   unpacked.Clear();

   // Sanity check
   if (raw.GetSize() > 250) {
      std::cout << "VERY LARGE EVENT SIZE!! EXITING " << raw.GetSize() << std::endl;
      exit(1);
   }


   // Unpack the event from raw to unpacked
   UnpackEvent(raw, histos, lookup, unpacked, ana);

   // Ge loop
   GeLoop(histos, unpacked);
   unpacked.NewTotalMult += unpacked.pMultLaBr3;
   
   //isomer calorimetry
   unpacked.dMultModule += unpacked.dMultLaBr3;
   unpacked.dEmodule += unpacked.dEsumLaBr3;
   
   // Increment spectra
   IncrementDsum(histos, unpacked);

   // Increment prompt gamma-gamma matrix
   if (unpacked.NewTotalMult < 2 || unpacked.pMultGe < 2) return;
   IncrementMultiplicitySpectra(histos, unpacked);
   IncrementPromptGammaGammaMatrix(histos, unpacked, ana);

   // Increment prompt and delayed matrices
   if (unpacked.pMultGe < 2 || unpacked.NewTotalMult < 4) return;
  // const int DPSPLIT = 100;
   IncrementPromptDelayedMatrices(histos, unpacked, ana);
   IncrementDelayedMatrices(histos, unpacked, ana);
}

//-----------------------------------------------------------------------------
// Extract the run and file number from the filename
void DecodeFilename(std::string filename, int &RunNumber, int &FileNumber) {

   // Figure out the run number from the filename
   RunNumber = atoi(filename.substr(4,3).c_str());
   
   int fn1=atoi(filename.substr(filename.size()-6,1).c_str()); //first digit
   int fn2=atoi(filename.substr(filename.size()-7,1).c_str()); //second digit
   int fn3=atoi(filename.substr(filename.size()-8,1).c_str()); //third digit
   if ((filename.substr(filename.size()-7,1) == "_")) {FileNumber=fn1;}
   if ((filename.substr(filename.size()-8,1) == "_")) {FileNumber=fn1+(10*fn2);}
   if ((filename.substr(filename.size()-9,1) == "_")) {FileNumber=fn1+(10*fn2)+(100*fn3);}
}

//-----------------------------------------------------------------------------
// Treat a single input root file
void TreatFile(std::string filename, const MyHistograms &histos, Lookup &lookup, Analysis &ana,
               std::ofstream &logfile, std::string OutputSpectraFile,
               int &nfiles, int &countfiles, long &EventCount) {
   
   // Increment number of files
   nfiles++;

   // The only reason we need to read the list of files twice is so we can
   // count the files, in order to write the following message!
   if (nfiles == countfiles) {std::cout << "Reading LAST file " << std::endl;}

   // Figure out the run and file numbers from the filename
   int RunNumber, FileNumber;
   DecodeFilename(filename, RunNumber, FileNumber);
   
   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! START TO PROCESS THIS FILE !!!!!!!!!!!!!!!!
   
   // Get full path of input root file
   std::string InputFile = InputDirectory + filename;
   std::cout << "Reading " << InputFile << std::endl;
   
   // Open input root file
   auto fin = TFile::Open(InputFile.c_str(),"READ");
   if (!fin) return;
   
   // Get the tree from the input root file
   auto inputtree = (TTree*)fin->Get("DataTree");;

   // Create storage space for raw data and set branch addresses
   Raw raw;
   raw.SetBranchAddresses(inputtree);

   // Get number of entries
   auto entries = inputtree->GetEntries();
   std::cout << "File " << nfiles << " contains " << entries << " events" << std::endl;
   logfile << "File " << nfiles << " " << InputFile << " contains " << entries << " events" << std::endl;
   
   // Loop over all the entries in the tree
   Long64_t i;
   for(i = 0; i < entries; i++) {

      // Get the entry
      inputtree->GetEntry(i);
      
      // Treat the entry
      TreatEntry(raw, histos, lookup, ana);
      
   } //End of loop to read the Tree entries

   // Close the input file
   fin->Close();

   // Add to event count
   EventCount += i;

   // Write the root and radware files after the third subrun, just to check
   if (nfiles == 3) histos.WriteAll();
}

//-----------------------------------------------------------------------------
// Main routine
int main(int argc, char **argv) {

   // Initialise some variables
   int nfiles = 0;
   long EventCount = 0;
   std::string OutputSpectraFile = OutputDirectory + "Test_COIN150.root";

   // Initialise random number generator
   srand48(time(0));

   // Initialise lookup tables
   Lookup lookup;

   // Initialise histograms
   MyHistograms histos(&lookup, OutputSpectraFile);
   
   // Initialise Analysis
   Analysis ana;
   
   // Open log file
   std::ofstream logfile("log.dat");

   // Count the number of files in the list of input root files
   std::ifstream iflist2(FileList.c_str());
   std::string line;
   int countfiles = 0;
   while(iflist2 >> line){countfiles++;}
   iflist2.close();
   
   std::cout << "Processing " << countfiles << " files " <<std::endl;
   
   // Loop over lines in list of root files
   std::ifstream iflist(FileList.c_str());
   while(iflist >> line) {

      // Create a timer and start it
      TStopwatch timer;
      timer.Reset();
      timer.Start();
      
      // Treat a single input root file
      TreatFile(line, histos, lookup, ana, logfile, OutputSpectraFile, nfiles,
                countfiles, EventCount);

      // Stop the timer and write some information
      timer.Stop();
      Double_t rtime2 = timer.RealTime();
      Double_t ctime2 = timer.CpuTime();
      std::cout << "# RealTime=" << rtime2 << " seconds, CpuTime="<< ctime2 <<
                   " seconds" <<std::endl;

      // Write cumulative number of events read
      std::cout << "Total Events Read = " << EventCount << std::endl;
   }

   // Write everything
   histos.WriteAll();

   // Close list of input root files
   iflist.close();
}
