//----------------------------------------------------------------------------------------------------------------------
#ifndef INC_VCL_SHORT                            // Инициализация INC_VCL_SHORT в файле PCH
   #define INC_VCL_SHORT
#endif
#include "PCH.h"                                 // Предварительно откомпилированные заголовки
#pragma hdrstop                                  // Конец предкомпиляции
#include "UnitMain.h"                            // Включить заголовочный файл главного модуля
#pragma package(smart_init)                      // Последовательность пакетов инициализации
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma resource "*.dfm"                         // Форма
#include <tlhelp32.h>                            // Включить заголовочный файл tlhelp32
#include "UnitHelp.h"                            // Включить заголовочный файл модуля справки
#include "UnitFrameApp.h"                        // Включить заголовочный файл модуля приложения
//======================================================================================================================
//                                          Глобальные функции и переменные
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
TDateTime TimeOutMax;                        // Тайм-аут на повторный запуск
TDateTime TimeLast;                          // Время запуска процесса. Минус 1 минута для первого запуска
bool LogWriteOptimise  = false;              // Оптимизация записи лога
unsigned long SizeName = 255;
TIniFile*  fSettings;
// TmINI      *INIMain = new TmINI;
bool RunAfterStart = false;
//======================================================================================================================
//                                                  Исходный код
//======================================================================================================================

// Активация формы
void __fastcall TFormMain::OnActivate(TWMActivate& a)
{
	if (a.Active == WA_INACTIVE)  {
		// FormMain->Caption = "[фокус ввода покинул окно]";
	} else  {
		// FormMain->Caption = "";
	}
	a.Result = 0;
} // Обновлено 2014.01.27 ----------------------------------------------------------------------------------------------

// Закрытие формы
void __fastcall TFormMain::OnClose(TWMClose& a)
{
	a.Result = 0;
	FormMain->Close();
} // Обновлено 2014.01.27 ----------------------------------------------------------------------------------------------

// Прилипание формы к краям экрана
void __fastcall TFormMain::OnWindowPosChanging(TMessage &Message)
{
	const int sensitivity = 10;         // количество пикселей для прилипания
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
} // Обновлено 2014.01.27 ----------------------------------------------------------------------------------------------

void __fastcall TFormMain::OnHotKey(TWMHotKey &Message)
{
	switch (Message.HotKey) {
	case 0x0101: // Скрыть форму
		MenuAppHideClick(this);
		break;
	case 0x0102: // Показать форму
		MenuAppShowClick(this);
		break;
	default: break;
	}
} // Обновлено 2014.01.27 ----------------------------------------------------------------------------------------------

// Регистрация глобальных горячих клавиш в системе
void __fastcall TFormMain::MenuHotKeyRegClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuHotKeyRegClick";
	// RegisterHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk)
	// hWnd - окно
	// id - идентификатор комбинации "горячая" клавиша
	//      Прикладная программа = от 0x0000 до 0xBFFF. DLL = от 0xC000 до 0xFFFF
	//      DLL должна использовать GlobalAddAtom, чтобы избежать конфликтов с другими идентификаторами
	// fsModifiers - клавиши, которые должны быть нажаты в комбинации с клавишей, заданной параметром nVirtKey
	//               MOD_ALT(0x0001), MOD_CONTROL(0x0002), MOD_NOREPEAT(0x4000), MOD_SHIFT(0x0004), MOD_WIN(0x0008)
	// vk - код виртуальной клавиши комбинации "горячая" клавиша
	if ((RegisterHotKey(Handle,0x0101,MOD_CONTROL+MOD_ALT,'H')) && (RegisterHotKey(Handle,0x0102,MOD_CONTROL+MOD_ALT,'S'))) {
		MemoSettings->Lines->Add("Регистрация горячих клавиш.");
		AppMain->FileOutWrite(0,0,"Регистрация горячих клавиш.");
		LabelHotKey2Active->Caption = "OK";
		LabelHotKey3Active->Caption = "OK";
	}
} // Обновлено 2014.02.04 ----------------------------------------------------------------------------------------------

// Сброс регистрации глобальных горячих клавиш в системе
void __fastcall TFormMain::MenuHotKeyUnRegClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuHotKeyUnRegClick";
	if ((UnregisterHotKey(Handle,0x0101)) && (UnregisterHotKey(Handle,0x0102))) {
		MemoSettings->Lines->Add("Регистрация горячих клавиш.");
		AppMain->FileOutWrite(0,0,"Сброс регистрации горячих клавиш");
		LabelHotKey2Active->Caption = "-";
		LabelHotKey3Active->Caption = "-";
	}
} // Обновлено 2014.02.04 ----------------------------------------------------------------------------------------------

// Поиск параметров запуска процесса
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
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// Поиск процесса в памяти
bool __fastcall TFormMain::ProcessFind(AnsiString ExeName, AnsiString ExePath)
{
	CurrentFunction = "TFormMain::ProcessFind";
	//MemoStatus->Lines->Add("\""+ExeName+"\"");
   HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
   PROCESSENTRY32 pe;
   // DWORD* Pid;
   // Получить FileName из Appname
   AnsiString FileName = "";
   if (ExeName.Pos(" ") != 0) {
      FileName = ExeName.SubString(0,ExeName.Pos(" ")-1);
   } else  {
      FileName = ExeName;
   }
   // Получить параметры ExeParams из Appname
   AnsiString ExeParams = "";
   if ((FileName.Length() == ExeName.Length()) || (FileName.Length() == ExeName.Length()-1)) {
      ExeParams = "";
   } else  {
      ExeParams	= ExeName.SubString(ExeName.Pos(" "),ExeName.Length());
   }
   // Проверка доступа к снимку процессов
   if (hSnapshot == (void*)-1)  {
		MessageBox(NULL, "Ошибка доступа к снимку процессов!", "Ошибка", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
      return false;
   }
   //  Проверка доступа к NTDLL
	NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(GetModuleHandle("NTDLL"), "NtQueryInformationProcess");
   if (!NtQueryInformationProcess)  {
      MessageDlg("[!] Ошибка доступа к NTDLL", mtError, TMsgDlgButtons() << mbOK, 0);
      return false;
   }
   WCHAR wstr[255];
   // Поиск в первом процессе
   Process32First(hSnapshot, &pe);                                     // поиск в первом процессе
   pe.dwSize = sizeof(PROCESSENTRY32);
   if ((AnsiString(pe.szExeFile).UpperCase()).Pos((FileName).UpperCase()))  { // если найден процесс
      ZeroMemory(&wstr, sizeof(wstr));                              // чистим wstr
      if (ProcessGetParams(pe.th32ProcessID,wstr,sizeof(wstr)))  {  // получить его параметры
         if (ExeParams != "")  {                                    // если имеются ExeParams
            if ((AnsiString(wstr).UpperCase()).Pos(ExeParams.UpperCase()))  {   // параметры сошлись
               CloseHandle (hSnapshot);
               return true;
            }
         } else {                                                   // ищем без ExeParams
            /*
            if (ExePath == "") {                                    // Если ExePath нет - то ищем без кавычек
               if (AnsiString(wstr).UpperCase() == (ExePath+FileName).UpperCase())  {   // если параметры сошлись
                  CloseHandle (hSnapshot);
                  return true;
               }
            } else  {                                               // Если ExePath есть - то ищем с кавычками
               if (AnsiString(wstr).UpperCase() == "\""+(ExePath+FileName).UpperCase()+"\"")  {   // если параметры сошлись
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
   // Поиск в остальных процессах
   while (Process32Next(hSnapshot,&pe))  {                             // поиск в остальных процессах
      pe.dwSize = sizeof(PROCESSENTRY32);
      if ((AnsiString(pe.szExeFile).UpperCase()).Pos((FileName).UpperCase()))  { // если найден процесс
         ZeroMemory(&wstr, sizeof(wstr));                              // чистим wstr
         if (ProcessGetParams(pe.th32ProcessID,wstr,sizeof(wstr)))  {  // получить его параметры
            if (ExeParams != "")  {                                    // если имеются ExeParams
               if ((AnsiString(wstr).UpperCase()).Pos(ExeParams.UpperCase()))  {   // параметры сошлись
                  CloseHandle (hSnapshot);
                  return true;
               }
            } else {                                                   // ищем без ExeParams
               // if (AnsiString(wstr).UpperCase() == (ExePath+FileName).UpperCase())  {   // если параметры сошлись
               // if (AnsiString(wstr).UpperCase() == "\""+(ExePath+FileName).UpperCase()+"\"")  {   // если параметры сошлись
               // Получить параметры ExeParams из Appname
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
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// Конструктор
__fastcall TFormMain::TFormMain(TComponent* Owner) : TForm(Owner)
{
	// Инициализация 2014.02.27
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
	// Настройки по-умолчанию
	Caption                          = Application->Title+" [релиз " + GetVers3dig+"]";
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
	// Расширение меню
	MenuTitle->Caption = Application->Title+" - Справка";
	MenuTitle->OnClick = FrameAppMain->MenuHelpClick;
	PopupMenuMain->Items->Add(NewLine());
	PopupMenuMain->Items->Add(NewItem("Перезапуск",   0, False, True, FrameAppMain->MenuRestartClick,  0, "MenuRestart"));
	PopupMenuMain->Items->Add(NewItem("Выход",        0, False, True, MenuAppExitClick,                0, "MenuExit"));
	// Инициализация INI
	fSettings = new TIniFile(ChangeFileExt(Application->ExeName,".ini"));
} // Обновлено 2014.02.04 ----------------------------------------------------------------------------------------------

// Создание главной формы
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
	CurrentFunction = "TFormMain::FormCreate";
	AppMain->FileOutWrite(0,0,"Запуск программы");
	// INIMain->FileNameWrite(ChangeFileExt(Application->ExeName,".ini"));
	// INIMain->Create();
	// Загрузка INI-файла
	INILoad(Sender);
} // Обновлено 2014.02.04 ----------------------------------------------------------------------------------------------

// Запуск Мониторинга
void __fastcall TFormMain::ButtonStartMonitorClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::ButtonStartMonitorClick";
	// Интерфейс
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
	// Старт TimerMonitor
	Caption = Application->Title+" [релиз " + GetVers3dig+"]"+" [Старт: " + DateToStr(Now())+" "+TimeToStr(Now())+"]";
	MemoStatus->Clear();
	MemoStatus->Lines->Add(TimeToStr(Now())+" [ ] Старт");
	AppMain->FileOutWrite(0,0,"Старт");
	TimerMonitor->Enabled              = true;
	TimerMonitorTimer(Sender);
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// Останов Мониторинга
void __fastcall TFormMain::ButtonStopMonitorClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::ButtonStopMonitorClick";
	// Интерфейс
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
	// Останов TimerMonitor
	Caption                            = Application->Title+" [релиз " + GetVers3dig+"]";
	TimerMonitor->Enabled              = false;
	MemoStatus->Lines->Add(TimeToStr(Now())+" [ ] Стоп");
	AppMain->FileOutWrite(0,0,"Стоп");
	TimeLast = Now()-1.0/60/24;
	LogWriteOptimise = false;
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// Показать форму
void __fastcall TFormMain::MenuAppShowClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuAppShowClick";
	::ShowWindow(Application->Handle,SW_SHOW);
	::ShowWindow(FormMain->Handle,SW_SHOW);
	MenuAppHide->Enabled = true;
	MenuAppShow->Enabled = false;
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// Скрыть форму
void __fastcall TFormMain::MenuAppHideClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuAppHideClick";
	::ShowWindow(Application->Handle,SW_HIDE);
	::ShowWindow(FormMain->Handle,SW_HIDE);
	MenuAppHide->Enabled = false;
	MenuAppShow->Enabled = true;
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// Выход из программы
void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	CurrentFunction = "TFormMain::FormClose";
	// Сохранение настроек скинов в ini
	INISave(Sender);
	// Сброс регистрации глобальных клавиш
	MenuHotKeyUnRegClick(Sender);
	// Логи
	AppMain->FileOutWrite(0,0,"Выход из программы");
	TimerMonitor->Enabled = false;
	// Лог завершения работы и очистка памяти
	delete fSettings;
	AppCloseAll();
} // Обновлено 2014.02.02 ----------------------------------------------------------------------------------------------

// Выход из программы
void __fastcall TFormMain::MenuAppExitClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuAppExitClick";
	INIUnload(Sender);
	Close();
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// Монитор процессов
void __fastcall TFormMain::TimerMonitorTimer(TObject *Sender)
{
	CurrentFunction = "TFormMain::TimerMonitorTimer";
	// Обновление имен лог-файлов
	AppMain->FilesOutCheck();
	// Читаем данные из INI-файла и проверяем процессы
   MemoStatus->Clear();
   MemoStatus->Lines->Add(TimeToStr(Now())+" [ ] Старт");
	for (register unsigned short i=0; i<fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0); i++) {
		AppName   = fSettings->ReadString(ComboBoxSettings->Text, "AppName"+IntToStr(i+1),     "");
		AppFolder = fSettings->ReadString(ComboBoxSettings->Text, "AppFolder"+IntToStr(i+1), "");
      // Поиск запущенного процесса в памяти с параметрами коммандной строки
      if (ProcessFind(AppName.UpperCase(),AppFolder.UpperCase()))  {
         MemoStatus->Lines->Add(TimeToStr(Now())+" [ ] Процесс \""+AppName+"\" запущен");
      } else  {
      try {
			if ((double)(TimeOutMax-(Now()-TimeLast)) <= 0)  {   // Разрешить запуск если прошло больше TimeMax
				MemoStatus->Lines->Add(TimeToStr(Now())+" [S] Запуск процесса \""+AppName+"\"");
				MemoReport->Lines->Add(FormatDateTime("hh:nn:ss",Now())+" Запуск процесса \""+AppName+"\"");
				AppMain->FileOutWrite(0,0,"Запуск процесса \""+AppName+"\"");  // лог
				AppMain->FileOutWrite(1,0,"Запуск процесса \""+AppName+"\"");  // отчёт
				STARTUPINFO           si;
            PROCESS_INFORMATION   pi;
            ZeroMemory(&si, sizeof(si));                              // чистим si
            ZeroMemory(&pi, sizeof(pi));                              // чистим pi
				memset(&si, 0, sizeof(si));                               // выделяем память si
            memset(&pi, 0, sizeof(pi));                               // выделяем память pi
            si.cb = sizeof(si);
            if (AppFolder == "")
					CreateProcess(NULL, AppName.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
				else
					CreateProcess(NULL, (AppFolder+AppName).c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, AppFolder.c_str(), &si, &pi);
            TimeLast = Now();
            LogWriteOptimise = false;
            //Sleep(5000);
         } else {
				MemoStatus->Lines->Add(TimeToStr(Now())+" [!] Прошло недостаточно времени "+FormatDateTime("hh:mm:ss", TimeOutMax-(Now()-TimeLast)));
				MemoStatus->Lines->Add(TimeToStr(Now())+" [ ] Тайм-аут ожидания: "+FormatDateTime("hh:mm:ss", TimeOutMax));
            if (LogWriteOptimise == false) {
					AppMain->FileOutWrite(0,0,"Прошло недостаточно времени");
					AppMain->FileOutWrite(0,0,"Тайм-аут ожидания: "+FormatDateTime("hh:mm:ss", TimeOutMax));
               LogWriteOptimise = true;
            }
         }
      } catch (Exception &e)  {
         MemoStatus->Lines->Add(TimeToStr(Now())+" [!] Ошибка запуска "+AppName+"!\n"+"[!] "+e.Message);
			AppMain->FileOutWrite(0,2,"Ошибка запуска "+AppName+"!\n"+"[!] "+e.Message);
		}
		catch (...)  {
			try  {
				throw Exception("");
			} catch (Exception &e)  {
				MemoStatus->Lines->Add(TimeToStr(Now())+" [!] Ошибка запуска "+AppName+"!\n"+"[!] "+e.Message);
				AppMain->FileOutWrite(0,0,"Ошибка запуска "+AppName+"!\n"+"[!] "+e.Message);
			}
      }
   }
   }
} // Обновлено 2014.02.04 ----------------------------------------------------------------------------------------------

// Корректное создание и закрытие окна "О программе"
void __fastcall TFormMain::ButtonAboutClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::ButtonAboutClick";
	Application->CreateForm(__classid(TFormHelp), &FormHelp);
	FormHelp->ShowModal();
	FormHelp->Free();
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// INI - Создать настройки
void __fastcall TFormMain::ButtonINICreateClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::ButtonINICreateClick";
	INICreate(Sender, ComboBoxSettings->Text);
} // Обновлено 2014.02.03 ----------------------------------------------------------------------------------------------

// INI - Создать настройки
void __fastcall TFormMain::INICreate(TObject *Sender, AnsiString SettingsName)
{
	CurrentFunction = "TFormMain::INICreate";
	try {
		if (SettingsName=="") {
			MemoSettings->Lines->Add("Не задано имя конфигурации! Создаю по-умолчанию.");
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
		// Интерфейс
		ButtonINIDelete->Enabled = true;
		ButtonINILoad->Enabled = true;
		MemoSettings->Lines->Add("Конфигурация успешно создана.");
		// Добавить тестовую запись
		EditAppName->Text = "calc";
		INIRecAdd(Sender);
		MemoSettings->Lines->Add("Добавлена тестовая запись.");
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"Ошибка создания конфигурации");
		MsgBoxWithTimeOut(NULL,"Ошибка создания конфигурации!", MB_OK+MB_ICONERROR,2500);
	}
} // Обновлено 2014.02.03 ----------------------------------------------------------------------------------------------

// INI - Удалить файл настроек
void __fastcall TFormMain::INIDelete(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIDelete";
	try {
		if (MessageDlg("Вы уверенны, что хотит удалить файл настроек?", mtConfirmation, TMsgDlgButtons() << mbNo << mbYes , 0) == mrYes)  {
			INIUnload(Sender);
			DeleteFile(ChangeFileExt(Application->ExeName,".ini"));
			ButtonINICreate->Enabled         = true;
			ButtonINIDelete->Enabled         = false;
			CheckBoxHotKeysRegister->Checked = false;
			FrameAppMain->CheckBoxSkinUse->Checked = false;
			MemoSettings->Lines->Add("Файл настроек обнаружен и удален.");
		}
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"Ошибка удаления файла настроек");
		MsgBoxWithTimeOut(NULL,"Ошибка удаления файла настроек!",MB_OK+MB_ICONERROR,2500);
	}
} // Обновлено 2014.02.04 ----------------------------------------------------------------------------------------------

// INI - Чтение настроек
void __fastcall TFormMain::INILoad(TObject *Sender)
{
	CurrentFunction = "TFormMain::INILoad";
	try {
		INIUnload(Sender);
		// Подготовка интерфейса перед загрузкой конфигураций
		ComboBoxSettings->Items->Clear();
		StringGridSettings->RowCount = 1;
		StringGridSettings->Rows[0]->Text = "";
		// Почистить пустые конфигурации
		if (fSettings->SectionExists(NULL))
			fSettings->EraseSection(NULL);
		// Загрузка конфигов из INI-файла
		fSettings->ReadSections(ComboBoxSettings->Items);
		if (ComboBoxSettings->Items->Count==0)  {  // Если нет конфигов
			ButtonINICreate->Enabled = true;
			MemoSettings->Lines->Add("Настроек не обнаружено!");
		} else {                                   // Прочитать конфиг
			for (register unsigned short i=0; i<ComboBoxSettings->Items->Count; i++) {
				ComboBoxSettings->ItemIndex = i;
				if (ComboBoxSettings->Text=="")  {
					if (MessageDlg("Настройки не найдены!\nСоздать конфигурацию по-умолчанию?", mtConfirmation, TMsgDlgButtons() << mbNo << mbYes , 0) == mrYes)  {
						ComboBoxSettings->Text = AppMain->CompName;
						// создать настройки по-умолчанию
						INICreate(Sender,AppMain->CompName);
					} else  {
						MsgBoxWithTimeOut(NULL,"Конфигурации по-умолчанию не обнаружено!",MB_OK+MB_ICONERROR,2500);
					}
				}
			}
			// Интерфейс
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
			// Прочитать и вывести список всех приложений
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
			// Проверка AutoStart
			if (CheckBoxAutoStart->Checked)  ButtonStartMonitorClick(Sender);
			// Лог
			MemoSettings->Lines->Add("Настройки успешно загруженны.");
		}
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"Ошибка загрузки настроек");
		MsgBoxWithTimeOut(NULL, "Ошибка загрузки настроек!",MB_OK+MB_ICONERROR,2500);
	}
} // Обновлено 2014.02.04 ----------------------------------------------------------------------------------------------

// INI - Выгрузка настроек
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
		MemoSettings->Lines->Add("Настройки успешно выгруженны.");
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"Ошибка выгрузки настроек");
		MsgBoxWithTimeOut(NULL,"Ошибка выгрузки настроек!",MB_OK+MB_ICONERROR,2500);
	}
} // Обновлено 2014.02.04 ----------------------------------------------------------------------------------------------

// INI - Сохранить
void __fastcall TFormMain::INISave(TObject *Sender)
{
	CurrentFunction = "TFormMain::INISave";
	try {
		if (ComboBoxSettings->Text=="") {
			MsgBoxWithTimeOut(NULL,"Не задано имя кофигурации!",MB_OK+MB_ICONINFORMATION,2500);
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
			// Лог
			MsgBoxWithTimeOut(NULL,"Настройки конфигурации успешно сохранены.",MB_OK+MB_ICONINFORMATION,2500);
		}
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"Ошибка сохранения настроек");
		MsgBoxWithTimeOut(NULL,"Ошибка сохранения настроек!",MB_OK+MB_ICONERROR,2500);
	}
} // Обновлено 2014.02.03 ----------------------------------------------------------------------------------------------

// INI - Очистить конфигурацию
void __fastcall TFormMain::INIClear(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIClear";
	try {
		if (MessageDlg("Вы уверенны, что хотит очистить конфигурацию?", mtConfirmation, TMsgDlgButtons() << mbNo << mbYes , 0) == mrYes)  {
				fSettings->EraseSection(ComboBoxSettings->Text);
				MemoSettings->Lines->Add("Конфигурация очищена."+ComboBoxSettings->Text);
				ButtonINIClear->Enabled = false;
		}
		INILoad(Sender);
	} catch (...)  {
		AppMain->FileOutWrite(0,2,"Ошибка выгрузки настроек");
		MsgBoxWithTimeOut(NULL,"Ошибка выгрузки настроек!",MB_OK+MB_ICONERROR,2500);
	}
} // Обновлено 2014.02.04 ----------------------------------------------------------------------------------------------

// INI - Добавить запись
void __fastcall TFormMain::INIRecAdd(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIRecAdd";
	if ((ComboBoxSettings->Text != "") && (EditAppName->Text != "")) {
		fSettings->WriteString (ComboBoxSettings->Text, "AppName"+IntToStr(fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)+1), EditAppName->Text);
		fSettings->WriteString (ComboBoxSettings->Text, "AppFolder"+IntToStr(fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)+1), EditAppFolder->Text);
		fSettings->WriteInteger(ComboBoxSettings->Text, "Count", fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)+1);
		INILoad(Sender);
	} else {
		MemoSettings->Lines->Add("Заполните следующие поля:");
		if (ComboBoxSettings->Text == "")
			MemoSettings->Lines->Add("    - имя конфигурации");
		if (EditAppName->Text == "")
			MemoSettings->Lines->Add("    - имя процесса, например: iexplore.exe");
		if (EditAppFolder->Text == "")
			MemoSettings->Lines->Add("    - имя папки,    например: c:\\Program Files\\Internet Explorer\\");
	}
} // Обновлено 2014.02.03 ----------------------------------------------------------------------------------------------

// INI - Удалить запись
void __fastcall TFormMain::INIRecDel(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIRecDel";
	if (StringGridSettings->Cells[0][StringGridSettings->Row] != "") {
		// Удалить выделенный ключ
		fSettings->DeleteKey(ComboBoxSettings->Text, "AppName"+IntToStr(StringGridSettings->Row+1));
		fSettings->DeleteKey(ComboBoxSettings->Text, "AppFolder"+IntToStr(StringGridSettings->Row+1));
		fSettings->WriteInteger(ComboBoxSettings->Text, "Count", fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)-1);
      // Переместить ключи с конца
      for (int i = StringGridSettings->Row+1; i <= fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0); i++) {
         fSettings->WriteString(ComboBoxSettings->Text, "AppName"+IntToStr(i),
         fSettings->ReadString(ComboBoxSettings->Text, "AppName"+IntToStr(i+1), ""));
         fSettings->WriteString(ComboBoxSettings->Text, "AppFolder"+IntToStr(i),
         fSettings->ReadString(ComboBoxSettings->Text, "AppFolder"+IntToStr(i+1), ""));
      }
      // Удалить последний ключ
      fSettings->DeleteKey(ComboBoxSettings->Text, "AppName"+IntToStr(fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)+1));
      fSettings->DeleteKey(ComboBoxSettings->Text, "AppFolder"+IntToStr(fSettings->ReadInteger(ComboBoxSettings->Text, "Count", 0)+1));
      MemoSettings->Lines->Add("Ключ успешно удалён!");
		INILoad(Sender);
   } else {
      MemoSettings->Lines->Add("Выберите ключ!");
   }
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// INI - Изменить запись
void __fastcall TFormMain::INIRecSave(TObject *Sender)
{
	CurrentFunction = "TFormMain::INIRecSave";
   if (StringGridSettings->Cells[0][StringGridSettings->Row] != "") {
      // Изменить ключ
      fSettings->WriteString(ComboBoxSettings->Text, "AppName"     +IntToStr(StringGridSettings->Row+1), EditAppName->Text);
      fSettings->WriteString(ComboBoxSettings->Text, "AppFolder" +IntToStr(StringGridSettings->Row+1), EditAppFolder->Text);
      MemoSettings->Lines->Add("Ключ успешно изменён!");
      INILoad(Sender);
   } else {
      MemoSettings->Lines->Add("Выберите ключ!");
   }
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// Раскрытие ключа при щелчке мышкой
void __fastcall TFormMain::StringGridSettingsClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::StringGridSettingsClick";
	EditAppName->Text   = fSettings->ReadString(ComboBoxSettings->Text, "AppName"+IntToStr(StringGridSettings->Row+1),   "");
	EditAppFolder->Text = fSettings->ReadString(ComboBoxSettings->Text, "AppFolder"+IntToStr(StringGridSettings->Row+1), "");
} // Обновлено 2012.10.12 ----------------------------------------------------------------------------------------------

// Проекты Dam33
void __fastcall TFormMain::LabelLinkClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::LabelLinkClick";
	ShellExecute(Application->Handle,"open","http://forum-pro-it.ru/",NULL,NULL,SW_SHOWNORMAL);
} // Обновлено 2014.01.26 ----------------------------------------------------------------------------------------------

void __fastcall TFormMain::LabelLinkMouseEnter(TObject *Sender)
{
	CurrentFunction = "TFormMain::LabelLinkMouseEnter";
	LabelLink->Font->Color = clBlue;
} // Обновлено 2014.01.27 ----------------------------------------------------------------------------------------------

void __fastcall TFormMain::LabelLinkMouseLeave(TObject *Sender)
{
	CurrentFunction = "TFormMain::LabelLinkMouseLeave";
	LabelLink->Font->Color = clBlack;
} // Обновлено 2014.01.27 ----------------------------------------------------------------------------------------------// Включить/выключить тень

// Регистрация горячих клавиш
void __fastcall TFormMain::CheckBoxHotKeysRegisterClick(TObject *Sender)
{
	if (CheckBoxHotKeysRegister->Checked) {
		MenuHotKeyRegClick(Sender);
	} else {
		MenuHotKeyUnRegClick(Sender);
	}
} // Обновлено 2014.02.02 ----------------------------------------------------------------------------------------------

// Изменение тайм-аута ожидания
void __fastcall TFormMain::EditTimeOutHChange(TObject *Sender)
{
	CurrentFunction = "TFormMain::EditTimeOutHChange";
	if (EditTimeOutH->Text=="") { EditTimeOutH->Text=0; }
	if (EditTimeOutM->Text=="") { EditTimeOutM->Text=0; }
	if (EditTimeOutS->Text=="") { EditTimeOutS->Text=0; }
	if (StrToInt(EditTimeOutH->Text)<0 || StrToInt(EditTimeOutH->Text)>23 ||
		 StrToInt(EditTimeOutM->Text)<0 || StrToInt(EditTimeOutM->Text)>59 ||
		 StrToInt(EditTimeOutS->Text)<0 || StrToInt(EditTimeOutS->Text)>59) {
		MsgBoxWithTimeOut(NULL,"Значение тайм-аута ожидания не верно!\nПроверьте значение!",MB_OK+MB_ICONINFORMATION,2500);
		return;
	}
	TimeOutMax = EncodeTime(StrToInt(EditTimeOutH->Text), StrToInt(EditTimeOutM->Text), StrToInt(EditTimeOutS->Text), 0);
} // Обновлено 2014.02.02 ----------------------------------------------------------------------------------------------

// Очистить лог настроек
void __fastcall TFormMain::MenuogClearClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuogClearClick";
	MemoSettings->Clear();
} // Обновлено 2014.02.02 ----------------------------------------------------------------------------------------------

// Задать имя компьютера
void __fastcall TFormMain::MenuSettingsNameInsertClick(TObject *Sender)
{
	CurrentFunction = "TFormMain::MenuogClearClick";
	ComboBoxSettings->Text = AppMain->CompName;
} // Обновлено 2014.02.02 ----------------------------------------------------------------------------------------------


