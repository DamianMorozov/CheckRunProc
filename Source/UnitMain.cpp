//----------------------------------------------------------------------------------------------------------------------
#ifndef INC_VCL_SHORT                            // ������������� INC_VCL_SHORT � ����� PCH
   #define INC_VCL_SHORT
#endif
#include "PCH.h"                                 // �������������� ����������������� ���������
#pragma hdrstop                                  // ����� ��������������
#include "UnitMain.h"                            // �������� ������������ ���� �������� ������
#pragma package(smart_init)                      // ������������������ ������� �������������
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma resource "*.dfm"                         // �����
#include <tlhelp32.h>                            // �������� ������������ ���� tlhelp32
#include "UnitHelp.h"                            // �������� ������������ ���� ������ �������
#include "UnitFrameApp.h"                        // �������� ������������ ���� ������ ����������
//======================================================================================================================
//                                          ���������� ������� � ����������
//======================================================================================================================
TFormMain *FormMain;
#define ProcessBasicInformation 0
typedef struct  {
   USHORT Length;
   USHORT MaximumLength;
   PWSTR  Buffer;
}  UNICODE_STRING, *PUNICODE_STRING;
typedef struct  {
   ULONG          AllocationSize;
   ULONG          ActualSize;
   ULONG          Flags;
   ULONG          Unknown1;
   UNICODE_STRING Unknown2;
   HANDLE         InputHandle;
   HANDLE         OutputHandle;
   HANDLE         ErrorHandle;
   UNICODE_STRING CurrentDirectory;
   HANDLE         CurrentDirectoryHandle;
   UNICODE_STRING SearchPaths;
   UNICODE_STRING ApplicationName;
   UNICODE_STRING CommandLine;
   PVOID          EnvironmentBlock;
   ULONG          Unknown[9];
   UNICODE_STRING Unknown3;
   UNICODE_STRING Unknown4;
   UNICODE_STRING Unknown5;
   UNICODE_STRING Unknown6;
}  PROCESS_PARAMETERS, *PPROCESS_PARAMETERS;
typedef struct  {
   ULONG               AllocationSize;
   ULONG               Unknown1;
   HINSTANCE           ProcessHinstance;
   PVOID               ListDlls;
   PPROCESS_PARAMETERS ProcessParameters;
   ULONG               Unknown2;
   HANDLE              Heap;
}  PEB, *PPEB;
typedef struct  {
   DWORD ExitStatus;
   PPEB  PebBaseAddress;
   DWORD AffinityMask;
   DWORD BasePriority;
   ULONG UniqueProcessId;
   ULONG InheritedFromUniqueProcessId;
}  PROCESS_BASIC_INFORMATION;
typedef  LONG (WINAPI *PROCNTQSIP)(HANDLE,UINT,PVOID,ULONG,PULONG);
//AnsiString fFolderPath;
//AnsiString FileSettingsName = "UpMonitor.ini";
PROCNTQSIP NtQueryInformationProcess;
AnsiString AppName;
AnsiString AppFolder;
bool GetProcessCmdLine(DWORD dwId,LPWSTR wBuf,DWORD dwBufLen);
TDateTime TimeOutMax;                        // ����-��� �� ��������� ������
TDateTime TimeLast;                          // ����� ������� ��������. ����� 1 ������ ��� ������� �������
bool LogWriteOptimise  = false;              // ����������� ������ ����
unsigned long SizeName = 255;
TIniFile*  fSettings;
// TmINI      *INIMain = new TmINI;
bool RunAfterStart = false;
//======================================================================================================================
//                                                  �������� ���
//======================================================================================================================

// ��������� �����
void __fastcall TFormMain::OnActivate(TWMActivate& a)
{
	if (a.Active == WA_INACTIVE)  {
		// FormMain->Caption = "[����� ����� ������� ����]";
	} else  {
		// FormMain->Caption = "";
	}
	a.Result = 0;
} // ��������� 2014.01.27 ----------------------------------------------------------------------------------------------

// �������� �����
void __fastcall TFormMain::OnClose(TWMClose& a)
{
	a.Result = 0;
	FormMain->Close();
} // ��������� 2014.01.27 ----------------------------------------------------------------------------------------------

// ���������� ����� � ����� ������
void __fastcall TFormMain::OnWindowPosChanging(TMessage &Message)
{
	const int sensitivity = 10;         // ���������� �������� ��� ����������
	RECT rcWorkArea = {0};
	if (SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, FALSE))  {
		WINDOWPOS *wp = (WINDOWPOS*)Message.LParam;
		if(abs(wp->x - rcWorkArea.left) <= sensitivity)
				 wp->x = rcWorkArea.left;
		if(abs(wp->y - rcWorkArea.top) <= sensitivity)
				 wp->y = rcWorkArea.top;
		if(abs(wp->cx + wp->x - rcWorkArea.right) <= sensitivity)
				wp->x = rcWorkArea.right - wp->cx;
		if(abs(wp->cy + wp->y - rcWorkArea.bottom) <= sensitivity)
				 wp->y = rcWorkArea.bottom - wp->cy;
	}
	TForm::Dispatch(&Message);
} // ��������� 2014.01.27 ----------------------------------------------------------------------------------------------

void __fastcall TFormMain::OnHotKey(TWMHotKey &Message)
{
	switch (Message.HotKey) {
	case 0x0101: // ������ �����
		MenuAppHideClick(this);
		break;
	case 0x0102: // �������� �����
		MenuAppShowClick(this);
		break;
	default: break;
	}
} // ��������� 2014.01.27 ----------------------------------------------------------------------------------------------

// ����������� ���������� ������� ������ � �������
void __fastcall TFormMain::MenuHotKeyRegClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuHotKeyRegClick";
	// RegisterHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk)
	// hWnd - ����
	// id - ������������� ���������� "�������" �������
	//      ���������� ��������� = �� 0x0000 �� 0xBFFF. DLL = �� 0xC000 �� 0xFFFF
	//      DLL ������ ������������ GlobalAddAtom, ����� �������� ���������� � ������� ����������������
	// fsModifiers - �������, ������� ������ ���� ������ � ���������� � ��������, �������� ���������� nVirtKey
	//               MOD_ALT(0x0001), MOD_CONTROL(0x0002), MOD_NOREPEAT(0x4000), MOD_SHIFT(0x0004), MOD_WIN(0x0008)
	// vk - ��� ����������� ������� ���������� "�������" �������
	if ((RegisterHotKey(Handle,0x0101,MOD_CONTROL+MOD_ALT,'H')) && (RegisterHotKey(Handle,0x0102,MOD_CONTROL+MOD_ALT,'S'))) {
		MemoSettings->Lines->Add("����������� ������� ������.");
		AppMain->FileOutWrite(0,0,"����������� ������� ������.");
		LabelHotKey2Active->Caption = "OK";
		LabelHotKey3Active->Caption = "OK";
	}
} // ��������� 2014.02.04 ----------------------------------------------------------------------------------------------

// ����� ����������� ���������� ������� ������ � �������
void __fastcall TFormMain::MenuHotKeyUnRegClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuHotKeyUnRegClick";
	if ((UnregisterHotKey(Handle,0x0101)) && (UnregisterHotKey(Handle,0x0102))) {
		MemoSettings->Lines->Add("����������� ������� ������.");
		AppMain->FileOutWrite(0,0,"����� ����������� ������� ������");
		LabelHotKey2Active->Caption = "-";
		LabelHotKey3Active->Caption = "-";
	}
} // ��������� 2014.02.04 ----------------------------------------------------------------------------------------------

// ����� ���������� ������� ��������
bool __fastcall TFormMain::ProcessGetParams(DWORD dwId, LPWSTR wBuf, DWORD dwBufLen)
{
	CurrentFunction = "TFormMain::ProcessGetParams";
	LONG                      status;
   HANDLE                    hProcess;
   PROCESS_BASIC_INFORMATION pbi;
   PEB                       Peb;
   PROCESS_PARAMETERS        ProcParam;
   DWORD                     dwDummy;
   DWORD                     dwSize;
   LPVOID                    lpAddress;
   // Get process handle
   hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwId);
   if (!hProcess)  return false;
   // Retrieve information
   status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, (PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
   if (status)  CloseHandle (hProcess);
   if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &Peb, sizeof(PEB), &dwDummy))  CloseHandle (hProcess);
   if (!ReadProcessMemory(hProcess, Peb.ProcessParameters, &ProcParam, sizeof(PROCESS_PARAMETERS), &dwDummy))
      CloseHandle (hProcess);
   lpAddress = ProcParam.CommandLine.Buffer;
   dwSize = ProcParam.CommandLine.Length;
   if (dwBufLen<dwSize)  CloseHandle (hProcess);
   if (!ReadProcessMemory(hProcess, lpAddress, wBuf, dwSize, &dwDummy))  CloseHandle (hProcess);
      return true;
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// ����� �������� � ������
bool __fastcall TFormMain::ProcessFind(AnsiString ExeName, AnsiString ExePath)
{
	CurrentFunction = "TFormMain::ProcessFind";
	//MemoStatus->Lines->Add("\""+ExeName+"\"");
   HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
   PROCESSENTRY32 pe;
   // DWORD* Pid;
   // �������� FileName �� Appname
   AnsiString FileName = "";
   if (ExeName.Pos(" ") != 0) {
      FileName = ExeName.SubString(0,ExeName.Pos(" ")-1);
   } else  {
      FileName = ExeName;
   }
   // �������� ��������� ExeParams �� Appname
   AnsiString ExeParams = "";
   if ((FileName.Length() == ExeName.Length()) || (FileName.Length() == ExeName.Length()-1)) {
      ExeParams = "";
   } else  {
      ExeParams	= ExeName.SubString(ExeName.Pos(" "),ExeName.Length());
   }
   // �������� ������� � ������ ���������
   if (hSnapshot == (void*)-1)  {
		MessageBox(NULL, "������ ������� � ������ ���������!", "������", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
      return false;
   }
   //  �������� ������� � NTDLL
	NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(GetModuleHandle("NTDLL"), "NtQueryInformationProcess");
   if (!NtQueryInformationProcess)  {
      MessageDlg("[!] ������ ������� � NTDLL", mtError, TMsgDlgButtons() << mbOK, 0);
      return false;
   }
   WCHAR wstr[255];
   // ����� � ������ ��������
   Process32First(hSnapshot, &pe);                                     // ����� � ������ ��������
   pe.dwSize = sizeof(PROCESSENTRY32);
   if ((AnsiString(pe.szExeFile).UpperCase()).Pos((FileName).UpperCase()))  { // ���� ������ �������
      ZeroMemory(&wstr, sizeof(wstr));                              // ������ wstr
      if (ProcessGetParams(pe.th32ProcessID,wstr,sizeof(wstr)))  {  // �������� ��� ���������
         if (ExeParams != "")  {                                    // ���� ������� ExeParams
            if ((AnsiString(wstr).UpperCase()).Pos(ExeParams.UpperCase()))  {   // ��������� �������
               CloseHandle (hSnapshot);
               return true;
            }
         } else {                                                   // ���� ��� ExeParams
            /*
            if (ExePath == "") {                                    // ���� ExePath ��� - �� ���� ��� �������
               if (AnsiString(wstr).UpperCase() == (ExePath+FileName).UpperCase())  {   // ���� ��������� �������
                  CloseHandle (hSnapshot);
                  return true;
               }
            } else  {                                               // ���� ExePath ���� - �� ���� � ���������
               if (AnsiString(wstr).UpperCase() == "\""+(ExePath+FileName).UpperCase()+"\"")  {   // ���� ��������� �������
                  CloseHandle (hSnapshot);
                  return true;
               }
            }
            */
            CloseHandle (hSnapshot);
            return true;
         }
      }
   }
   // ����� � ��������� ���������
   while (Process32Next(hSnapshot,&pe))  {                             // ����� � ��������� ���������
      pe.dwSize = sizeof(PROCESSENTRY32);
      if ((AnsiString(pe.szExeFile).UpperCase()).Pos((FileName).UpperCase()))  { // ���� ������ �������
         ZeroMemory(&wstr, sizeof(wstr));                              // ������ wstr
         if (ProcessGetParams(pe.th32ProcessID,wstr,sizeof(wstr)))  {  // �������� ��� ���������
            if (ExeParams != "")  {                                    // ���� ������� ExeParams
               if ((AnsiString(wstr).UpperCase()).Pos(ExeParams.UpperCase()))  {   // ��������� �������
                  CloseHandle (hSnapshot);
                  return true;
               }
            } else {                                                   // ���� ��� ExeParams
               // if (AnsiString(wstr).UpperCase() == (ExePath+FileName).UpperCase())  {   // ���� ��������� �������
               // if (AnsiString(wstr).UpperCase() == "\""+(ExePath+FileName).UpperCase()+"\"")  {   // ���� ��������� �������
               // �������� ��������� ExeParams �� Appname
               /*
               AnsiString ProcessParams = "";
               if (AnsiString(wstr).Pos(" "))  {
                  ProcessParams = AnsiString(wstr).SubString(AnsiString(wstr).Pos(" "),AnsiString(wstr).Length());
                  } else {
                     ProcessParams = "";
                  }
               MemoStatus->Lines->Add(ProcessParams);
               */
               //if ((AnsiString(wstr).UpperCase()).Pos(ExeParams.UpperCase()))  {
               CloseHandle (hSnapshot);
               return true;
               //}
            }
         }
      }
   }
   CloseHandle(hSnapshot);
   return false;
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// �����������
__fastcall TFormMain::TFormMain(TComponent* Owner) : TForm(Owner)
{
	// ������������� 2014.02.27
	AppMain                    = new TmApp;
	AppMain->COMEnable         = false;
	AppMain->FrameEnable       = true;
	AppMain->LogEnable         = true;
	AppMain->ReportEnable      = false;
	AppMain->SettingsEnable    = true;
	AppMain->SettingsIniEnable = true;
	AppMain->SocketsEnable     = false;
	AppMain->SQLEnable         = false;
	AppInitAll();
	// ��������� ��-���������
	Caption                          = Application->Title+" [����� " + GetVers3dig+"]";
	StringGridSettings->ColWidths[0] = 575;
	TimeLast                         = Now()-1.0/60/24;
	PageControlMain->ActivePage      = TabSheetStatus;
	MemoStatus->OnChange             = AppMain->MemoChange;
	MemoReport->OnChange             = AppMain->MemoChange;
	MemoSettings->OnChange           = AppMain->MemoChange;
	FrameAppMain->ButtonCreateDesktopLink->Parent = TabSheetSettings;
	FrameAppMain->ButtonCreateDesktopLink->Left = 355;
	FrameAppMain->ButtonCreateDesktopLink->Top  =  45;
	FrameAppMain->GroupBoxSkins->Parent         = TabSheetSettings;
	FrameAppMain->GroupBoxSkins->Left           = 385;
	FrameAppMain->GroupBoxSkins->Top            =  75;
	FrameAppMain->GroupBoxSkins->Width          = 220;
	FrameAppMain->GroupBoxSkins->Height         =  85;
	FrameAppMain->CheckBoxSkinUse->Left         =  15;
	FrameAppMain->CheckBoxSkinUse->Top          =  15;
	FrameAppMain->CheckBoxExtendedBorders->Left =  15;
	FrameAppMain->CheckBoxExtendedBorders->Top  =  35;
	FrameAppMain->ComboBoxSkinsList->Left       =  15;
	FrameAppMain->ComboBoxSkinsList->Top        =  55;
	// ���������� ����
	MenuTitle->Caption = Application->Title+" - �������";
	MenuTitle->OnClick = FrameAppMain->MenuHelpClick;
	PopupMenuMain->Items->Add(NewLine());
	PopupMenuMain->Items->Add(NewItem("����������",   0, False, True, FrameAppMain->MenuRestartClick,  0, "MenuRestart"));
	PopupMenuMain->Items->Add(NewItem("�����",        0, False, True, MenuAppExitClick,                0, "MenuExit"));
	// ������������� INI
	fSettings = new TIniFile(ChangeFileExt(Application->ExeName,".ini"));
} // ��������� 2014.02.04 ----------------------------------------------------------------------------------------------

// �������� ������� �����
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
	CurrentFunction = "TFormMain::FormCreate";
	AppMain->FileOutWrite(0,0,"������ ���������");
	// INIMain->FileNameWrite(ChangeFileExt(Application->ExeName,".ini"));
	// INIMain->Create();
	// �������� INI-�����
	INILoad(Sender);
} // ��������� 2014.02.04 ----------------------------------------------------------------------------------------------

// ������ �����������
void __fastcall TFormMain::ButtonStartMonitorClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::ButtonStartMonitorClick";
	// ���������
	ObjectsEnabled(Name,false);
	ButtonStopMonitor->Enabled         = true;
	MenuStopMonitor->Enabled           = true;
	PageControlMain->Enabled           = true;
	TabSheetStatus->Enabled            = true;
	TabSheetReport->Enabled            = true;
	TabSheetSettings->Enabled          = true;
	MemoStatus->Enabled                = true;
	MemoReport->Enabled                = true;
	PageControlMain->ActivePage        = TabSheetStatus;
	// ����� TimerMonitor
	Caption = Application->Title+" [����� " + GetVers3dig+"]"+" [�����: " + DateToStr(Now())+" "+TimeToStr(Now())+"]";
	MemoStatus->Clear();
	MemoStatus->Lines->Add(TimeToStr(Now())+" [ ] �����");
	AppMain->FileOutWrite(0,0,"�����");
	TimerMonitor->Enabled              = true;
	TimerMonitorTimer(Sender);
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// ������� �����������
void __fastcall TFormMain::ButtonStopMonitorClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::ButtonStopMonitorClick";
	// ���������
/*	ButtonStartMonitor->Enabled        = true;
	MenuStartMonitor->Enabled          = true;
	ButtonStopMonitor->Enabled         = false;
	MenuStopMonitor->Enabled           = false;
	ButtonINICreate->Enabled           = true;
	ButtonINIDelete->Enabled           = true;
	ButtonINILoad->Enabled             = true;
	ButtonINIUnload->Enabled           = true;
	ButtonINIClear->Enabled            = true;
	ButtonINIRecSave->Enabled          = true;
	ButtonINIRecDel->Enabled           = true;
	ButtonINIRecAdd->Enabled           = true;
	ComboBoxSettings->Enabled          = true;
	CheckBoxAutoStart->Enabled         = true;
	EditTimeOutH->Enabled              = true;
	EditTimeOutM->Enabled              = true;
	EditTimeOutS->Enabled              = true;
*/
	ObjectsEnabled(Name,true);
	// ������� TimerMonitor
	Caption                            = Application->Title+" [����� " + GetVers3dig+"]";
	TimerMonitor->Enabled              = false;
	MemoStatus->Lines->Add(TimeToStr(Now())+" [ ] ����");
	AppMain->FileOutWrite(0,0,"����");
	TimeLast = Now()-1.0/60/24;
	LogWriteOptimise = false;
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// �������� �����
void __fastcall TFormMain::MenuAppShowClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuAppShowClick";
	::ShowWindow(Application->Handle,SW_SHOW);
	::ShowWindow(FormMain->Handle,SW_SHOW);
	MenuAppHide->Enabled = true;
	MenuAppShow->Enabled = false;
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// ������ �����
void __fastcall TFormMain::MenuAppHideClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuAppHideClick";
	::ShowWindow(Application->Handle,SW_HIDE);
	::ShowWindow(FormMain->Handle,SW_HIDE);
	MenuAppHide->Enabled = false;
	MenuAppShow->Enabled = true;
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// ����� �� ���������
void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	CurrentFunction = "TFormMain::FormClose";
	// ���������� �������� ������ � ini
	INISave(Sender);
	// ����� ����������� ���������� ������
	MenuHotKeyUnRegClick(Sender);
	// ����
	AppMain->FileOutWrite(0,0,"����� �� ���������");
	TimerMonitor->Enabled = false;
	// ��� ���������� ������ � ������� ������
	delete fSettings;
	AppCloseAll();
} // ��������� 2014.02.02 ----------------------------------------------------------------------------------------------

// ����� �� ���������
void __fastcall TFormMain::MenuAppExitClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuAppExitClick";
	INIUnload(Sender);
	Close();
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// ������� ���������
void __fastcall TFormMain::TimerMonitorTimer(TObject *Sender)
{
	CurrentFunction = "TFormMain::TimerMonitorTimer";
	// ���������� ���� ���-������
	AppMain->FilesOutCheck();
	// ������ ������ �� INI-����� � ��������� ��������
   MemoStatus->Clear();
   MemoStatus->Lines->Add(TimeToStr(Now())+" [ ] �����");
	for (register unsigned short i=0; i<fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0); i++) {
		AppName   = fSettings->ReadString(ComboBoxSettings->Text, "AppName"+IntToStr(i+1),     "");
		AppFolder = fSettings->ReadString(ComboBoxSettings->Text, "AppFolder"+IntToStr(i+1), "");
      // ����� ����������� �������� � ������ � ����������� ���������� ������
      if (ProcessFind(AppName.UpperCase(),AppFolder.UpperCase()))  {
         MemoStatus->Lines->Add(TimeToStr(Now())+" [ ] ������� \""+AppName+"\" �������");
      } else  {
      try {
			if ((double)(TimeOutMax-(Now()-TimeLast)) <= 0)  {   // ��������� ������ ���� ������ ������ TimeMax
				MemoStatus->Lines->Add(TimeToStr(Now())+" [S] ������ �������� \""+AppName+"\"");
				MemoReport->Lines->Add(FormatDateTime("hh:nn:ss",Now())+" ������ �������� \""+AppName+"\"");
				AppMain->FileOutWrite(0,0,"������ �������� \""+AppName+"\"");  // ���
				AppMain->FileOutWrite(1,0,"������ �������� \""+AppName+"\"");  // �����
				STARTUPINFO           si;
            PROCESS_INFORMATION   pi;
            ZeroMemory(&si, sizeof(si));                              // ������ si
            ZeroMemory(&pi, sizeof(pi));                              // ������ pi
				memset(&si, 0, sizeof(si));                               // �������� ������ si
            memset(&pi, 0, sizeof(pi));                               // �������� ������ pi
            si.cb = sizeof(si);
            if (AppFolder == "")
					CreateProcess(NULL, AppName.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
				else
					CreateProcess(NULL, (AppFolder+AppName).c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, AppFolder.c_str(), &si, &pi);
            TimeLast = Now();
            LogWriteOptimise = false;
            //Sleep(5000);
         } else {
				MemoStatus->Lines->Add(TimeToStr(Now())+" [!] ������ ������������ ������� "+FormatDateTime("hh:mm:ss", TimeOutMax-(Now()-TimeLast)));
				MemoStatus->Lines->Add(TimeToStr(Now())+" [ ] ����-��� ��������: "+FormatDateTime("hh:mm:ss", TimeOutMax));
            if (LogWriteOptimise == false) {
					AppMain->FileOutWrite(0,0,"������ ������������ �������");
					AppMain->FileOutWrite(0,0,"����-��� ��������: "+FormatDateTime("hh:mm:ss", TimeOutMax));
               LogWriteOptimise = true;
            }
         }
      } catch (Exception &e)  {
         MemoStatus->Lines->Add(TimeToStr(Now())+" [!] ������ ������� "+AppName+"!\n"+"[!] "+e.Message);
			AppMain->FileOutWrite(0,2,"������ ������� "+AppName+"!\n"+"[!] "+e.Message);
		}
		catch (...)  {
			try  {
				throw Exception("");
			} catch (Exception &e)  {
				MemoStatus->Lines->Add(TimeToStr(Now())+" [!] ������ ������� "+AppName+"!\n"+"[!] "+e.Message);
				AppMain->FileOutWrite(0,0,"������ ������� "+AppName+"!\n"+"[!] "+e.Message);
			}
      }
   }
   }
} // ��������� 2014.02.04 ----------------------------------------------------------------------------------------------

// ���������� �������� � �������� ���� "� ���������"
void __fastcall TFormMain::ButtonAboutClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::ButtonAboutClick";
	Application->CreateForm(__classid(TFormHelp), &FormHelp);
	FormHelp->ShowModal();
	FormHelp->Free();
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// INI - ������� ���������
void __fastcall TFormMain::ButtonINICreateClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::ButtonINICreateClick";
	INICreate(Sender, ComboBoxSettings->Text);
} // ��������� 2014.02.03 ----------------------------------------------------------------------------------------------

// INI - ������� ���������
void __fastcall TFormMain::INICreate(TObject *Sender, AnsiString SettingsName)
{
	CurrentFunction = "TFormMain::INICreate";
	try {
		if (SettingsName=="") {
			MemoSettings->Lines->Add("�� ������ ��� ������������! ������ ��-���������.");
			ComboBoxSettings->Text = AppMain->CompName;
			SettingsName           = ComboBoxSettings->Text;
		}
		fSettings->WriteString (SettingsName, "ComputerName", SettingsName);
		fSettings->WriteBool   (SettingsName, "AutoStart",    false);
		fSettings->WriteBool   (SettingsName, "HotKeysUse",   false);
		fSettings->WriteBool   (SettingsName, "SkinUse",      false);
		fSettings->WriteString (SettingsName, "SkinName",     "");
		fSettings->WriteBool   (SettingsName, "SkinExtendedBorders",      false);
		fSettings->WriteInteger(SettingsName, "Count",        0);
		// ���������
		ButtonINIDelete->Enabled = true;
		ButtonINILoad->Enabled = true;
		MemoSettings->Lines->Add("������������ ������� �������.");
		// �������� �������� ������
		EditAppName->Text = "calc";
		INIRecAdd(Sender);
		MemoSettings->Lines->Add("��������� �������� ������.");
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"������ �������� ������������");
		MsgBoxWithTimeOut(NULL,"������ �������� ������������!", MB_OK+MB_ICONERROR,2500);
	}
} // ��������� 2014.02.03 ----------------------------------------------------------------------------------------------

// INI - ������� ���� ��������
void __fastcall TFormMain::INIDelete(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIDelete";
	try {
		if (MessageDlg("�� ��������, ��� ����� ������� ���� ��������?", mtConfirmation, TMsgDlgButtons() << mbNo << mbYes , 0) == mrYes)  {
			INIUnload(Sender);
			DeleteFile(ChangeFileExt(Application->ExeName,".ini"));
			ButtonINICreate->Enabled         = true;
			ButtonINIDelete->Enabled         = false;
			CheckBoxHotKeysRegister->Checked = false;
			FrameAppMain->CheckBoxSkinUse->Checked = false;
			MemoSettings->Lines->Add("���� �������� ��������� � ������.");
		}
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"������ �������� ����� ��������");
		MsgBoxWithTimeOut(NULL,"������ �������� ����� ��������!",MB_OK+MB_ICONERROR,2500);
	}
} // ��������� 2014.02.04 ----------------------------------------------------------------------------------------------

// INI - ������ ��������
void __fastcall TFormMain::INILoad(TObject *Sender)
{
	CurrentFunction = "TFormMain::INILoad";
	try {
		INIUnload(Sender);
		// ���������� ���������� ����� ��������� ������������
		ComboBoxSettings->Items->Clear();
		StringGridSettings->RowCount = 1;
		StringGridSettings->Rows[0]->Text = "";
		// ��������� ������ ������������
		if (fSettings->SectionExists(NULL))
			fSettings->EraseSection(NULL);
		// �������� �������� �� INI-�����
		fSettings->ReadSections(ComboBoxSettings->Items);
		if (ComboBoxSettings->Items->Count==0)  {  // ���� ��� ��������
			ButtonINICreate->Enabled = true;
			MemoSettings->Lines->Add("�������� �� ����������!");
		} else {                                   // ��������� ������
			for (register unsigned short i=0; i<ComboBoxSettings->Items->Count; i++) {
				ComboBoxSettings->ItemIndex = i;
				if (ComboBoxSettings->Text=="")  {
					if (MessageDlg("��������� �� �������!\n������� ������������ ��-���������?", mtConfirmation, TMsgDlgButtons() << mbNo << mbYes , 0) == mrYes)  {
						ComboBoxSettings->Text = AppMain->CompName;
						// ������� ��������� ��-���������
						INICreate(Sender,AppMain->CompName);
					} else  {
						MsgBoxWithTimeOut(NULL,"������������ ��-��������� �� ����������!",MB_OK+MB_ICONERROR,2500);
					}
				}
			}
			// ���������
			ButtonINIRecAdd->Enabled          = true;
			ButtonINIRecDel->Enabled          = true;
			ButtonINIRecSave->Enabled         = true;
			ButtonINIUnload->Enabled          = true;
			ButtonINIClear->Enabled           = true;
			CheckBoxAutoStart->Enabled        = true;
			ButtonStartMonitor->Enabled       = true;
			ButtonStopMonitor->Enabled        = false;
			EditAppName->Enabled              = true;
			EditAppFolder->Enabled            = true;
			EditAppName->Text                 = "";
			EditAppFolder->Text               = "";
			EditTimeOutH->Enabled             = true;
			EditTimeOutM->Enabled             = true;
			EditTimeOutS->Enabled             = true;
			CheckBoxHotKeysRegister->Enabled  = true;
			FrameAppMain->CheckBoxSkinUse->Enabled = true;
			ButtonSettingsSave->Enabled        = true;
			// ��������� � ������� ������ ���� ����������
			StringGridSettings->RowCount      = 1;
			StringGridSettings->Rows[0]->Text = "";
			for (register unsigned short j=0; j<fSettings->ReadInteger(ComboBoxSettings->Text,"Count",0); j++) {
				StringGridSettings->Cells[0][StringGridSettings->RowCount-1] =
				fSettings->ReadString(ComboBoxSettings->Text, "AppName"+IntToStr(j+1), "");
				StringGridSettings->RowCount++;
			}
			StringGridSettings->RowCount--;
			// SettingsName
			ComboBoxSettings->Text = fSettings->ReadString(ComboBoxSettings->Text, "ComputerName", fSettings->ReadString(ComboBoxSettings->Text, "ComputerName", AppMain->CompName));
			// AutoStart
			CheckBoxAutoStart->Checked = fSettings->ReadBool(ComboBoxSettings->Text, "AutoStart", false);
			// TimeWait
			EditTimeOutH->Text = fSettings->ReadInteger(ComboBoxSettings->Text, "TimeWaitH", 0);
			EditTimeOutM->Text = fSettings->ReadInteger(ComboBoxSettings->Text, "TimeWaitM", 1);
			EditTimeOutS->Text = fSettings->ReadInteger(ComboBoxSettings->Text, "TimeWaitS", 0);
			TimeOutMax = EncodeTime(StrToInt(EditTimeOutH->Text), StrToInt(EditTimeOutM->Text), StrToInt(EditTimeOutS->Text), 0);
			// HotKeys
			if (fSettings->ReadBool(ComboBoxSettings->Text, "HotKeysUse", false)) {
				CheckBoxHotKeysRegister->Checked = true;
				CheckBoxHotKeysRegisterClick(this);
			}
			// SkinUse
			if (fSettings->ReadBool(ComboBoxSettings->Text, "SkinUse", false)) {
				FrameAppMain->SkinsCheckFolder();
				//FrameAppMain->ComboBoxSkinsList->Text = fSettings->ReadString(ComboBoxSettings->Text,"SkinName","");
				FrameAppMain->ComboBoxSkinsList->ItemIndex = FrameAppMain->ComboBoxSkinsList->Items->IndexOf(fSettings->ReadString(ComboBoxSettings->Text,"SkinName",""));
				FrameAppMain->ComboBoxSkinsListCloseUp(this);
				FrameAppMain->CheckBoxSkinUse->Checked = true;
				FrameAppMain->CheckBoxSkinUseClick(this);
				FrameAppMain->CheckBoxExtendedBorders->Checked = fSettings->ReadBool(ComboBoxSettings->Text,"SkinExtendedBorders",false);
				FrameAppMain->CheckBoxExtendedBordersClick(this);
			}
			// �������� AutoStart
			if (CheckBoxAutoStart->Checked)  ButtonStartMonitorClick(Sender);
			// ���
			MemoSettings->Lines->Add("��������� ������� ����������.");
		}
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"������ �������� ��������");
		MsgBoxWithTimeOut(NULL, "������ �������� ��������!",MB_OK+MB_ICONERROR,2500);
	}
} // ��������� 2014.02.04 ----------------------------------------------------------------------------------------------

// INI - �������� ��������
void __fastcall TFormMain::INIUnload(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIUnload";
	try  {
		ComboBoxSettings->Items->Clear();
		ComboBoxSettings->Text             = "";
      CheckBoxAutoStart->Enabled         = false;
		ButtonINIRecAdd->Enabled           = false;
      ButtonINIRecDel->Enabled           = false;
      ButtonINIRecSave->Enabled          = false;
      ButtonINIUnload->Enabled           = false;
      ButtonINIClear->Enabled            = false;
		ButtonStartMonitor->Enabled        = false;
      ButtonStopMonitor->Enabled         = false;
      StringGridSettings->RowCount       = 1;
      StringGridSettings->Rows[0]->Text  = "";
      EditAppName->Enabled               = false;
		EditAppFolder->Enabled             = false;
		EditAppName->Text                  = "";
		EditAppFolder->Text                = "";
		EditTimeOutH->Text                 = "0";
		EditTimeOutM->Text                 = "0";
		EditTimeOutS->Text                 = "0";
		EditTimeOutH->Enabled             = false;
		EditTimeOutM->Enabled             = false;
		EditTimeOutS->Enabled             = false;
		TimeOutMax = EncodeTime(StrToInt(EditTimeOutH->Text), StrToInt(EditTimeOutM->Text), StrToInt(EditTimeOutS->Text), 0);
		CheckBoxHotKeysRegister->Enabled   = false;
		FrameAppMain->CheckBoxSkinUse->Enabled = false;
		ButtonSettingsSave->Enabled        = false;
		MemoSettings->Lines->Add("��������� ������� ����������.");
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"������ �������� ��������");
		MsgBoxWithTimeOut(NULL,"������ �������� ��������!",MB_OK+MB_ICONERROR,2500);
	}
} // ��������� 2014.02.04 ----------------------------------------------------------------------------------------------

// INI - ���������
void __fastcall TFormMain::INISave(TObject *Sender)
{
	CurrentFunction = "TFormMain::INISave";
	try {
		if (ComboBoxSettings->Text=="") {
			MsgBoxWithTimeOut(NULL,"�� ������ ��� �����������!",MB_OK+MB_ICONINFORMATION,2500);
			return;
		}
		if (ComboBoxSettings->Text!="") {
			// SettingsName
			fSettings->WriteString(ComboBoxSettings->Text, "ComputerName", fSettings->ReadString(ComboBoxSettings->Text, "ComputerName", AppMain->CompName));
			// AutoStart
			fSettings->WriteBool(ComboBoxSettings->Text, "AutoStart", CheckBoxAutoStart->Checked);
			// TimeWait
			fSettings->WriteInteger(ComboBoxSettings->Text, "TimeWaitH", StrToInt(EditTimeOutH->Text));
			fSettings->WriteInteger(ComboBoxSettings->Text, "TimeWaitM", StrToInt(EditTimeOutM->Text));
			fSettings->WriteInteger(ComboBoxSettings->Text, "TimeWaitS", StrToInt(EditTimeOutS->Text));
			TimeOutMax = EncodeTime(StrToInt(EditTimeOutH->Text), StrToInt(EditTimeOutM->Text), StrToInt(EditTimeOutS->Text), 0);
			// HotKeys
			fSettings->WriteBool(ComboBoxSettings->Text,   "HotKeysUse",          CheckBoxHotKeysRegister->Checked);
			// SkinUse
			fSettings->WriteBool(ComboBoxSettings->Text,   "SkinUse",             FrameAppMain->CheckBoxSkinUse->Checked);
			if (FrameAppMain->CheckBoxSkinUse->Checked) { fSettings->WriteString(ComboBoxSettings->Text, "SkinName", FrameAppMain->ComboBoxSkinsList->Text);
				fSettings->WriteBool(ComboBoxSettings->Text,   "SkinExtendedBorders", FrameAppMain->CheckBoxExtendedBorders->Checked);
			} else {
				fSettings->WriteString(ComboBoxSettings->Text, "SkinName",            "");
				fSettings->WriteBool(ComboBoxSettings->Text,   "SkinExtendedBorders", false);
			}
			// Count
			fSettings->WriteInteger(ComboBoxSettings->Text, "Count", fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0));
			// ���
			MsgBoxWithTimeOut(NULL,"��������� ������������ ������� ���������.",MB_OK+MB_ICONINFORMATION,2500);
		}
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"������ ���������� ��������");
		MsgBoxWithTimeOut(NULL,"������ ���������� ��������!",MB_OK+MB_ICONERROR,2500);
	}
} // ��������� 2014.02.03 ----------------------------------------------------------------------------------------------

// INI - �������� ������������
void __fastcall TFormMain::INIClear(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIClear";
	try {
		if (MessageDlg("�� ��������, ��� ����� �������� ������������?", mtConfirmation, TMsgDlgButtons() << mbNo << mbYes , 0) == mrYes)  {
				fSettings->EraseSection(ComboBoxSettings->Text);
				MemoSettings->Lines->Add("������������ �������."+ComboBoxSettings->Text);
				ButtonINIClear->Enabled = false;
		}
		INILoad(Sender);
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"������ �������� ��������");
		MsgBoxWithTimeOut(NULL,"������ �������� ��������!",MB_OK+MB_ICONERROR,2500);
	}
} // ��������� 2014.02.04 ----------------------------------------------------------------------------------------------

// INI - �������� ������
void __fastcall TFormMain::INIRecAdd(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIRecAdd";
	if ((ComboBoxSettings->Text != "") && (EditAppName->Text != "")) {
		fSettings->WriteString (ComboBoxSettings->Text, "AppName"+IntToStr(fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)+1), EditAppName->Text);
		fSettings->WriteString (ComboBoxSettings->Text, "AppFolder"+IntToStr(fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)+1), EditAppFolder->Text);
		fSettings->WriteInteger(ComboBoxSettings->Text, "Count", fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)+1);
		INILoad(Sender);
	} else {
		MemoSettings->Lines->Add("��������� ��������� ����:");
		if (ComboBoxSettings->Text == "")
			MemoSettings->Lines->Add("    - ��� ������������");
		if (EditAppName->Text == "")
			MemoSettings->Lines->Add("    - ��� ��������, ��������: iexplore.exe");
		if (EditAppFolder->Text == "")
			MemoSettings->Lines->Add("    - ��� �����,    ��������: c:\\Program Files\\Internet Explorer\\");
	}
} // ��������� 2014.02.03 ----------------------------------------------------------------------------------------------

// INI - ������� ������
void __fastcall TFormMain::INIRecDel(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIRecDel";
	if (StringGridSettings->Cells[0][StringGridSettings->Row] != "") {
		// ������� ���������� ����
		fSettings->DeleteKey(ComboBoxSettings->Text, "AppName"+IntToStr(StringGridSettings->Row+1));
		fSettings->DeleteKey(ComboBoxSettings->Text, "AppFolder"+IntToStr(StringGridSettings->Row+1));
		fSettings->WriteInteger(ComboBoxSettings->Text, "Count", fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)-1);
      // ����������� ����� � �����
      for (int i = StringGridSettings->Row+1; i <= fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0); i++) {
         fSettings->WriteString(ComboBoxSettings->Text, "AppName"+IntToStr(i),
         fSettings->ReadString(ComboBoxSettings->Text, "AppName"+IntToStr(i+1), ""));
         fSettings->WriteString(ComboBoxSettings->Text, "AppFolder"+IntToStr(i),
         fSettings->ReadString(ComboBoxSettings->Text, "AppFolder"+IntToStr(i+1), ""));
      }
      // ������� ��������� ����
      fSettings->DeleteKey(ComboBoxSettings->Text, "AppName"+IntToStr(fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)+1));
      fSettings->DeleteKey(ComboBoxSettings->Text, "AppFolder"+IntToStr(fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)+1));
      MemoSettings->Lines->Add("���� ������� �����!");
		INILoad(Sender);
   } else {
      MemoSettings->Lines->Add("�������� ����!");
   }
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// INI - �������� ������
void __fastcall TFormMain::INIRecSave(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIRecSave";
   if (StringGridSettings->Cells[0][StringGridSettings->Row] != "") {
      // �������� ����
      fSettings->WriteString(ComboBoxSettings->Text, "AppName"     +IntToStr(StringGridSettings->Row+1), EditAppName->Text);
      fSettings->WriteString(ComboBoxSettings->Text, "AppFolder" +IntToStr(StringGridSettings->Row+1), EditAppFolder->Text);
      MemoSettings->Lines->Add("���� ������� ������!");
      INILoad(Sender);
   } else {
      MemoSettings->Lines->Add("�������� ����!");
   }
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// ��������� ����� ��� ������ ������
void __fastcall TFormMain::StringGridSettingsClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::StringGridSettingsClick";
	EditAppName->Text   = fSettings->ReadString(ComboBoxSettings->Text, "AppName"+IntToStr(StringGridSettings->Row+1),   "");
	EditAppFolder->Text = fSettings->ReadString(ComboBoxSettings->Text, "AppFolder"+IntToStr(StringGridSettings->Row+1), "");
} // ��������� 2012.10.12 ----------------------------------------------------------------------------------------------

// ������� Dam33
void __fastcall TFormMain::LabelLinkClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::LabelLinkClick";
	ShellExecute(Application->Handle,"open","http://forum-pro-it.ru/",NULL,NULL,SW_SHOWNORMAL);
} // ��������� 2014.01.26 ----------------------------------------------------------------------------------------------

void __fastcall TFormMain::LabelLinkMouseEnter(TObject *Sender)
{
	CurrentFunction = "TFormMain::LabelLinkMouseEnter";
	LabelLink->Font->Color = clBlue;
} // ��������� 2014.01.27 ----------------------------------------------------------------------------------------------

void __fastcall TFormMain::LabelLinkMouseLeave(TObject *Sender)
{
	CurrentFunction = "TFormMain::LabelLinkMouseLeave";
	LabelLink->Font->Color = clBlack;
} // ��������� 2014.01.27 ----------------------------------------------------------------------------------------------// ��������/��������� ����

// ����������� ������� ������
void __fastcall TFormMain::CheckBoxHotKeysRegisterClick(TObject *Sender)
{
	if (CheckBoxHotKeysRegister->Checked) {
		MenuHotKeyRegClick(Sender);
	} else {
		MenuHotKeyUnRegClick(Sender);
	}
} // ��������� 2014.02.02 ----------------------------------------------------------------------------------------------

// ��������� ����-���� ��������
void __fastcall TFormMain::EditTimeOutHChange(TObject *Sender)
{
	CurrentFunction = "TFormMain::EditTimeOutHChange";
	if (EditTimeOutH->Text=="") { EditTimeOutH->Text=0; }
	if (EditTimeOutM->Text=="") { EditTimeOutM->Text=0; }
	if (EditTimeOutS->Text=="") { EditTimeOutS->Text=0; }
	if (StrToInt(EditTimeOutH->Text)<0 || StrToInt(EditTimeOutH->Text)>23 ||
		 StrToInt(EditTimeOutM->Text)<0 || StrToInt(EditTimeOutM->Text)>59 ||
		 StrToInt(EditTimeOutS->Text)<0 || StrToInt(EditTimeOutS->Text)>59) {
		MsgBoxWithTimeOut(NULL,"�������� ����-���� �������� �� �����!\n��������� ��������!",MB_OK+MB_ICONINFORMATION,2500);
		return;
	}
	TimeOutMax = EncodeTime(StrToInt(EditTimeOutH->Text), StrToInt(EditTimeOutM->Text), StrToInt(EditTimeOutS->Text), 0);
} // ��������� 2014.02.02 ----------------------------------------------------------------------------------------------

// �������� ��� ��������
void __fastcall TFormMain::MenuogClearClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuogClearClick";
	MemoSettings->Clear();
} // ��������� 2014.02.02 ----------------------------------------------------------------------------------------------

// ������ ��� ����������
void __fastcall TFormMain::MenuSettingsNameInsertClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuogClearClick";
	ComboBoxSettings->Text = AppMain->CompName;
} // ��������� 2014.02.02 ----------------------------------------------------------------------------------------------


