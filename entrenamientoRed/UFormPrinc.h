//---------------------------------------------------------------------------

#ifndef UFormPrincH
#define UFormPrincH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Principal.h"
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TEdit *EditEntre;
        TEdit *EditValid;
        TEdit *EditPrueba;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TOpenDialog *OpenDialog1;
        TEdit *Edit1;
        TComboBox *ComboBox1;
        TButton *Button5;
        TMemo *Memo1;
        TButton *Button6;
        TEdit *Edit4;
        TButton *Button7;
        TEdit *Edit5;
        TEdit *Edit2;
        TMainMenu *MainMenu1;
        TMenuItem *RedNeuronal1;
        TMenuItem *Abrir1;
        TMenuItem *Guardar1;
        TMenuItem *N1;
        TMenuItem *Salir1;
        TSaveDialog *SaveDialog1;
        TImage *Image2;
        TLabel *Label8;
        TLabel *Label1;
        TLabel *Label2;
        TShape *Shape2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label9;
        TProgressBar *Prog;
        TImage *Image1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Salir1Click(TObject *Sender);
        void __fastcall Guardar1Click(TObject *Sender);
        void __fastcall Abrir1Click(TObject *Sender);
private:	// User declarations
        Principal* principal;
        tipoFoto tipofoto;
        bool entrenada;
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
