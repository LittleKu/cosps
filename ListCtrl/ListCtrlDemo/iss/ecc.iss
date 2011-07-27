; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{72E6F98E-C667-4A04-8223-149AEAAF4261}
AppName=Easy Code Counter
AppVerName=Easy Code Counter 1.0.1.1
AppPublisher=www.easy-code-counter.com
AppPublisherURL=http://www.easy-code-counter.com/
AppSupportURL=http://www.easy-code-counter.com/
AppUpdatesURL=http://www.easy-code-counter.com/
DefaultDirName={pf}\Easy Code Counter
DefaultGroupName=Easy Code Counter
DisableProgramGroupPage=yes
OutputDir=.\output
OutputBaseFilename=Easy-Code-Counter-Setup-1.0.1.1
Compression=lzma
SolidCompression=yes
VersionInfoVersion=1.0.1.1
VersionInfoCopyright=Copyright (C) 2011-2016

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\Release\ecc.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release\ecc.exe.manifest"; DestDir: "{app}"; Flags: ignoreversion
Source: "{%GTB4WIN_HOME}\bin\gtb.dll"; DestDir: "{app}"; Flags: ignoreversion external
Source: "{sys}\msvcp60.dll"; DestDir: "{app}"; Flags: ignoreversion external
Source: "{sys}\mfc42.dll"; DestDir: "{sys}"; Flags: onlyifdoesntexist uninsneveruninstall external
Source: "{sys}\msvcrt.dll"; DestDir: "{sys}"; Flags: onlyifdoesntexist uninsneveruninstall external
;Source: "{sys}\mfc42.dll"; DestDir: "{app}"; Flags: ignoreversion external
;Source: "{sys}\msvcrt.dll"; DestDir: "{app}"; Flags: ignoreversion external
Source: "..\dat\*"; DestDir: "{app}\dat"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\help\*"; DestDir: "{app}\help"; Flags: onlyifdoesntexist uninsrestartdelete
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Easy Code Counter"; Filename: "{app}\ecc.exe"
Name: "{group}\Help"; Filename: "{app}\help\help_en.chm"
Name: "{group}\{cm:UninstallProgram,Easy Code Counter}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Easy Code Counter"; Filename: "{app}\ecc.exe"; Tasks: desktopicon
;Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Easy Code Counter"; Filename: "{app}\ecc.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\ecc.exe"; Description: "{cm:LaunchProgram,Easy Code Counter}"; Flags: nowait postinstall skipifsilent

[Registry]

[UninstallRun]