//---------------------------------------------------------------------------

#ifndef frmCamaraH
#define frmCamaraH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "VMR_Capture.h"
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TComboBox *ComboBox1;
        TLabel *Label1;
        TSaveDialog *SaveDialog1;
        TImage *Image1;
        TTimer *Timer1;
        TLabel *Label2;
        TTrackBar *TrackBar1;
        TButton *Button4;
        TLabel *Label3;
        TCheckBox *CheckBox1;
        TGroupBox *GroupBox1;
        TLabel *Label4;
        TEdit *Edit1;
        TButton *Button7;
        TLabel *Label7;
        TButton *Button1;
        TButton *Button3;
        TLabel *Label5;
        TButton *Button2;
        TButton *Button5;
        TButton *Button6;
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Edit1Change(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
private:	// User declarations
        void Pinta();
        CVMR_Capture *c;
        AnsiString ruta;
        bool iniciado;
        unsigned turno;
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
