#ifndef __ANALYSIS_HH__
#define __ANALYSIS_HH__

#define T_GE_PROMPT_LOW 0.
#define T_GE_PROMPT_HIGH 100.
#define T_GE_DEL_SHORT_LOW 100.
#define T_GE_DEL_SHORT_HIGH 400.
#define T_GE_DEL_LONG_LOW 400.
#define T_GE_DEL_LONG_HIGH 2800.

#define T_LABR_PROMPT_LOW 35.
#define T_LABR_PROMPT_HIGH 50.
#define T_LABR_DEL_LOW 50.
#define T_LABR_DEL_HIGH 400.

// Class to do basic analysis steps such as gating and prompt
// delayed discrimination etc.

class Analysis {

 public:
 
	
	bool EGate(Double_t eGamma, Double_t lower, Double_t upper);
	bool TGate(Double_t tGamma, Double_t lower, Double_t upper);
	bool IsGePrompt(Double_t tGe);
	bool IsGeDelShort(Double_t tGe);
	bool IsGeDelLong(Double_t tGe);
	bool IsLaBrPrompt(Double_t tLaBr);
	bool IsLaBrDel(Double_t tLaBr);

};
#endif


 bool Analysis::EGate(Double_t eGamma, Double_t lower, Double_t upper)
 {
	if ( (eGamma > lower) && (eGamma <= upper) )
		return 1;
	
	return 0;
 };

 bool Analysis::TGate(Double_t tGamma, Double_t lower, Double_t upper)
 {
	if ( (tGamma > lower) && (tGamma <= upper) )
		return 1;
	
	return 0;
 };

bool Analysis::IsGePrompt(Double_t tGe)
 {
	if ( (tGe > T_GE_PROMPT_LOW) && (tGe <= T_GE_PROMPT_HIGH) )
		return 1;
	return 0;
 };

bool Analysis::IsGeDelShort(Double_t tGe)
 {
	if ( (tGe > T_GE_DEL_SHORT_LOW) && (tGe <= T_GE_DEL_SHORT_HIGH) )
		return 1;
	return 0;
 };

bool Analysis::IsGeDelLong(Double_t tGe)
 {
	if ( (tGe > T_GE_DEL_LONG_LOW) && (tGe <= T_GE_DEL_LONG_HIGH) )
		return 1;
	return 0;
 };

bool Analysis::IsLaBrPrompt(Double_t tLaBr)
 {
	if ( (tLaBr > T_LABR_PROMPT_LOW) && (tLaBr <= T_LABR_PROMPT_HIGH) )
		return 1;
	return 0;
 };

bool Analysis::IsLaBrDel(Double_t tLaBr)
 {
	if ( (tLaBr > T_LABR_DEL_LOW) && (tLaBr <= T_LABR_DEL_HIGH) )
		return 1;
	return 0;
 };


