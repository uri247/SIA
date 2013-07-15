
@rem wrapper to start start.html within MSHTA

@regsvr32 /s /n /i:user SiaCore.dll
@mshta.exe "%~dp0app.hta" {1E460BD7-F1C3-4B2E-88BF-4E770A288AF5}{1E460BD7-F1C3-4B2E-88BF-4E770A288AF5}
@regsvr32 /s /u SiaCore.dll


