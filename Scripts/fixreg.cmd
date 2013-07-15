@rem a work around to a last minute bug, handling the case when FeatureControl key may not exist for current user

reg add "HKCU\SOFTWARE\Microsoft\Internet Explorer\Main\FeatureControl\FEATURE_BROWSER_EMULATION" /v ScsIA /d 10000
regsvr32 /n /i:user SiaCore.dll


