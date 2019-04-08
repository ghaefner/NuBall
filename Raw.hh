#ifndef __RAW_HH__
#define __RAW_HH__

// This class is for the storage of the raw input data from the input root
// tree. The variables in the class are one-to-one those in the tree

class Raw {
 private:
   UInt_t size;
   Double_t energies[512]; // energies array
   Double_t timestamps[512]; // timestamps array
   UInt_t ids[512]; //ids array
   Bool_t pups[512]; //pileups array

 public:
   //--------------------------------------------------------------------------
   // Set the branch addresses
   void SetBranchAddresses(TTree *tree) {
      // Set the branch addresses
      tree->SetBranchAddress("size",&size);
      tree->SetBranchAddress("ids",&ids);
      tree->SetBranchAddress("energies",&energies);
      tree->SetBranchAddress("timestamps",&timestamps);
      tree->SetBranchAddress("pups",&pups);
   };

   //--------------------------------------------------------------------------
   // Get size
   UInt_t GetSize() const {
      return(size);
   };
   
   //--------------------------------------------------------------------------
   // Get energy
   Double_t GetEnergy(UInt_t ind) const {
      if (ind >= 512) return(0);
      return(energies[ind]);
   };

   //--------------------------------------------------------------------------
   // Get timestamp
   Double_t GetTimestamp(UInt_t ind) const {
      if (ind >= 512) return(0);
      return(timestamps[ind]);
   };

   //--------------------------------------------------------------------------
   // Get ID
   Int_t GetID(UInt_t ind) const {
      if (ind >= 512) return(0);
      return(ids[ind]);
   };

   //--------------------------------------------------------------------------
   // Get pileup flag
   Bool_t GetPileup(UInt_t ind) const {
      if (ind >= 512) return(0);
      return(pups[ind]);
   };

};

#endif
