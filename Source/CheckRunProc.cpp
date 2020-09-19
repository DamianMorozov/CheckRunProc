//======================================================================================================================
// ��������
// ========
//    ������� ���������.
// ����������
// ==========
//    � ������� ������ ����������
//    E-Mail: support@morozov33.ru
// ������
// ======
//    http://forum-pro-it.ru
//======================================================================================================================
#include <vcl.h>                                 // VCL
#include <tchar.h>                               // TCHAR
#pragma hdrstop                                  // ����� ��������������
//----------------------------------------------------------------------------------------------------------------------
USEFORM("..\_Libs\UnitSettings.cpp", FormSettings);
USEFORM("..\_Libs\UnitHelp.cpp", FormHelp);
USEFORM("..\_Libs\UnitFrameApp.cpp", FrameAppMain); /* TFrame: File Type */
USEFORM("UnitMain.cpp", FormMain);
//---------------------------------------------------------------------------
extern  HANDLE             MutexMain;
extern  AnsiString         MutexString;
extern  AnsiString         AppHelpAutor;
extern  AnsiString         AppHelpEMail;
extern  AnsiString         AppHelpDate;
extern  AnsiString         AppHelpAbout;
extern  AnsiString         AppLink;
extern  AnsiString         AppHelpWebPage;
extern  int  __fastcall    MsgBoxWithTimeOut(HWND hWnd, AnsiString StrlpText, UINT uType, DWORD dwMilliseconds);
//----------------------------------------------------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{  // ������ �� ���������� ������� ��������� 2014.02.18
	if (ParamStr(1) == "")  MutexString = "CheckRunProc";
	else                    MutexString = "CheckRunProc_"+ParamStr(1);
	MutexMain = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MutexString.c_str());
	if (MutexMain) {
		MsgBoxWithTimeOut(NULL,("��������� "+MutexString+" ��� ��������.\n��������� ������ ��������.").c_str(),MB_OK+MB_ICONERROR,2500);
		CloseHandle(MutexMain);
		return 0;
	} else {
		MutexMain = CreateMutex(NULL, TRUE, MutexString.c_str());
	}
   // �������� ��������� 2014.02.20
	try {
		Application->Initialize();
		Application->Title = "Check of running processes";
		Application->Name  = "CheckRunProc";
		AppHelpAutor       = "������� �.�.";
		AppHelpEMail       = "support@morozov33.ru";
		AppHelpDate        = "2012-2014";
		AppHelpAbout       = "�������� ���������� ��������� (��������) � �� ��������� ������ � ������ \n��������������� ���������� ������.";
		AppLink            = "�������� ���������� ���������.lnk";
		AppHelpWebPage     = "http://forum-pro-it.ru";
		Application->MainFormOnTaskBar = true;
		Application->ShowMainForm      = true;
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->Run();
	} catch (...) {
      try {
         throw Exception("");
      } catch (Exception &e) {
         MsgBoxWithTimeOut(NULL, e.Message, MB_OK+MB_ICONERROR, 2500);
      }
   }
   CloseHandle(MutexMain);
   return 0;
} // ��������� 2014.02.20 ----------------------------------------------------------------------------------------------

