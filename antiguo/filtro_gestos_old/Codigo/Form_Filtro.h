//---------------------------------------------------------------------------

#ifndef Form_FiltroH
#define Form_FiltroH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Codigo_Filtro.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
   TImage *Image1;
   TImage *Image2;
   TLabel *Label1;
   TLabel *Label2;
   TBevel *Bevel1;
   TLabel *Label3;
   TEdit *Edit1;
   TEdit *Edit2;
   TEdit *Edit3;
   TLabel *Label4;
   TLabel *Label5;
   TLabel *Label6;
private:	// User declarations
public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
