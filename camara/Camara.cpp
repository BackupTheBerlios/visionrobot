//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Camara.res");
USEFORM("frmCamara.cpp", Form1);
USEUNIT("Captura.cpp");
USELIB("C:\Archivos de programa\Borland\CBuilder5\Lib\bcb\strmiids.lib");
USELIB("C:\Archivos de programa\Borland\CBuilder5\Lib\bcb\quartz.lib");
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
