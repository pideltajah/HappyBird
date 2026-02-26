; HappyBird Inno Setup Script
; Build the project first (cmake + ninja), then compile this script with Inno Setup.
; Output: dist/HappyBirdSetup.exe

#define MyAppName "HappyBird"
#define MyAppVersion "1.0"
#define MyAppPublisher "Justin Hampton"
#define MyAppExeName "HappyBird.exe"

[Setup]
AppId={{76865417-505F-44B2-9215-EB56A4DBAAC7}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir=..\dist
OutputBaseFilename=HappyBirdSetup
SetupIconFile=..\build\favicon.ico
UninstallDisplayIcon={app}\favicon.ico
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Main executable
Source: "..\build\HappyBird.exe"; DestDir: "{app}"; Flags: ignoreversion

; SFML release DLLs only (exclude debug -d- variants)
Source: "..\build\sfml-audio-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\sfml-graphics-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\sfml-network-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\sfml-system-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\sfml-window-3.dll"; DestDir: "{app}"; Flags: ignoreversion

; Game assets
Source: "..\build\*.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\*.wav"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\*.jpg"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\Bicubik.OTF"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\favicon.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"; IconFilename: "{app}\favicon.ico"
Name: "{group}\Uninstall {#MyAppName}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"; IconFilename: "{app}\favicon.ico"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; WorkingDir: "{app}"; Flags: nowait postinstall skipifsilent
