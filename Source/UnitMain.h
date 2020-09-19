//======================================================================================================================
// Описание
// ========
//    Главный модуль. Описание программы см. в файле проекта *.cbproj
//======================================================================================================================
#ifndef UnitMainH
#define UnitMainH
//----------------------------------------------------------------------------------------------------------------------
#include <trayicon.h>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <IniFiles.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
//----------------------------------------------------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:
   TTrayIcon       *TrayIconMain;
	TPopupMenu *PopupMenuMain;
	TMenuItem       *MenuAppShow;
	TMenuItem       *MenuAppHide;
	TButton         *ButtonAbout;
	TTimer          *TimerMonitor;
	TMenuItem *Menu3;
	TMenuItem       *MenuStartMonitor;
	TMenuItem       *MenuStopMonitor;
   TButton         *ButtonStartMonitor;
   TButton         *ButtonStopMonitor;
	TComboBox *ComboBoxSettings;
   TLabel          *Label2;
   TPageControl    *PageControlMain;
	TTabSheet       *TabSheetStatus;
   TMemo           *MemoStatus;
	TTabSheet *TabSheetReport;
	TMemo *MemoReport;
	TMenuItem *MenuN2;
	TMenuItem *MenuHotKeyUnReg;
	TMenuItem *MenuHotKeyReg;
	TTabSheet *TabSheetSettings;
	TLabel *LabelTitle1;
	TLabel *LabelTitle2;
	TLabel *LabelTitle3;
	TLabel *Label4;
	TLabel *Label6;
	TLabel *LabelHotKey2Active;
	TLabel *Label7;
	TLabel *Label9;
	TLabel *LabelHotKey3Active;
	TLabel *LabelLink;
	TButton *ButtonSettingsSave;
	TCheckBox *CheckBoxHotKeysRegister;
	TCheckBox *CheckBoxAutoStart;
	TLabel *LabelTimeOut;
	TEdit *EditTimeOutH;
	TEdit *EditTimeOutM;
	TEdit *EditTimeOutS;
	TButton *ButtonINICreate;
	TButton *ButtonINIDelete;
	TButton *ButtonINILoad;
	TButton *ButtonINIUnload;
	TButton *ButtonINIClear;
	TPopupMenu *PopupMenuSettings;
	TMenuItem *MenuogClear;
	TPopupMenu *PopupMenuSettingsName;
	TMenuItem *MenuSettingsNameInsert;
	TStringGrid *StringGridSettings;
	TEdit *EditAppName;
	TEdit *EditAppFolder;
	TLabel *LabelAppFolder;
	TLabel *LabelAppName;
	TButton *ButtonINIRecAdd;
	TButton *ButtonINIRecDel;
	TButton *ButtonINIRecSave;
	TMemo *MemoSettings;
	TsSkinProvider *sSkinProvider1;
	TMenuItem *MenuN1;
	TMenuItem *MenuTitle;
	TLabel *LabelTimeOutH;
	TLabel *LabelTimeOutM;
	TLabel *LabelTimeOutS;
   bool __fastcall  ProcessGetParams(DWORD dwId,LPWSTR wBuf,DWORD dwBufLen);
   bool __fastcall  ProcessFind(AnsiString ExeName, AnsiString ExePath);
   void __fastcall  FormCreate(TObject *Sender);
   void __fastcall  MenuAppExitClick(TObject *Sender);
	void __fastcall  ButtonStartMonitorClick(TObject *Sender);
	void __fastcall  ButtonStopMonitorClick(TObject *Sender);
	void __fastcall  TimerMonitorTimer(TObject *Sender);
	void __fastcall  FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall  ButtonAboutClick(TObject *Sender);
	void __fastcall  StringGridSettingsClick(TObject *Sender);
	void __fastcall  MenuAppShowClick(TObject *Sender);
	void __fastcall  MenuAppHideClick(TObject *Sender);
	void __fastcall  MenuHotKeyRegClick(TObject *Sender);
	void __fastcall  MenuHotKeyUnRegClick(TObject *Sender);
	void __fastcall  LabelLinkClick(TObject *Sender);
	void __fastcall  LabelLinkMouseEnter(TObject *Sender);
	void __fastcall  LabelLinkMouseLeave(TObject *Sender);
	void __fastcall  CheckBoxHotKeysRegisterClick(TObject *Sender);
	void __fastcall  EditTimeOutHChange(TObject *Sender);
	void __fastcall  MenuogClearClick(TObject *Sender);
	void __fastcall  MenuSettingsNameInsertClick(TObject *Sender);
	void __fastcall  ButtonINICreateClick(TObject *Sender);
	// ---> INI --->
	void __fastcall  INICreate(TObject *Sender, AnsiString SettingsName);
	void __fastcall  INIDelete(TObject *Sender);
	void __fastcall  INILoad(TObject *Sender);
	void __fastcall  INIUnload(TObject *Sender);
	void __fastcall  INISave(TObject *Sender);
	void __fastcall  INIClear(TObject *Sender);
	void __fastcall  INIRecAdd(TObject *Sender);
	void __fastcall  INIRecDel(TObject *Sender);
	void __fastcall  INIRecSave(TObject *Sender);
	// <--- INI <---
private:
	// ---> Windows Messages Map --->
	void __fastcall  OnActivate(TWMActivate& Message);
	void __fastcall  OnClose(TWMClose& Message);
	void __fastcall  OnWindowPosChanging(TMessage &Message);
	void __fastcall  OnHotKey(TWMHotKey &Message);
	#pragma option push -vi-
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_ACTIVATE, TWMActivate, OnActivate)
		MESSAGE_HANDLER(WM_CLOSE,    TWMClose,    OnClose)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, TMessage, OnWindowPosChanging)
		MESSAGE_HANDLER(WM_HOTKEY,   TWMHotKey,   OnHotKey);
	END_MESSAGE_MAP(TComponent)
	#pragma option pop
	// <--- Windows Messages Map <---
public:
		  __fastcall  TFormMain(TComponent* Owner);
};
//======================================================================================================================
//                                          Глобальные функции и переменные
//======================================================================================================================
extern PACKAGE TFormMain *FormMain;
//----------------------------------------------------------------------------------------------------------------------
#endif
