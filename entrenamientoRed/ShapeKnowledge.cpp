//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("ShapeKnowledge.res");
USEFORM("UFormPrinc.cpp", Form1);
USEUNIT("CImagen.cpp");
USEUNIT("CListaImagenes.cpp");
USEUNIT("CRedNeuronal.cpp");
USEUNIT("FuncionesAdicionales.cpp");
USEUNIT("Principal.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
