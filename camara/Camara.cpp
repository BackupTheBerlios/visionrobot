//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Camara.res");
USEFORM("frmCamara.cpp", Form1);
USEUNIT("Captura.cpp");
USELIB("..\CBuilder_DX92_libs\quartz.lib");
USELIB("..\CBuilder_DX92_libs\strmiids.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Cámara";
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
