//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmCamara.h"
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        c = new CVMR_Capture();
        c->EnumDevices(ComboBox1->Handle);
        ruta = "c:\\";
        turno = 0;
        Edit1->Text = ruta;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
        if(!Timer1->Enabled) {
                Timer1->Interval = TrackBar1->Position;
        }
        Timer1->Enabled = !Timer1->Enabled;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
       Pinta();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBar1Change(TObject *Sender)
{
        Label3->Caption = TrackBar1->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        delete c;
}
//---------------------------------------------------------------------------

void TForm1::Pinta() {
        Image1->Canvas->Draw(0, 0, c->CrearBitmap(CheckBox1->Checked));
}

void __fastcall TForm1::Button7Click(TObject *Sender)
{
        Pinta();
        Label7->Caption = ruta + "snap" + turno + ".bmp";
        Image1->Picture->SaveToFile(Label7->Caption);
        turno++;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1Change(TObject *Sender)
{
        ruta = Edit1->Text;
}
//---------------------------------------------------------------------------







void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
        if(ComboBox1->ItemIndex != -1) {
             Button1->Enabled = true;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
        c->Init(ComboBox1->ItemIndex, Panel1->Handle, 320, 240);
        Button1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
        c->Pause();
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Button2Click(TObject *Sender)
{
        Pinta();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button5Click(TObject *Sender)
{
SaveDialog1->Filter =  "Imagen plana (*.raw)|*.raw|Todos los archivos (*.*)|*.*";
        if(SaveDialog1->Execute()) {
                c->ImageCapture(SaveDialog1->FileName.c_str());

        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
SaveDialog1->Filter =  "Bitmap (*.bmp)|*.bmp|Todos los archivos (*.*)|*.*";
        if(SaveDialog1->Execute()) {
                Image1->Picture->SaveToFile(SaveDialog1->FileName);
        }        
}
//---------------------------------------------------------------------------


