# NuBall
-------------------------
NuBall Analysis Code(s)
-------------------------

This code is based on Jon's ReadEvents8.cxx 
and modified to be programmed in OOP. La-
bels are changed to be clearer and histo-
grams are named properly. 

ReadEvents.cxx is the main code which 
- Unpacks events.
- Performs AB/CS.
- Fills histograms (using different func-
  tions, i.e. IncrementPromptMatrix()).
- Loops over all the files and applies
  all functions. 

The main classes are:

- Raw.hh
  Read the TTrees and sets the branch
  names (no modification done here).

- Unpacked.hh
  Unpackes the events, i.e. calculates
  things like multiplicities and defines
  the arrays used for AB/CS. 

- MyHistograms.hh
  Defines all the histograms and writes 
  them to root/radWare. 


Additional classes are:

- Analysis.hh
  Used for further analysis steps (so 
  far only for definition of prompt/
  delayed).

