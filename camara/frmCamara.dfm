object Form1: TForm1
  Left = 104
  Top = 85
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Cámara'
  ClientHeight = 545
  ClientWidth = 802
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    000000008000008000000080800080000000800080008080000080808000C0C0
    C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    00000000000000000000000000000000000000000000000000000000000000F8
    888888888888888888888888000000F8888870077888888888888888000000F8
    88F000000788888888888888000000F88F0000000088888888888888000000F8
    8F0000000078888888888888000000F8F00000000008888888888888000000F8
    F00000000008888888888888000000F88F0000000078888888888888000000F8
    8F0000000088888888888888000000F888F0000008888888FFFFF888000000F8
    888FF00FF888888800000F88000000F888888FF888888870000000F8000000F8
    8888888888888870000000F8000000F8888888888888888700000888000000F8
    8888888888888888777778880000000FFFFFFFFFFFFFFFFFFFFFFFFF00000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000000000000000000000000000000000000000000000000000FFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE000000FC000
    0007C0000007C0000007C0000007C0000007C0000007C0000007C0000007C000
    0007C0000007C0000007C0000007C0000007C0000007C0000007E000000FFFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 88
    Height = 13
    Caption = '&Interfaz de cámara'
    FocusControl = ComboBox1
  end
  object Image1: TImage
    Left = 336
    Top = 152
    Width = 320
    Height = 240
  end
  object Label2: TLabel
    Left = 8
    Top = 88
    Width = 44
    Height = 13
    Caption = '&Intervalo:'
  end
  object Label3: TLabel
    Left = 635
    Top = 88
    Width = 6
    Height = 13
    Alignment = taRightJustify
    Caption = '0'
  end
  object Panel1: TPanel
    Left = 8
    Top = 152
    Width = 320
    Height = 240
    TabOrder = 0
  end
  object ComboBox1: TComboBox
    Left = 8
    Top = 24
    Width = 649
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 1
    OnChange = ComboBox1Change
  end
  object TrackBar1: TTrackBar
    Left = 64
    Top = 80
    Width = 553
    Height = 25
    Max = 100
    Orientation = trHorizontal
    Frequency = 1
    Position = 0
    SelEnd = 0
    SelStart = 0
    TabOrder = 2
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = TrackBar1Change
  end
  object Button4: TButton
    Left = 8
    Top = 112
    Width = 649
    Height = 25
    Caption = 'C&apturar/detener'
    TabOrder = 3
    OnClick = Button4Click
  end
  object CheckBox1: TCheckBox
    Left = 672
    Top = 24
    Width = 97
    Height = 17
    Caption = 'Es&pecular'
    TabOrder = 4
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 400
    Width = 649
    Height = 137
    Caption = 'Fotos'
    TabOrder = 5
    object Label4: TLabel
      Left = 8
      Top = 20
      Width = 48
      Height = 13
      Caption = 'Directorio:'
    end
    object Label7: TLabel
      Left = 102
      Top = 112
      Width = 3
      Height = 13
    end
    object Label5: TLabel
      Left = 8
      Top = 112
      Width = 26
      Height = 13
      Caption = 'Ruta:'
    end
    object Label6: TLabel
      Left = 8
      Top = 80
      Width = 54
      Height = 13
      Caption = 'Argumento:'
    end
    object Label8: TLabel
      Left = 8
      Top = 48
      Width = 24
      Height = 13
      Caption = 'Tipo:'
    end
    object Edit1: TEdit
      Left = 104
      Top = 20
      Width = 409
      Height = 21
      TabOrder = 0
      Text = 'Edit1'
      OnChange = Edit1Change
    end
    object Button7: TButton
      Left = 520
      Top = 16
      Width = 121
      Height = 113
      Caption = '&Clic'
      Default = True
      TabOrder = 1
      OnClick = Button7Click
    end
    object Edit2: TEdit
      Left = 104
      Top = 80
      Width = 409
      Height = 21
      TabOrder = 2
      Text = 'Edit2'
    end
    object Edit3: TEdit
      Left = 104
      Top = 48
      Width = 409
      Height = 21
      TabOrder = 3
      Text = 'Edit3'
    end
  end
  object Button1: TButton
    Left = 8
    Top = 48
    Width = 321
    Height = 25
    Caption = '&Iniciar'
    Enabled = False
    TabOrder = 6
    OnClick = Button1Click
  end
  object Button3: TButton
    Left = 336
    Top = 48
    Width = 321
    Height = 25
    Caption = 'P&ausa/reanudar'
    TabOrder = 7
    OnClick = Button3Click
  end
  object Button2: TButton
    Left = 664
    Top = 48
    Width = 129
    Height = 49
    Caption = '&Foto'
    TabOrder = 8
    OnClick = Button2Click
  end
  object Button5: TButton
    Left = 664
    Top = 104
    Width = 129
    Height = 49
    Caption = '&Guardar instantánea'
    TabOrder = 9
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 664
    Top = 160
    Width = 129
    Height = 49
    Caption = 'Guardar &bitmap'
    TabOrder = 10
    OnClick = Button6Click
  end
  object SaveDialog1: TSaveDialog
    Filter = 'Imagen plana (*.raw)|*.raw|Todos los archivos (*.*)|*.*'
    Title = 'Guardar imagen...'
    Left = 584
    Top = 128
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 616
    Top = 128
  end
end
