//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UFormPrinc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TForm1::FormCreate(TObject *Sender)
{
 entrenada=false;
 Prog->Visible=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button5Click(TObject *Sender)
{
 if(EditEntre->Text=="" || EditValid->Text=="" || EditPrueba->Text=="" || Edit1->Text=="" || ComboBox1->Text=="" || Edit2->Text=="")
   ShowMessage("Rellena los campos vacios");
 else{
  if(ComboBox1->Text=="ORDEN")tipofoto=ORDEN;
  else tipofoto=PARAM;

  int vueltas= StrToInt(Edit1->Text);
  AnsiString t= Edit2->Text;
  double tasa= StrToFloat(t);

  principal= new Principal(EditEntre->Text, EditValid->Text, EditPrueba->Text,
                                      vueltas, tipofoto, Memo1, Prog, tasa);
  entrenada=true;
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   EditEntre->Text= OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   EditValid->Text= OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   EditPrueba->Text= OpenDialog1->FileName;        
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button7Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   Edit4->Text= OpenDialog1->FileName;
 Imagen* imagen= new Imagen();
 imagen->Leer(Edit4->Text);
 Image1->Picture= imagen->GetDibujo();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
 if(entrenada){
   Edit5->Clear();
   Edit5->Text= principal->Reconocer(Edit4->Text, tipofoto);
 }
 else ShowMessage("¡Debes de cargar o entrenar antes una red!");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Salir1Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Guardar1Click(TObject *Sender)
{
 if(SaveDialog1->Execute() && entrenada)
   principal->GuardarRed(SaveDialog1->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Abrir1Click(TObject *Sender)
{
 if(OpenDialog1->Execute()){
   entrenada=true;
   principal=new Principal();
   principal->AbrirRed(OpenDialog1->FileName);
 }
}
//---------------------------------------------------------------------------

