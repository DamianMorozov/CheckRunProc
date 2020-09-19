object FormMain: TFormMain
  Left = 233
  Top = 114
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'FormMain'
  ClientHeight = 532
  ClientWidth = 629
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Courier New'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenuMain
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  DesignSize = (
    629
    532)
  PixelsPerInch = 96
  TextHeight = 14
  object Label2: TLabel
    Left = 259
    Top = 15
    Width = 112
    Height = 14
    Caption = #1048#1084#1103' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080
    Transparent = True
  end
  object ButtonAbout: TButton
    Left = 591
    Top = 5
    Width = 25
    Height = 25
    Hint = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
    Caption = '?'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 3
    OnClick = ButtonAboutClick
  end
  object ButtonStartMonitor: TButton
    Left = 5
    Top = 5
    Width = 100
    Height = 25
    Cursor = crHandPoint
    Caption = #1057#1090#1072#1088#1090
    Enabled = False
    TabOrder = 0
    OnClick = ButtonStartMonitorClick
  end
  object ButtonStopMonitor: TButton
    Left = 120
    Top = 5
    Width = 100
    Height = 25
    Cursor = crHandPoint
    Caption = #1057#1090#1086#1087
    Enabled = False
    TabOrder = 1
    OnClick = ButtonStopMonitorClick
  end
  object ComboBoxSettings: TComboBox
    Left = 377
    Top = 8
    Width = 200
    Height = 22
    PopupMenu = PopupMenuSettingsName
    TabOrder = 2
    OnClick = INILoad
  end
  object PageControlMain: TPageControl
    Left = 5
    Top = 35
    Width = 620
    Height = 495
    ActivePage = TabSheetStatus
    Anchors = [akLeft, akTop, akRight, akBottom]
    BiDiMode = bdLeftToRight
    ParentBiDiMode = False
    PopupMenu = PopupMenuMain
    TabOrder = 4
    object TabSheetStatus: TTabSheet
      Caption = #1057#1086#1089#1090#1086#1103#1085#1080#1077
      DesignSize = (
        612
        466)
      object MemoStatus: TMemo
        Left = 5
        Top = 5
        Width = 601
        Height = 457
        Anchors = [akLeft, akTop, akRight, akBottom]
        ReadOnly = True
        ScrollBars = ssVertical
        TabOrder = 0
      end
    end
    object TabSheetReport: TTabSheet
      Caption = #1054#1090#1095#1105#1090
      ImageIndex = 2
      DesignSize = (
        612
        466)
      object MemoReport: TMemo
        Left = 5
        Top = 5
        Width = 601
        Height = 457
        Anchors = [akLeft, akTop, akRight, akBottom]
        ReadOnly = True
        ScrollBars = ssVertical
        TabOrder = 0
      end
    end
    object TabSheetSettings: TTabSheet
      Caption = #1050#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1103
      ImageIndex = 3
      DesignSize = (
        612
        466)
      object LabelTitle1: TLabel
        Left = 5
        Top = 101
        Width = 56
        Height = 14
        Caption = #1044#1077#1081#1089#1090#1074#1080#1077
      end
      object LabelTitle2: TLabel
        Left = 165
        Top = 101
        Width = 110
        Height = 14
        AutoSize = False
        Caption = #1043#1086#1088#1103#1095#1080#1077' '#1082#1083#1072#1074#1080#1096#1099
      end
      object LabelTitle3: TLabel
        Left = 295
        Top = 101
        Width = 80
        Height = 14
        AutoSize = False
        Caption = #1056#1077#1075#1080#1089#1090#1088#1072#1094#1080#1103
      end
      object Label4: TLabel
        Left = 5
        Top = 121
        Width = 112
        Height = 14
        Caption = #1057#1082#1088#1099#1090#1100' '#1087#1088#1086#1075#1088#1072#1084#1084#1091
      end
      object Label6: TLabel
        Left = 165
        Top = 121
        Width = 110
        Height = 14
        Alignment = taCenter
        AutoSize = False
        Caption = 'Ctrl+Alt+H'
      end
      object LabelHotKey2Active: TLabel
        Left = 295
        Top = 141
        Width = 80
        Height = 13
        Alignment = taCenter
        AutoSize = False
        Caption = '-'
      end
      object Label7: TLabel
        Left = 5
        Top = 141
        Width = 126
        Height = 14
        Caption = #1055#1086#1082#1072#1079#1072#1090#1100' '#1087#1088#1086#1075#1088#1072#1084#1084#1091
      end
      object Label9: TLabel
        Left = 165
        Top = 141
        Width = 110
        Height = 14
        Alignment = taCenter
        AutoSize = False
        Caption = 'Ctrl+Alt+S'
      end
      object LabelHotKey3Active: TLabel
        Left = 295
        Top = 121
        Width = 80
        Height = 13
        Alignment = taCenter
        AutoSize = False
        Caption = '-'
      end
      object LabelLink: TLabel
        Left = 5
        Top = 165
        Width = 343
        Height = 14
        Cursor = crHandPoint
        Caption = #1055#1086#1089#1077#1090#1080#1090#1100' '#1089#1072#1081#1090' '#1088#1072#1079#1088#1072#1073#1086#1090#1095#1080#1082#1072' http://forum-pro-it.ru'
        OnClick = LabelLinkClick
        OnMouseEnter = LabelLinkMouseEnter
        OnMouseLeave = LabelLinkMouseLeave
      end
      object LabelTimeOut: TLabel
        Left = 117
        Top = 52
        Width = 119
        Height = 14
        Hint = #1058#1072#1081#1084'-'#1072#1091#1090' '#1085#1072' '#1087#1086#1074#1090#1086#1088#1085#1099#1081' '#1079#1072#1087#1091#1089#1082' '#1087#1088#1086#1094#1077#1089#1089#1072
        Caption = #1058#1072#1081#1084'-'#1072#1091#1090' '#1086#1078#1080#1076#1072#1085#1080#1103
        ParentShowHint = False
        ShowHint = True
        Transparent = True
      end
      object LabelAppFolder: TLabel
        Left = 505
        Top = 345
        Width = 63
        Height = 14
        Caption = #1048#1084#1103' '#1087#1072#1087#1082#1080
        Transparent = True
      end
      object LabelAppName: TLabel
        Left = 505
        Top = 325
        Width = 84
        Height = 14
        Caption = #1048#1084#1103' '#1087#1088#1086#1094#1077#1089#1089#1072
        Transparent = True
      end
      object LabelTimeOutH: TLabel
        Left = 247
        Top = 67
        Width = 7
        Height = 14
        Hint = #1058#1072#1081#1084'-'#1072#1091#1090' '#1085#1072' '#1087#1086#1074#1090#1086#1088#1085#1099#1081' '#1079#1072#1087#1091#1089#1082' '#1087#1088#1086#1094#1077#1089#1089#1072
        Caption = #1095
        ParentShowHint = False
        ShowHint = True
        Transparent = True
      end
      object LabelTimeOutM: TLabel
        Left = 268
        Top = 67
        Width = 7
        Height = 14
        Hint = #1058#1072#1081#1084'-'#1072#1091#1090' '#1085#1072' '#1087#1086#1074#1090#1086#1088#1085#1099#1081' '#1079#1072#1087#1091#1089#1082' '#1087#1088#1086#1094#1077#1089#1089#1072
        Caption = #1084
        ParentShowHint = False
        ShowHint = True
        Transparent = True
      end
      object LabelTimeOutS: TLabel
        Left = 287
        Top = 67
        Width = 7
        Height = 14
        Hint = #1058#1072#1081#1084'-'#1072#1091#1090' '#1085#1072' '#1087#1086#1074#1090#1086#1088#1085#1099#1081' '#1079#1072#1087#1091#1089#1082' '#1087#1088#1086#1094#1077#1089#1089#1072
        Caption = #1089
        ParentShowHint = False
        ShowHint = True
        Transparent = True
      end
      object ButtonSettingsSave: TButton
        Left = 5
        Top = 185
        Width = 600
        Height = 25
        Anchors = [akLeft, akTop, akRight]
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
        TabOrder = 10
        OnClick = INISave
      end
      object CheckBoxHotKeysRegister: TCheckBox
        Left = 5
        Top = 78
        Width = 214
        Height = 17
        Caption = #1056#1077#1075#1080#1089#1090#1088#1072#1094#1080#1103' '#1075#1086#1088#1103#1095#1080#1093' '#1082#1083#1072#1074#1080#1096
        TabOrder = 9
        OnClick = CheckBoxHotKeysRegisterClick
      end
      object CheckBoxAutoStart: TCheckBox
        Left = 5
        Top = 50
        Width = 97
        Height = 17
        Hint = #1040#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1080#1081' '#1079#1072#1087#1091#1089#1082' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' '#1087#1088#1080' '#1079#1072#1075#1088#1091#1079#1082#1077' '#1087#1088#1086#1075#1088#1072#1084#1084#1099
        Caption = #1040#1074#1090#1086#1057#1090#1072#1088#1090
        ParentShowHint = False
        ShowHint = True
        TabOrder = 5
      end
      object EditTimeOutH: TEdit
        Left = 240
        Top = 47
        Width = 21
        Height = 22
        Hint = #1095#1072#1089#1099
        Alignment = taCenter
        NumbersOnly = True
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        Text = '0'
        OnChange = EditTimeOutHChange
      end
      object EditTimeOutM: TEdit
        Left = 260
        Top = 47
        Width = 21
        Height = 22
        Hint = #1084#1080#1085#1091#1090#1099
        Alignment = taCenter
        NumbersOnly = True
        ParentShowHint = False
        ShowHint = True
        TabOrder = 7
        Text = '0'
        OnChange = EditTimeOutHChange
      end
      object EditTimeOutS: TEdit
        Left = 280
        Top = 47
        Width = 21
        Height = 22
        Hint = #1089#1077#1082#1091#1085#1076#1099
        Alignment = taCenter
        NumbersOnly = True
        ParentShowHint = False
        ShowHint = True
        TabOrder = 8
        Text = '0'
        OnChange = EditTimeOutHChange
      end
      object ButtonINICreate: TButton
        Left = 5
        Top = 10
        Width = 100
        Height = 25
        Hint = #1055#1086' '#1080#1084#1077#1085#1080' '#1082#1086#1084#1087#1100#1102#1090#1077#1088#1072
        Caption = #1057#1086#1079#1076#1072#1090#1100' '#1085#1086#1074#1091#1102
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnClick = ButtonINICreateClick
      end
      object ButtonINIDelete: TButton
        Left = 125
        Top = 10
        Width = 100
        Height = 25
        Caption = #1059#1076#1072#1083#1080#1090#1100' '#1074#1089#1077
        TabOrder = 1
        OnClick = INIDelete
      end
      object ButtonINILoad: TButton
        Left = 250
        Top = 10
        Width = 100
        Height = 25
        Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100
        TabOrder = 2
        OnClick = INILoad
      end
      object ButtonINIUnload: TButton
        Left = 375
        Top = 10
        Width = 100
        Height = 25
        Caption = #1042#1099#1075#1088#1091#1079#1080#1090#1100
        Enabled = False
        TabOrder = 3
        OnClick = INIUnload
      end
      object ButtonINIClear: TButton
        Left = 505
        Top = 10
        Width = 100
        Height = 25
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100
        Enabled = False
        TabOrder = 4
        OnClick = INIClear
      end
      object StringGridSettings: TStringGrid
        Left = 5
        Top = 215
        Width = 600
        Height = 100
        Anchors = [akLeft, akTop, akRight]
        ColCount = 1
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        ScrollBars = ssVertical
        TabOrder = 11
        OnClick = StringGridSettingsClick
      end
      object EditAppName: TEdit
        Left = 5
        Top = 320
        Width = 490
        Height = 22
        Hint = #1048#1084#1103' '#1087#1088#1086#1094#1077#1089#1089#1072' '#1089' '#1087#1072#1088#1072#1084#1077#1090#1088#1072#1084#1080
        Enabled = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 12
      end
      object EditAppFolder: TEdit
        Left = 5
        Top = 340
        Width = 490
        Height = 22
        Hint = #1048#1084#1103' '#1087#1072#1087#1082#1080
        Enabled = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 13
      end
      object ButtonINIRecAdd: TButton
        Left = 5
        Top = 365
        Width = 120
        Height = 25
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1079#1072#1087#1080#1089#1100
        Enabled = False
        TabOrder = 14
        OnClick = INIRecAdd
      end
      object ButtonINIRecDel: TButton
        Left = 185
        Top = 365
        Width = 120
        Height = 25
        Caption = #1059#1076#1072#1083#1080#1090#1100' '#1079#1072#1087#1080#1089#1100
        Enabled = False
        TabOrder = 15
        OnClick = INIRecDel
      end
      object ButtonINIRecSave: TButton
        Left = 350
        Top = 365
        Width = 120
        Height = 25
        Caption = #1048#1079#1084#1077#1085#1080#1090#1100' '#1079#1072#1087#1080#1089#1100
        Enabled = False
        TabOrder = 16
        OnClick = INIRecSave
      end
      object MemoSettings: TMemo
        Left = 5
        Top = 395
        Width = 600
        Height = 67
        Anchors = [akLeft, akTop, akRight, akBottom]
        PopupMenu = PopupMenuSettings
        ReadOnly = True
        ScrollBars = ssVertical
        TabOrder = 17
      end
    end
  end
  object TrayIconMain: TTrayIcon
    Hint = #1057#1090#1072#1088#1090#1077#1088' '#1087#1088#1086#1094#1077#1089#1089#1086#1074
    PopupMenu = PopupMenuMain
    Visible = True
    OnClick = MenuAppShowClick
    Left = 264
    Top = 32
  end
  object PopupMenuMain: TPopupMenu
    Left = 352
    Top = 32
    object MenuTitle: TMenuItem
      Caption = 'Title'
    end
    object MenuN1: TMenuItem
      Caption = '-'
    end
    object MenuHotKeyReg: TMenuItem
      Caption = #1056#1077#1075#1080#1089#1090#1088#1072#1094#1080#1103' '#1075#1086#1088#1103#1095#1080#1093' '#1082#1083#1072#1074#1080#1096
      Visible = False
      OnClick = MenuHotKeyRegClick
    end
    object MenuHotKeyUnReg: TMenuItem
      Caption = #1057#1073#1088#1086#1089' '#1075#1086#1088#1103#1095#1080#1093' '#1082#1083#1072#1074#1080#1096
      Visible = False
      OnClick = MenuHotKeyUnRegClick
    end
    object MenuN2: TMenuItem
      Caption = '-'
      Visible = False
    end
    object MenuStartMonitor: TMenuItem
      Caption = #1057#1090#1072#1088#1090
      OnClick = ButtonStartMonitorClick
    end
    object MenuStopMonitor: TMenuItem
      Caption = #1057#1090#1086#1087
      Enabled = False
      OnClick = ButtonStopMonitorClick
    end
    object Menu3: TMenuItem
      Caption = '-'
    end
    object MenuAppShow: TMenuItem
      Caption = #1055#1086#1082#1072#1079#1072#1090#1100
      Enabled = False
      OnClick = MenuAppShowClick
    end
    object MenuAppHide: TMenuItem
      Caption = #1057#1082#1088#1099#1090#1100
      OnClick = MenuAppHideClick
    end
  end
  object TimerMonitor: TTimer
    Enabled = False
    OnTimer = TimerMonitorTimer
    Left = 192
    Top = 32
  end
  object PopupMenuSettings: TPopupMenu
    Left = 448
    Top = 32
    object MenuogClear: TMenuItem
      Caption = #1054#1095#1080#1089#1090#1080#1090#1100
      OnClick = MenuogClearClick
    end
  end
  object PopupMenuSettingsName: TPopupMenu
    Left = 560
    Top = 32
    object MenuSettingsNameInsert: TMenuItem
      Caption = #1047#1072#1076#1072#1090#1100' '#1080#1084#1103' '#1082#1086#1084#1087#1100#1102#1090#1077#1088#1072
      OnClick = MenuSettingsNameInsertClick
    end
  end
  object sSkinProvider1: TsSkinProvider
    AddedTitle.Font.Charset = DEFAULT_CHARSET
    AddedTitle.Font.Color = clNone
    AddedTitle.Font.Height = -11
    AddedTitle.Font.Name = 'Tahoma'
    AddedTitle.Font.Style = []
    SkinData.SkinSection = 'FORM'
    TitleButtons = <>
    Left = 32
    Top = 32
  end
end
