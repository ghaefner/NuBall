#ifndef __UNPACKED_HH__
#define __UNPACKED_HH__

// Class to store the unpacked data. We use arrays of dimension 35 for the 34
// modules and we don't use the first element of each array. So the array is
// indexed by module number

class Unpacked {
 public:    
   // LaBr3 multiplicities
   int pMultLaBr3; // LaBr3 prompt mult
   int dMultLaBr3;
   

   // PROMPT: Used for Compton suppression and addback in clovers
   int pMultGe; // Prompt clean Ge mult
   int pHitBGO[35]; // BGO hit pattern in the event
   int pHitGe[35]; // Ge clover mult pattern in the event
   int pHitPileup[35]; // Ge clover pileup pattern in the event
   double pEmod[35]; // Ge module energies
   double pTmod[35]; // Variable to make an "average" clover timestamp

   // Isomer calorimetry
   int dMultModule; //
   double dEmodule; //
   double dEsumModule[35]; // Ge module energies
   double dEsumLaBr3; //
   

   // DELAYED: Used for Compton suppression and addback in clovers;
   int dHitBGO[35]; //BGO hit pattern in the event
   int dHitGe[35]; //Ge clover mult pattern in the event
   int dHitPileup[35]; //Ge clover pileup pattern in the event
   double dEmod[35]; //Ge module energies
   double dTmod[35]; //Variable to make an "average" clover timestamp
      
   //Arrays in which to extract the information
   double eGe[512]; //Array of Ge module energies
   double tGe[512]; //Array of Ge module timestamps
   int idGe[512]; //Array of Ge module label 

   // Multiplicity and total energy
   int TotalMult;
   int NewTotalMult;
   int pMultGeBGO;
   int dMultGeBGO;
   double TotalEnergy;

   //--------------------------------------------------------------------------
   // Set everything to zero
   void Clear() {
      NewTotalMult = 0;
      dMultLaBr3 = 0;
      dEsumLaBr3 = 0;
      dMultModule = 0;
      dEmodule = 0;
      for (int k = 1; k <= 34; k++) {
         pHitBGO[k] = 0;
         pHitGe[k] = 0;
         pHitPileup[k] = 0;
         pEmod[k] = 0;
         pTmod[k] = 0; // prompt
         dHitBGO[k] = 0;
         dHitGe[k] = 0;
         dHitPileup[k] = 0;
         dEmod[k] = 0;
         dTmod[k] = 0; // delayed
         dEsumModule[k] = 0;
         eGe[k] = 0;
         idGe[k] = 0;
         tGe[k] = 0;
      }
      pMultLaBr3 = 0;
      pMultGe = 0;
      TotalMult = 0;
      pMultGeBGO = 0;
      dMultGeBGO = 0;
      TotalEnergy = 0;
   };
};

#endif
