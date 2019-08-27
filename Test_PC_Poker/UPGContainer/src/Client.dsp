# Microsoft Developer Studio Project File - Name="Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Client - Win32 TEST Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Client.mak" CFG="Client - Win32 TEST Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Client - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 TEST Release" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 TEST Debug" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 Dev Release" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 TestClient" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Client - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /Od /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /I ".\SockLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "MONEY_LIMIT_2010_08_14" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /out:"../_BIN/Client/NewPoker.exe" /pdbtype:sept /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib" /libpath:".\SockLib" /libpath:".NMRunParmLib"

!ELSEIF  "$(CFG)" == "Client - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /I ".\SockLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "MONEY_LIMIT_2010_08_14" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 vfw32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.ib" /out:"../_BIN/Client/NewPoker_Debug.exe" /pdbtype:sept /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib" /libpath:".\SockLib"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Client - Win32 TEST Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Client___Win32_TEST_Release"
# PROP BASE Intermediate_Dir "Client___Win32_TEST_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "TEST_Release"
# PROP Intermediate_Dir "TEST_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /I ".\SockLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /I ".\SockLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_TEST" /D "MONEY_LIMIT_2010_08_14" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"../_BIN/Client/NewPoker.exe" /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib" /libpath:".\SockLib"
# ADD LINK32 /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"../_BIN/Client_Test/NewPoker.exe" /pdbtype:sept /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib" /libpath:".\SockLib"

!ELSEIF  "$(CFG)" == "Client - Win32 TEST Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Client___Win32_TEST_Debug"
# PROP BASE Intermediate_Dir "Client___Win32_TEST_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "TEST_Debug"
# PROP Intermediate_Dir "TEST_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /I ".\SockLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT BASE CPP /WX /Fr
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /I ".\SockLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_TEST" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 vfw32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.ib" /out:"../_BIN/Client/NewPoker_Debug.exe" /pdbtype:sept /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib" /libpath:".\SockLib"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 vfw32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.ib" /out:"../_BIN/Client_Test/NewPoker_Debug.exe" /pdbtype:sept /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib" /libpath:".\SockLib"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Client - Win32 Dev Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Client___Win32_Dev_Release"
# PROP BASE Intermediate_Dir "Client___Win32_Dev_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Client___Win32_Dev_Release"
# PROP Intermediate_Dir "Client___Win32_Dev_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /I ".\SockLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /I ".\SockLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DEV" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"../_BIN/Client/NewPoker.exe" /pdbtype:sept /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib" /libpath:".\SockLib"
# ADD LINK32 /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"../_BIN/Client/NewPoker.exe" /pdbtype:sept /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib" /libpath:".\SockLib"

!ELSEIF  "$(CFG)" == "Client - Win32 TestClient"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Client___Win32_TestClient"
# PROP BASE Intermediate_Dir "Client___Win32_TestClient"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Client___Win32_TestClient"
# PROP Intermediate_Dir "Client___Win32_TestClient"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /I ".\SockLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_TEST" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\CommonFile" /I ".\GraphLib" /I ".\SoundLib" /I ".\SockLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_TEST" /D "_TESTCLIENT" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"../_BIN/Client_Test/NewPoker.exe" /pdbtype:sept /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib" /libpath:".\SockLib"
# ADD LINK32 /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"../_BIN/Client_TestClient/NewPoker.exe" /pdbtype:sept /libpath:".\GraphLib" /libpath:"..\CommonFile" /libpath:".\FolderLib" /libpath:".\SoundLib" /libpath:".\SockLib"

!ENDIF 

# Begin Target

# Name "Client - Win32 Release"
# Name "Client - Win32 Debug"
# Name "Client - Win32 TEST Release"
# Name "Client - Win32 TEST Debug"
# Name "Client - Win32 Dev Release"
# Name "Client - Win32 TestClient"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ClientDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GameModeToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\LobyDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClientDlg.h
# End Source File
# Begin Source File

SOURCE=.\GameDlg.h
# End Source File
# Begin Source File

SOURCE=.\GameModeToolTip.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\LobyDlg.h
# End Source File
# End Group
# Begin Group "Basic"

# PROP Default_Filter ""
# Begin Group "Source Files_1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files_1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AutoBetSetting.bmp
# End Source File
# Begin Source File

SOURCE=.\res\banishvote.BMP
# End Source File
# Begin Source File

SOURCE=.\res\BigMsgBack.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Client.ico
# End Source File
# Begin Source File

SOURCE=.\res\Client.rc2
# End Source File
# Begin Source File

SOURCE=.\res\croomback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_i.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_m.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_s.cur
# End Source File
# Begin Source File

SOURCE=.\res\image_itemtab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\image_ro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageChan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageCheck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageLevel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageLevel_New.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Invite.bmp
# End Source File
# Begin Source File

SOURCE=.\res\inviteback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LobyBack.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RefuseInvite.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SPoker2_GameItem.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TitleBack.bmp
# End Source File
# End Group
# Begin Group "Etc"

# PROP Default_Filter ""
# Begin Group "GraphicButton"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseDialog.h
# End Source File
# Begin Source File

SOURCE=.\BmpCheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpCheckButton.h
# End Source File
# Begin Source File

SOURCE=.\BmpRadioBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpRadioBtn.h
# End Source File
# Begin Source File

SOURCE=.\CheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckButton.h
# End Source File
# Begin Source File

SOURCE=.\GraphButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphButton.h
# End Source File
# Begin Source File

SOURCE=.\GraphButton_Text.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphButton_Text.h
# End Source File
# Begin Source File

SOURCE=.\GraphButtonList.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphButtonList.h
# End Source File
# Begin Source File

SOURCE=.\GraphScrollThumbButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphScrollThumbButton.h
# End Source File
# Begin Source File

SOURCE=.\MyBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBitmap.h
# End Source File
# Begin Source File

SOURCE=.\MyButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MyButton.h
# End Source File
# End Group
# Begin Group "SSO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SSOWebLib.h
# End Source File
# Begin Source File

SOURCE=.\Avatar.lib
# End Source File
# End Group
# Begin Group "WebControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WebControl\webbro.cpp
# End Source File
# Begin Source File

SOURCE=.\WebControl\webbro.h
# End Source File
# Begin Source File

SOURCE=.\WebEventDisplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\WebEventDisplayer.h
# End Source File
# End Group
# Begin Group "Curve"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CurveControl.cpp
# End Source File
# Begin Source File

SOURCE=.\CurveControl.h
# End Source File
# Begin Source File

SOURCE=.\spline.h
# End Source File
# End Group
# Begin Group "BadFilterLoader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BadFilter\BadFilterLoader.h
# End Source File
# Begin Source File

SOURCE=.\BadFilter\BadFilterLoader_MD.lib
# End Source File
# Begin Source File

SOURCE=.\BadFilter\BadFilterLoader_MDd.lib
# End Source File
# Begin Source File

SOURCE=.\BadFilter\BadFilterLoader_ML.lib
# End Source File
# Begin Source File

SOURCE=.\BadFilter\BadFilterLoader_MLd.lib
# End Source File
# Begin Source File

SOURCE=.\BadFilter\BadFilterLoader_MT.lib
# End Source File
# Begin Source File

SOURCE=.\BadFilter\BadFilterLoader_MTd.lib
# End Source File
# End Group
# Begin Group "NMRunParamLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NMRunParamLib.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Avatar.h
# End Source File
# Begin Source File

SOURCE=.\BannerWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BannerWnd.h
# End Source File
# Begin Source File

SOURCE=.\BmpToRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpToRegion.h
# End Source File
# Begin Source File

SOURCE=.\DesktopIconMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DesktopIconMan.h
# End Source File
# Begin Source File

SOURCE=.\DialogCtrlReposition.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogCtrlReposition.h
# End Source File
# Begin Source File

SOURCE=.\IME.cpp
# End Source File
# Begin Source File

SOURCE=.\IME.h
# End Source File
# Begin Source File

SOURCE=.\KHSToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\KHSToolTip.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\NoChat.cpp
# End Source File
# Begin Source File

SOURCE=.\NoChat.h
# End Source File
# Begin Source File

SOURCE=.\ShellMan.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellMan.h
# End Source File
# Begin Source File

SOURCE=.\SliderCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\SliderCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\SysUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\SysUtil.h
# End Source File
# Begin Source File

SOURCE=.\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\WebCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WebCtrl.h
# End Source File
# End Group
# Begin Group "Game Item Work"

# PROP Default_Filter ""
# Begin Group "Goguma&goldenfish"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GogumaPlayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GogumaPlayDlg.h
# End Source File
# Begin Source File

SOURCE=.\GoldenFishDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GoldenFishDlg.h
# End Source File
# End Group
# Begin Group "ChoiceAvata"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UniversalItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UniversalItemDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BGBuyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BGBuyDlg.h
# End Source File
# Begin Source File

SOURCE=.\BuyLuckAvataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BuyLuckAvataDlg.h
# End Source File
# Begin Source File

SOURCE=.\BuySuperAvatarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BuySuperAvatarDlg.h
# End Source File
# Begin Source File

SOURCE=.\GameItemMan.cpp
# End Source File
# Begin Source File

SOURCE=.\GameItemMan.h
# End Source File
# Begin Source File

SOURCE=.\NewGameItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewGameItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\PremServiceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PremServiceDlg.h
# End Source File
# Begin Source File

SOURCE=.\RewardItemMan.cpp
# End Source File
# Begin Source File

SOURCE=.\RewardItemMan.h
# End Source File
# End Group
# Begin Group "SafeAngelItem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HTransRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\HTransRegion.h
# End Source File
# Begin Source File

SOURCE=.\NewAskChargeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewAskChargeDlg.h
# End Source File
# Begin Source File

SOURCE=.\WebControl4Charge\WebCtrl4New.cpp
# End Source File
# Begin Source File

SOURCE=.\WebControl4Charge\WebCtrl4New.h
# End Source File
# End Group
# End Group
# Begin Group "Lib Files"

# PROP Default_Filter ""
# Begin Group "GraphLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GraphLib\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Display.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Dither.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Dither.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Graphic.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Graphic.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\GraphMod.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\GraphMod8.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Jpegfile.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Jpegfile.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Sprite8.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Sprite8.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xAniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xAniFile.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xAniMan.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xAniMan.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xHan.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xHan.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xLayer.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xMultiScene.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xMultiScene.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xScene.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xScene.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\xSprite.h
# End Source File
# End Group
# Begin Group "SockLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SockLib\DataSock.cpp
# End Source File
# Begin Source File

SOURCE=.\SockLib\DataSock.h
# End Source File
# Begin Source File

SOURCE=.\SockLib\ServerSock.cpp
# End Source File
# Begin Source File

SOURCE=.\SockLib\ServerSock.h
# End Source File
# Begin Source File

SOURCE=.\SockLib\SockCmnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SockLib\SockCmnd.h
# End Source File
# Begin Source File

SOURCE=.\SockLib\SockMan.cpp
# End Source File
# Begin Source File

SOURCE=.\SockLib\SockMan.h
# End Source File
# Begin Source File

SOURCE=.\SockLib\StreamBuf.cpp
# End Source File
# Begin Source File

SOURCE=.\SockLib\StreamBuf.h
# End Source File
# End Group
# Begin Group "SoundLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SoundLib\codec.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\MciMedia.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundLib\MciMedia.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\ogg.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\os_types.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundLib\Sound.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\vorbisenc.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\vorbisfile.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\xOgg.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundLib\xOgg.h
# End Source File
# Begin Source File

SOURCE=.\SoundLib\xSound.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundLib\xSound.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GameRes.cpp
# End Source File
# Begin Source File

SOURCE=.\GameRes.h
# End Source File
# End Group
# Begin Group "Common Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CommonFile\CardRule.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\CardRule.h
# End Source File
# Begin Source File

SOURCE=..\CommonFile\CommEventMsg.h
# End Source File
# Begin Source File

SOURCE=..\CommonFile\CommMsg.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\CommMsg.h
# End Source File
# Begin Source File

SOURCE=..\CommonFile\CommUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\CommUtil.h
# End Source File
# Begin Source File

SOURCE=..\CommonFile\CompressUtil.h
# End Source File
# Begin Source File

SOURCE=..\CommonFile\ExHandle.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\ExHandle.h
# End Source File
# Begin Source File

SOURCE=..\CommonFile\GFUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\GFUtil.h
# End Source File
# Begin Source File

SOURCE=..\CommonFile\SevenRuleMan.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\SevenRuleMan.h
# End Source File
# Begin Source File

SOURCE=..\CommonFile\UserStruct.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFile\UserStruct.h
# End Source File
# End Group
# Begin Group "Dialog Files"

# PROP Default_Filter ""
# Begin Group "Server Notice"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MsgBoxDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBoxDlg.h
# End Source File
# Begin Source File

SOURCE=.\NoticeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NoticeDlg.h
# End Source File
# End Group
# Begin Group "ListView"

# PROP Default_Filter ""
# Begin Group "ChatViewEdit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GrpChatEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\GrpChatEdit.h
# End Source File
# Begin Source File

SOURCE=.\MEMDC.H
# End Source File
# Begin Source File

SOURCE=.\SkinVerticleScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinVerticleScrollbar.h
# End Source File
# End Group
# Begin Group "TitleView"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LobyChannelListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LobyChannelListWnd.h
# End Source File
# End Group
# Begin Group "EtcView"

# PROP Default_Filter ""
# Begin Group "LobyUserProfile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LobyUserInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LobyUserInfoWnd.h
# End Source File
# End Group
# Begin Group "UserListView"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UserListView.cpp
# End Source File
# Begin Source File

SOURCE=.\UserListView.h
# End Source File
# End Group
# End Group
# Begin Group "RoomView"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LobyRoomListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LobyRoomListWnd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BaseListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseListWnd.h
# End Source File
# Begin Source File

SOURCE=.\LobyListCtrMan.cpp
# End Source File
# Begin Source File

SOURCE=.\LobyListCtrMan.h
# End Source File
# End Group
# Begin Group "Admin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AdminControlMan.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminControlMan.h
# End Source File
# Begin Source File

SOURCE=.\AdminMessageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminMessageDlg.h
# End Source File
# Begin Source File

SOURCE=.\AdminSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\AdminTitleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminTitleDlg.h
# End Source File
# Begin Source File

SOURCE=.\StatusEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusEdit.h
# End Source File
# End Group
# Begin Group "InviteDlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\InviteAllUserListView.cpp
# End Source File
# Begin Source File

SOURCE=.\InviteAllUserListView.h
# End Source File
# Begin Source File

SOURCE=.\InviteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InviteDlg.h
# End Source File
# Begin Source File

SOURCE=.\InviteUserListView.cpp
# End Source File
# Begin Source File

SOURCE=.\InviteUserListView.h
# End Source File
# Begin Source File

SOURCE=.\RecvInviteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RecvInviteDlg.h
# End Source File
# Begin Source File

SOURCE=.\RefuseInviteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RefuseInviteDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResultFindIdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultFindIdDlg.h
# End Source File
# Begin Source File

SOURCE=.\SendInviteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SendInviteDlg.h
# End Source File
# End Group
# Begin Group "BadUserAndVote"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Baduser_ConspireDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Baduser_ConspireDlg.h
# End Source File
# Begin Source File

SOURCE=.\BadUser_MoneyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BadUser_MoneyDlg.h
# End Source File
# Begin Source File

SOURCE=.\Baduser_Reportdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Baduser_Reportdlg.h
# End Source File
# Begin Source File

SOURCE=.\BadUserComplete.cpp
# End Source File
# Begin Source File

SOURCE=.\BadUserComplete.h
# End Source File
# Begin Source File

SOURCE=.\BadUserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BadUserDlg.h
# End Source File
# Begin Source File

SOURCE=.\BadUserWarningDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BadUserWarningDlg.h
# End Source File
# Begin Source File

SOURCE=.\BanishVoteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BanishVoteDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyVoteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyVoteDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlEx.h
# End Source File
# End Group
# Begin Group "Basic Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoadingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadingDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\MsgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBox.h
# End Source File
# Begin Source File

SOURCE=.\MsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelCardChoiceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelCardChoiceDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelCardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelCardDlg.h
# End Source File
# Begin Source File

SOURCE=.\WebProfileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WebProfileDlg.h
# End Source File
# Begin Source File

SOURCE=.\WhisperDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WhisperDlg.h
# End Source File
# End Group
# Begin Group "EventDlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BestEventDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BestEventDlg.h
# End Source File
# Begin Source File

SOURCE=.\EventLobyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EventLobyDlg.h
# End Source File
# Begin Source File

SOURCE=.\HotkeySurveyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HotkeySurveyDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewGoldenTimePopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewGoldenTimePopupDlg.h
# End Source File
# Begin Source File

SOURCE=.\NoticeNewRuleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NoticeNewRuleDlg.h
# End Source File
# Begin Source File

SOURCE=.\PolicyChangeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PolicyChangeDlg.h
# End Source File
# End Group
# Begin Group "SprDlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SprDlgBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SprDlgBase.h
# End Source File
# Begin Source File

SOURCE=.\SprDlgList.cpp
# End Source File
# Begin Source File

SOURCE=.\SprDlgList.h
# End Source File
# Begin Source File

SOURCE=.\SprDlgObjectMan.cpp
# End Source File
# Begin Source File

SOURCE=.\SprDlgObjectMan.h
# End Source File
# End Group
# Begin Group "AutoBet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutoBetSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoBetSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoBetSetUpWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoBetSetUpWnd.h
# End Source File
# Begin Source File

SOURCE=.\BettingInfoMsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BettingInfoMsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewtAutoBetManDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewtAutoBetManDlg.h
# End Source File
# End Group
# Begin Group "EndBanner"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BannerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BannerDlg.h
# End Source File
# End Group
# Begin Group "종료 팝업들"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EndPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\EndPopup.h
# End Source File
# Begin Source File

SOURCE=.\EventGoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EventGoDlg.h
# End Source File
# End Group
# Begin Group "CreateRoom"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CreateRoomDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateRoomDlg.h
# End Source File
# Begin Source File

SOURCE=.\RoomOptionMan.cpp
# End Source File
# Begin Source File

SOURCE=.\RoomOptionMan.h
# End Source File
# End Group
# Begin Group "Observer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ObserverDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObserverDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObserverListView.cpp
# End Source File
# Begin Source File

SOURCE=.\ObserverListView.h
# End Source File
# Begin Source File

SOURCE=.\ObserverMan.cpp
# End Source File
# Begin Source File

SOURCE=.\ObserverMan.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AdminRemoveRoomDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminRemoveRoomDlg.h
# End Source File
# Begin Source File

SOURCE=.\DSEventLevelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DSEventLevelDlg.h
# End Source File
# Begin Source File

SOURCE=.\GameDirectGo.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDirectGo.h
# End Source File
# Begin Source File

SOURCE=.\GameDirectGoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDirectGoDlg.h
# End Source File
# Begin Source File

SOURCE=.\GameGradeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GameGradeDlg.h
# End Source File
# Begin Source File

SOURCE=.\LeadersMoneyLimit.cpp
# End Source File
# Begin Source File

SOURCE=.\LeadersMoneyLimit.h
# End Source File
# Begin Source File

SOURCE=.\LimitMoneyConfirmDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LimitMoneyConfirmDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\Oring.cpp
# End Source File
# Begin Source File

SOURCE=.\Oring.h
# End Source File
# Begin Source File

SOURCE=.\ProfileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResultHi.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultHi.h
# End Source File
# Begin Source File

SOURCE=.\RoomOptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RoomOptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\RoomPassDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RoomPassDlg.h
# End Source File
# Begin Source File

SOURCE=.\StartDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StartDlg.h
# End Source File
# Begin Source File

SOURCE=.\SuperAvataPopUpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SuperAvataPopUpDlg.h
# End Source File
# Begin Source File

SOURCE=.\SupplySuperAvataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SupplySuperAvataDlg.h
# End Source File
# Begin Source File

SOURCE=.\TodayTipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TodayTipDlg.h
# End Source File
# End Group
# Begin Group "Game Files"

# PROP Default_Filter ""
# Begin Group "ChattingBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CharCutInArray.cpp
# End Source File
# Begin Source File

SOURCE=.\CharCutInArray.h
# End Source File
# Begin Source File

SOURCE=.\ChatBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatBox.h
# End Source File
# Begin Source File

SOURCE=.\ChatViewEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatViewEdit.h
# End Source File
# Begin Source File

SOURCE=.\SendEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SendEdit.h
# End Source File
# End Group
# Begin Group "SevenPoker"

# PROP Default_Filter ""
# Begin Group "NMSTR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NMSTR.cpp
# End Source File
# Begin Source File

SOURCE=.\NMSTR.h
# End Source File
# Begin Source File

SOURCE=.\NMSTRMAN.cpp
# End Source File
# Begin Source File

SOURCE=.\NMSTRMAN.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AllBetChip.cpp
# End Source File
# Begin Source File

SOURCE=.\AllBetChip.h
# End Source File
# Begin Source File

SOURCE=.\BombCard.cpp
# End Source File
# Begin Source File

SOURCE=.\BombCard.h
# End Source File
# Begin Source File

SOURCE=.\CardDefine.h
# End Source File
# Begin Source File

SOURCE=.\MovementEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MovementEx.h
# End Source File
# Begin Source File

SOURCE=.\Probability.cpp
# End Source File
# Begin Source File

SOURCE=.\Probability.h
# End Source File
# Begin Source File

SOURCE=.\ShortCut.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortCut.h
# End Source File
# Begin Source File

SOURCE=.\SprPublicMomeyDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\SprPublicMomeyDraw.h
# End Source File
# End Group
# Begin Group "Event"

# PROP Default_Filter ""
# Begin Group "GMarket"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GMarketEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\GMarketEvent.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BettingEventPopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BettingEventPopupDlg.h
# End Source File
# Begin Source File

SOURCE=.\EventMan.cpp
# End Source File
# Begin Source File

SOURCE=.\EventMan.h
# End Source File
# Begin Source File

SOURCE=.\EventPopUpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EventPopUpDlg.h
# End Source File
# Begin Source File

SOURCE=.\GoldenEventGaugeManEx.cpp
# End Source File
# Begin Source File

SOURCE=.\GoldenEventGaugeManEx.h
# End Source File
# Begin Source File

SOURCE=.\MyEventButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEventButton.h
# End Source File
# Begin Source File

SOURCE=.\NewGoldenTimeEventMan.cpp
# End Source File
# Begin Source File

SOURCE=.\NewGoldenTimeEventMan.h
# End Source File
# Begin Source File

SOURCE=.\SDriveEventClient.cpp
# End Source File
# Begin Source File

SOURCE=.\SDriveEventClient.h
# End Source File
# Begin Source File

SOURCE=.\UnionEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\UnionEvent.h
# End Source File
# End Group
# Begin Group "Player"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CharBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CharBox.h
# End Source File
# Begin Source File

SOURCE=.\CharView.cpp
# End Source File
# Begin Source File

SOURCE=.\CharView.h
# End Source File
# Begin Source File

SOURCE=.\MyBetChip.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBetChip.h
# End Source File
# Begin Source File

SOURCE=.\MyCard.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCard.h
# End Source File
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\PlayScr.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayScr.h
# End Source File
# Begin Source File

SOURCE=.\SprMoneyDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\SprMoneyDraw.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AutoBettingSmallSpr.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoBettingSmallSpr.h
# End Source File
# Begin Source File

SOURCE=.\ButtonToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonToolTip.h
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\GameOverResult.cpp
# End Source File
# Begin Source File

SOURCE=.\GameOverResult.h
# End Source File
# Begin Source File

SOURCE=.\GameView.cpp
# End Source File
# Begin Source File

SOURCE=.\GameView.h
# End Source File
# Begin Source File

SOURCE=.\LevelView.cpp
# End Source File
# Begin Source File

SOURCE=.\LevelView.h
# End Source File
# Begin Source File

SOURCE=.\ResultFocus.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultFocus.h
# End Source File
# Begin Source File

SOURCE=.\ResultHistoryMan.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultHistoryMan.h
# End Source File
# Begin Source File

SOURCE=.\SelectCard.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectCard.h
# End Source File
# Begin Source File

SOURCE=.\SuperAvataView.cpp
# End Source File
# Begin Source File

SOURCE=.\SuperAvataView.h
# End Source File
# Begin Source File

SOURCE=.\SyncFPS.cpp
# End Source File
# Begin Source File

SOURCE=.\SyncFPS.h
# End Source File
# Begin Source File

SOURCE=.\ThreadTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadTimer.h
# End Source File
# Begin Source File

SOURCE=.\VoiceMan.cpp
# End Source File
# Begin Source File

SOURCE=.\VoiceMan.h
# End Source File
# Begin Source File

SOURCE=.\VSModePlayRoundResultMan.cpp
# End Source File
# Begin Source File

SOURCE=.\VSModePlayRoundResultMan.h
# End Source File
# End Group
# Begin Group "Anilistman"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AniFileListMan.cpp
# End Source File
# Begin Source File

SOURCE=.\AniFileListMan.h
# End Source File
# Begin Source File

SOURCE=.\BaseObject.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseObject.h
# End Source File
# Begin Source File

SOURCE=.\NMultiScene.cpp
# End Source File
# Begin Source File

SOURCE=.\NMultiScene.h
# End Source File
# Begin Source File

SOURCE=.\NScene.cpp
# End Source File
# Begin Source File

SOURCE=.\NScene.h
# End Source File
# Begin Source File

SOURCE=.\ObjectList.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectList.h
# End Source File
# End Group
# Begin Group "NWindow"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChattingWindowMan.cpp
# End Source File
# Begin Source File

SOURCE=.\ChattingWindowMan.h
# End Source File
# Begin Source File

SOURCE=.\GameViewButtonMan.cpp
# End Source File
# Begin Source File

SOURCE=.\GameViewButtonMan.h
# End Source File
# Begin Source File

SOURCE=.\GraphButtonEx.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphButtonEx.h
# End Source File
# Begin Source File

SOURCE=.\NChatView.cpp
# End Source File
# Begin Source File

SOURCE=.\NChatView.h
# End Source File
# Begin Source File

SOURCE=.\NEditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\NEditBox.h
# End Source File
# Begin Source File

SOURCE=.\NText.cpp
# End Source File
# Begin Source File

SOURCE=.\NText.h
# End Source File
# End Group
# Begin Group "Module"

# PROP Default_Filter ""
# Begin Group "VIP Service"

# PROP Default_Filter ""
# Begin Group "Dlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VipCloseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VipCloseDlg.h
# End Source File
# Begin Source File

SOURCE=.\VipServiceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VipServiceDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GrpChatEdit2.cpp
# End Source File
# Begin Source File

SOURCE=.\GrpChatEdit2.h
# End Source File
# Begin Source File

SOURCE=.\SkinVerticleScrollbar2.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinVerticleScrollbar2.h
# End Source File
# Begin Source File

SOURCE=.\VipCommMsg.h
# End Source File
# Begin Source File

SOURCE=.\VipService.cpp
# End Source File
# Begin Source File

SOURCE=.\VipService.h
# End Source File
# Begin Source File

SOURCE=.\VipServiceSockCmnd.cpp
# End Source File
# Begin Source File

SOURCE=.\VipServiceSockCmnd.h
# End Source File
# Begin Source File

SOURCE=.\VipStruct.h
# End Source File
# End Group
# Begin Group "JekiCharge"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FontBag.cpp
# End Source File
# Begin Source File

SOURCE=.\FontBag.h
# End Source File
# Begin Source File

SOURCE=.\JekiCharge.cpp
# End Source File
# Begin Source File

SOURCE=.\JekiCharge.h
# End Source File
# Begin Source File

SOURCE=.\MyTextButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTextButton.h
# End Source File
# Begin Source File

SOURCE=.\RecoveryChargeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RecoveryChargeDlg.h
# End Source File
# End Group
# Begin Group "LimitMoney"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AskAgainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AskAgainDlg.h
# End Source File
# Begin Source File

SOURCE=.\AskSubscribeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AskSubscribeDlg.h
# End Source File
# Begin Source File

SOURCE=.\LimitMoneyAlertDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LimitMoneyAlertDlg.h
# End Source File
# Begin Source File

SOURCE=.\RestoreCutMoneyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RestoreCutMoneyDlg.h
# End Source File
# End Group
# Begin Group "Kisa"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KisaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KisaDlg.h
# End Source File
# End Group
# Begin Group "Quick Join"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\QuickJoinerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QuickJoinerDlg.h
# End Source File
# Begin Source File

SOURCE=.\RefuseQJDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RefuseQJDlg.h
# End Source File
# End Group
# Begin Group "게임 도우미"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GH_CardValueMan.cpp
# End Source File
# Begin Source File

SOURCE=.\GH_CardValueMan.h
# End Source File
# Begin Source File

SOURCE=.\Probability_AllCard.cpp
# End Source File
# Begin Source File

SOURCE=.\Probability_AllCard.h
# End Source File
# Begin Source File

SOURCE=.\Value_Other_Hi_Rule.cpp
# End Source File
# Begin Source File

SOURCE=.\Value_Other_Hi_Rule.h
# End Source File
# End Group
# Begin Group "패밀리 아이콘"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FamilyIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\FamilyIcon.h
# End Source File
# Begin Source File

SOURCE=.\IconList.cpp
# End Source File
# Begin Source File

SOURCE=.\IconList.h
# End Source File
# End Group
# Begin Group "자리지킴이"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SeatKeeper.cpp
# End Source File
# Begin Source File

SOURCE=.\SeatKeeper.h
# End Source File
# End Group
# Begin Group "패밀리 자세히"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyFamilyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFamilyDlg.h
# End Source File
# End Group
# Begin Group "테스트 url 변경"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameTest.cpp
# End Source File
# Begin Source File

SOURCE=.\GameTest.h
# End Source File
# End Group
# Begin Group "폭탄실전 입장"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RealAdmissionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RealAdmissionDlg.h
# End Source File
# End Group
# Begin Group "보유한도 보상"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CompansationAskPlayGame.cpp
# End Source File
# Begin Source File

SOURCE=.\CompansationAskPlayGame.h
# End Source File
# Begin Source File

SOURCE=.\CompensationViewLimitMoneyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CompensationViewLimitMoneyDlg.h
# End Source File
# Begin Source File

SOURCE=.\FamilyCompensationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FamilyCompensationDlg.h
# End Source File
# End Group
# Begin Group "PlayTime"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PlayTimeWarning.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayTimeWarning.h
# End Source File
# End Group
# Begin Group "Shared"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\shared\KeyDefine.h
# End Source File
# Begin Source File

SOURCE=.\shared\SharedDefine.h
# End Source File
# End Group
# Begin Group "보유한도초과"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Animation.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation.h
# End Source File
# Begin Source File

SOURCE=.\OwnLimit.cpp
# End Source File
# Begin Source File

SOURCE=.\OwnLimit.h
# End Source File
# Begin Source File

SOURCE=.\OwnLimitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OwnLimitDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\FamilySelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FamilySelectDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Client : {3F3226B7-1ACF-4A4D-9073-C59C050776DF}
# 	1:22:IDD_DIALOG_QUICKJOINER:149
# 	2:16:Resource Include:resource.h
# 	2:25:CLASS: CQuickJoinControls:CQuickJoinControls
# 	2:22:IDD_DIALOG_QUICKJOINER:IDD_DIALOG_QUICKJOINER
# 	2:22:CLASS: CQuickJoinerDlg:CQuickJoinerDlg
# 	2:18:QuickJoinerDlg.cpp:QuickJoinerDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:18:ENUM: TYPE_CONTROL:TYPE_CONTROL
# 	2:22:CLASS: CMyCQJCondition:CMyCQJCondition
# 	2:16:QuickJoinerDlg.h:QuickJoinerDlg.h
# 	2:19:CLASS: CQJCondition:CQJCondition
# 	2:19:Application Include:client.h
# 	2:23:CLASS: CQJConditionBase:CQJConditionBase
# End Section
# Section Client : {413EF58E-B69B-4F81-AE09-77D34E9D59D1}
# 	1:21:IDD_DIALOG_BEST_EVENT:122
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CBestEventDlg:CBestEventDlg
# 	2:10:ENUM: enum:enum
# 	2:16:BestEventDlg.cpp:BestEventDlg.cpp
# 	2:21:IDD_DIALOG_BEST_EVENT:IDD_DIALOG_BEST_EVENT
# 	2:14:BestEventDlg.h:BestEventDlg.h
# 	2:19:Application Include:client.h
# End Section
# Section Client : {6E8E4694-5B3E-49CB-852F-64CAAECA0C3A}
# 	1:22:IDD_DIALOG_VIP_SERVICE:140
# 	2:16:Resource Include:resource.h
# 	2:17:VipServiceDlg.cpp:VipServiceDlg.cpp
# 	2:21:CLASS: CVipServiceDlg:CVipServiceDlg
# 	2:15:VipServiceDlg.h:VipServiceDlg.h
# 	2:10:ENUM: enum:enum
# 	2:22:IDD_DIALOG_VIP_SERVICE:IDD_DIALOG_VIP_SERVICE
# 	2:19:Application Include:client.h
# End Section
# Section Client : {4806A5B1-BF40-434E-BF40-1B5DC4F574F4}
# 	1:20:IDD_DIALOG_BODYGUARD:109
# 	2:16:Resource Include:resource.h
# 	2:12:BGBuyDlg.cpp:BGBuyDlg.cpp
# 	2:16:CLASS: CBGBuyDlg:CBGBuyDlg
# 	2:20:IDD_DIALOG_BODYGUARD:IDD_DIALOG_BODYGUARD
# 	2:10:BGBuyDlg.h:BGBuyDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:client.h
# End Section
# Section Client : {0E0BC3E7-3C62-4F4E-B965-AA93FD84C550}
# 	1:19:IDD_DIALOG_REFUSEQJ:150
# 	2:16:Resource Include:resource.h
# 	2:15:RefuseQJDlg.cpp:RefuseQJDlg.cpp
# 	2:13:RefuseQJDlg.h:RefuseQJDlg.h
# 	2:19:CLASS: CRefuseQJDlg:CRefuseQJDlg
# 	2:10:ENUM: enum:enum
# 	2:19:IDD_DIALOG_REFUSEQJ:IDD_DIALOG_REFUSEQJ
# 	2:19:Application Include:client.h
# End Section
# Section Client : {72C13C25-9917-4F37-96B2-C37422DB48FD}
# 	1:11:IDD_DIALOG4:124
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:18:NoticeNewRuleDlg.h:NoticeNewRuleDlg.h
# 	2:20:NoticeNewRuleDlg.cpp:NoticeNewRuleDlg.cpp
# 	2:24:CLASS: CNoticeNewRuleDlg:CNoticeNewRuleDlg
# 	2:11:IDD_DIALOG4:IDD_DIALOG4
# 	2:19:Application Include:client.h
# End Section
# Section Client : {C357D8A8-ACAE-44FD-9F50-CAD916877AD1}
# 	1:25:IDD_DIALOG_UNIVERSAL_ITEM:160
# 	2:16:Resource Include:resource.h
# 	2:25:IDD_DIALOG_UNIVERSAL_ITEM:IDD_DIALOG_UNIVERSAL_ITEM
# 	2:10:ENUM: enum:enum
# 	2:18:UniversalItemDlg.h:UniversalItemDlg.h
# 	2:24:CLASS: CUniversalItemDlg:CUniversalItemDlg
# 	2:19:Application Include:client.h
# 	2:20:UniversalItemDlg.cpp:UniversalItemDlg.cpp
# End Section
# Section Client : {A9B01EE6-350B-4FE8-884B-FC467C090D24}
# 	1:21:IDD_DIALOG_BEST_EVENT:160
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CBestEventDlg:CBestEventDlg
# 	2:10:ENUM: enum:enum
# 	2:16:BestEventDlg.cpp:BestEventDlg.cpp
# 	2:21:IDD_DIALOG_BEST_EVENT:IDD_DIALOG_BEST_EVENT
# 	2:14:BestEventDlg.h:BestEventDlg.h
# 	2:19:Application Include:client.h
# End Section
# Section Client : {3C40A311-FFF2-4750-BC39-145E8A8391BB}
# 	1:19:IDD_DIALOG_MYFAMILY:151
# 	2:16:Resource Include:resource.h
# 	2:19:CLASS: CMyFamilyDlg:CMyFamilyDlg
# 	2:19:IDD_DIALOG_MYFAMILY:IDD_DIALOG_MYFAMILY
# 	2:10:ENUM: enum:enum
# 	2:15:MyFamilyDlg.cpp:MyFamilyDlg.cpp
# 	2:13:MyFamilyDlg.h:MyFamilyDlg.h
# 	2:19:Application Include:client.h
# End Section
# Section Client : {A6683EA6-E47A-4E2B-9AEF-1E8F5A9B3015}
# 	1:21:IDD_DIALOG_GAME_GRADE:117
# 	2:16:Resource Include:resource.h
# 	2:21:IDD_DIALOG_GAME_GRADE:IDD_DIALOG_GAME_GRADE
# 	2:20:CLASS: CGameGradeDlg:CGameGradeDlg
# 	2:14:GameGradeDlg.h:GameGradeDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:client.h
# 	2:16:GameGradeDlg.cpp:GameGradeDlg.cpp
# End Section
# Section Client : {857A2B35-3337-46DF-AB27-BA46B3D2523E}
# 	1:18:IDD_DIALOG_LOADING:162
# 	2:18:CLASS: CLoadingDlg:CLoadingDlg
# 	2:19:CLASS: CLoadManager:CLoadManager
# 	2:16:Resource Include:resource.h
# 	2:14:LoadingDlg.cpp:LoadingDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_DIALOG_LOADING:IDD_DIALOG_LOADING
# 	2:12:LoadingDlg.h:LoadingDlg.h
# 	2:19:Application Include:client.h
# End Section
# Section Client : {089D3B0E-052C-4FD5-AF10-07B3B8D51CAE}
# 	1:19:IDD_DIALOG_ENDPOPUP:152
# 	2:16:Resource Include:resource.h
# 	2:10:EndPopup.h:EndPopup.h
# 	2:19:IDD_DIALOG_ENDPOPUP:IDD_DIALOG_ENDPOPUP
# 	2:10:ENUM: enum:enum
# 	2:16:CLASS: CEndPopup:CEndPopup
# 	2:19:Application Include:client.h
# 	2:12:EndPopup.cpp:EndPopup.cpp
# End Section
# Section Client : {39D14F2C-D5F4-48BA-8355-540D83D46FC8}
# 	2:26:CLASS: CInviteUserListView:CInviteUserListView
# 	2:21:CLASS: SMALLUSERINFO1:SMALLUSERINFO1
# 	2:10:ENUM: enum:enum
# 	2:20:InviteUserListView.h:InviteUserListView1.h
# 	2:22:InviteUserListView.cpp:InviteUserListView1.cpp
# 	2:19:Application Include:client.h
# 	2:23:TYPEDEF: SMALLUSERINFO1:SMALLUSERINFO1
# End Section
# Section Client : {3CC0913D-C8B7-4A38-9160-C79CF68782DC}
# 	1:24:IDD_DIALOG_NEWYEAR_EVENT:123
# 	2:16:Resource Include:resource.h
# 	2:23:CLASS: CNewYearEventDlg:CNewYearEventDlg
# 	2:10:ENUM: enum:enum
# 	2:19:NewYearEventDlg.cpp:NewYearEventDlg.cpp
# 	2:24:IDD_DIALOG_NEWYEAR_EVENT:IDD_DIALOG_NEWYEAR_EVENT
# 	2:19:Application Include:client.h
# 	2:17:NewYearEventDlg.h:NewYearEventDlg.h
# End Section
# Section Client : {0BD08A98-FB0C-4C76-9B49-2ADBF0B6170F}
# 	1:22:IDD_DIALOG_FAMILY_LOBY:120
# 	2:16:Resource Include:resource.h
# 	2:17:FamilyLobyDlg.cpp:FamilyLobyDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:15:FamilyLobyDlg.h:FamilyLobyDlg.h
# 	2:19:Application Include:client.h
# 	2:22:IDD_DIALOG_FAMILY_LOBY:IDD_DIALOG_FAMILY_LOBY
# 	2:21:CLASS: CFamilyLobyDlg:CFamilyLobyDlg
# End Section
# Section Client : {AD913274-21A6-438C-9FA7-31136CE20052}
# 	1:23:IDD_DIALOG_FAMILY_STAMP:121
# 	2:16:Resource Include:resource.h
# 	2:23:IDD_DIALOG_FAMILY_STAMP:IDD_DIALOG_FAMILY_STAMP
# 	2:10:ENUM: enum:enum
# 	2:16:FamilyStampDlg.h:FamilyStampDlg.h
# 	2:22:CLASS: CFamilyStampDlg:CFamilyStampDlg
# 	2:19:Application Include:client.h
# 	2:18:FamilyStampDlg.cpp:FamilyStampDlg.cpp
# End Section
# Section Client : {594349F8-60CD-419B-8A9B-05265BDE8FB0}
# 	2:29:CLASS: CInviteAllUserListView:CInviteAllUserListView
# 	2:23:InviteAllUserListView.h:InviteAllUserListView1.h
# 	2:19:Application Include:client.h
# 	2:25:InviteAllUserListView.cpp:InviteAllUserListView1.cpp
# End Section
# Section Client : {5A24F35C-9EBE-46B4-A852-88A1188ABBAD}
# 	1:24:IDD_DIALOG_LEADERS_EVENT:113
# 	2:16:Resource Include:resource.h
# 	2:24:IDD_DIALOG_LEADERS_EVENT:IDD_DIALOG_LEADERS_EVENT
# 	2:19:LeadersEventDlg.cpp:LeadersEventDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:17:LeadersEventDlg.h:LeadersEventDlg.h
# 	2:23:CLASS: CLeadersEventDlg:CLeadersEventDlg
# 	2:19:Application Include:client.h
# End Section
# Section Client : {9A920AD4-E270-4D6D-B2E8-1EF2ABE38C19}
# 	1:17:IDD_DIALOG_GOGUMA:142
# 	2:16:Resource Include:resource.h
# 	2:17:IDD_DIALOG_GOGUMA:IDD_DIALOG_GOGUMA
# 	2:21:CLASS: CGogumaPlayDlg:CGogumaPlayDlg
# 	2:17:GogumaPlayDlg.cpp:GogumaPlayDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:client.h
# 	2:15:GogumaPlayDlg.h:GogumaPlayDlg.h
# End Section
# Section Client : {695C91D6-1ED1-4EDE-B580-C3A6B18A91C8}
# 	1:21:IDD_DIALOG_EVENTPOPUP:111
# 	2:16:Resource Include:resource.h
# 	2:17:EventPopUpDlg.cpp:EventPopUpDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: CEventPopUpDlg:CEventPopUpDlg
# 	2:15:EventPopUpDlg.h:EventPopUpDlg.h
# 	2:21:IDD_DIALOG_EVENTPOPUP:IDD_DIALOG_EVENTPOPUP
# 	2:19:Application Include:client.h
# End Section
# Section Client : {8AD1E979-CC93-40F7-B6FE-24EE43216AEF}
# 	1:22:IDD_DIALOG_NEWGAMEITEM:110
# 	2:16:Resource Include:resource.h
# 	2:16:NewGameItemDlg.h:NewGameItemDlg.h
# 	2:18:NewGameItemDlg.cpp:NewGameItemDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:22:CLASS: CNewGameItemDlg:CNewGameItemDlg
# 	2:19:Application Include:client.h
# 	2:22:IDD_DIALOG_NEWGAMEITEM:IDD_DIALOG_NEWGAMEITEM
# End Section
# Section Client : {DC9F2440-8EF6-42DF-A8F2-99EF01C511DF}
# 	1:23:IDD_DIALOG_GAMEDIRECTGO:138
# 	2:16:Resource Include:resource.h
# 	2:23:IDD_DIALOG_GAMEDIRECTGO:IDD_DIALOG_GAMEDIRECTGO
# 	2:19:GameDirectGoDlg.cpp:GameDirectGoDlg.cpp
# 	2:17:GameDirectGoDlg.h:GameDirectGoDlg.h
# 	2:10:ENUM: enum:enum
# 	2:23:CLASS: CGameDirectGoDlg:CGameDirectGoDlg
# 	2:19:Application Include:client.h
# End Section
# Section Client : {138A761E-4893-418D-834F-8A67330D70C7}
# 	2:29:CLASS: CInviteAllUserListView:CInviteAllUserListView
# 	2:23:InviteAllUserListView.h:InviteAllUserListView.h
# 	2:25:InviteAllUserListView.cpp:InviteAllUserListView.cpp
# 	2:19:Application Include:client.h
# End Section
# Section Client : {0C7D0E65-4D98-49D8-B1A0-5B44A54881DC}
# 	1:19:IDD_DIALOG_CGVEVENT:112
# 	2:16:Resource Include:resource.h
# 	2:19:IDD_DIALOG_CGVEVENT:IDD_DIALOG_CGVEVENT
# 	2:10:ENUM: enum:enum
# 	2:15:CGVEventDlg.cpp:CGVEventDlg.cpp
# 	2:19:CLASS: CCGVEventDlg:CCGVEventDlg
# 	2:13:CGVEventDlg.h:CGVEventDlg.h
# 	2:19:Application Include:client.h
# End Section
# Section Client : {A22EB5FA-CA15-4ADE-B9A5-C37CEB23F93B}
# 	1:24:IDD_BADUSER_MONEY_DIALOG:116
# 	2:16:Resource Include:resource.h
# 	2:18:BadUser_MoneyDlg.h:BadUser_MoneyDlg.h
# 	2:20:BadUser_MoneyDlg.cpp:BadUser_MoneyDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:24:IDD_BADUSER_MONEY_DIALOG:IDD_BADUSER_MONEY_DIALOG
# 	2:19:Application Include:client.h
# 	2:24:CLASS: CBadUser_MoneyDlg:CBadUser_MoneyDlg
# End Section
# Section Client : {0C8F2780-0D9B-45BB-9A43-8A34BB64D267}
# 	1:27:IDD_DIALOG_LIMITMONEY_ALERT:147
# 	2:16:Resource Include:resource.h
# 	2:20:LimitMoneyAlertDlg.h:LimitMoneyAlertDlg1.h
# 	2:26:CLASS: CLimitMoneyAlertDlg:CLimitMoneyAlertDlg
# 	2:10:ENUM: enum:enum
# 	2:22:LimitMoneyAlertDlg.cpp:LimitMoneyAlertDlg1.cpp
# 	2:19:Application Include:client.h
# 	2:27:IDD_DIALOG_LIMITMONEY_ALERT:IDD_DIALOG_LIMITMONEY_ALERT
# End Section
# Section Client : {62A96397-D4A7-43B0-8545-367ABDB6FE32}
# 	1:18:IDD_INVITE_DIALOG1:106
# 	2:16:Resource Include:resource.h
# 	2:11:InviteDlg.h:InviteDlg1.h
# 	2:13:InviteDlg.cpp:InviteDlg1.cpp
# 	2:10:ENUM: enum:enum
# 	2:17:IDD_INVITE_DIALOG:IDD_INVITE_DIALOG1
# 	2:17:CLASS: CInviteDlg:CInviteDlg
# 	2:19:Application Include:client.h
# End Section
# Section Client : {A0632D61-6E4F-4EEB-BCA9-D8C537D969FA}
# 	1:24:IDD_DIALOG_REALADMISSION:155
# 	2:16:Resource Include:resource.h
# 	2:24:IDD_DIALOG_REALADMISSION:IDD_DIALOG_REALADMISSION
# 	2:24:CLASS: CRealAdmissionDlg:CRealAdmissionDlg
# 	2:20:ENUM: MODE_ADMISSION:MODE_ADMISSION
# 	2:10:ENUM: enum:enum
# 	2:18:RealAdmissionDlg.h:RealAdmissionDlg.h
# 	2:20:RealAdmissionDlg.cpp:RealAdmissionDlg.cpp
# 	2:19:Application Include:client.h
# End Section
# Section Client : {D8D1D70D-94F3-4A37-B104-FAE0D3A8DE1E}
# 	1:36:IDD_DIALOG_COMPENSATION_ASK_PLAYGAME:156
# 	2:16:Resource Include:resource.h
# 	2:31:CLASS: CCompansationAskPlayGame:CCompansationAskPlayGame
# 	2:10:ENUM: enum:enum
# 	2:27:CompansationAskPlayGame.cpp:CompansationAskPlayGame.cpp
# 	2:25:CompansationAskPlayGame.h:CompansationAskPlayGame.h
# 	2:36:IDD_DIALOG_COMPENSATION_ASK_PLAYGAME:IDD_DIALOG_COMPENSATION_ASK_PLAYGAME
# 	2:19:Application Include:client.h
# End Section
# Section Client : {B2AC5E9D-C9A8-40C8-AC8A-88797DC6E605}
# 	1:26:IDD_DIALOG_BADUSERCOMPLETE:107
# 	2:16:Resource Include:resource.h
# 	2:19:BadUserComplete.cpp:BadUserComplete.cpp
# 	2:10:ENUM: enum:enum
# 	2:17:BadUserComplete.h:BadUserComplete.h
# 	2:23:CLASS: CBadUserComplete:CBadUserComplete
# 	2:26:IDD_DIALOG_BADUSERCOMPLETE:IDD_DIALOG_BADUSERCOMPLETE
# 	2:19:Application Include:client.h
# End Section
# Section Client : {B2A15F87-2C14-41D7-90C2-898514223318}
# 	1:19:IDD_OWNLIMIT_DIALOG:164
# 	2:16:Resource Include:resource.h
# 	2:19:CLASS: COwnLimitDlg:COwnLimitDlg
# 	2:15:OwnLimitDlg.cpp:OwnLimitDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:client.h
# 	2:13:OwnLimitDlg.h:OwnLimitDlg.h
# 	2:19:IDD_OWNLIMIT_DIALOG:IDD_OWNLIMIT_DIALOG
# End Section
# Section Client : {C825D562-BAC6-4783-B025-8810F10B233E}
# 	1:15:IDD_DIALOG_KISA:148
# 	2:16:Resource Include:resource.h
# 	2:11:KisaDlg.cpp:KisaDlg.cpp
# 	2:15:IDD_DIALOG_KISA:IDD_DIALOG_KISA
# 	2:9:KisaDlg.h:KisaDlg.h
# 	2:10:ENUM: enum:enum
# 	2:15:CLASS: CKisaDlg:CKisaDlg
# 	2:19:Application Include:client.h
# End Section
# Section Client : {3EB40A1F-0FE5-4294-B0C3-F63160197064}
# 	1:17:IDD_DIALOG_POLICY:125
# 	2:16:Resource Include:resource.h
# 	2:19:PolicyChangeDlg.cpp:PolicyChangeDlg.cpp
# 	2:23:CLASS: CPolicyChangeDlg:CPolicyChangeDlg
# 	2:17:IDD_DIALOG_POLICY:IDD_DIALOG_POLICY
# 	2:10:ENUM: enum:enum
# 	2:17:PolicyChangeDlg.h:PolicyChangeDlg.h
# 	2:19:Application Include:client.h
# End Section
# Section Client : {F476484E-9D89-410F-85B0-1160F2B1812A}
# 	1:11:IDD_DIALOG5:139
# 	2:16:Resource Include:resource.h
# 	2:15:VipCloseDlg.cpp:VipCloseDlg.cpp
# 	2:13:VipCloseDlg.h:VipCloseDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:CLASS: CVipCloseDlg:CVipCloseDlg
# 	2:19:Application Include:client.h
# 	2:11:IDD_DIALOG5:IDD_DIALOG5
# End Section
# Section Client : {59320DF6-ECD2-4B47-85F6-8F3FA702EC91}
# 	1:21:IDD_DIALOG_GOLDENFISH:141
# 	2:16:Resource Include:resource.h
# 	2:21:IDD_DIALOG_GOLDENFISH:IDD_DIALOG_GOLDENFISH
# 	2:10:ENUM: enum:enum
# 	2:17:GoldenFishDlg.cpp:GoldenFishDlg.cpp
# 	2:21:CLASS: CGoldenFishDlg:CGoldenFishDlg
# 	2:19:Application Include:client.h
# 	2:15:GoldenFishDlg.h:GoldenFishDlg.h
# End Section
# Section Client : {DEDF57BD-90D6-4E6B-87A5-824E32586337}
# 	1:38:IDD_DIALOG_COMPENSATION_VIEWLIMITMONEY:158
# 	2:16:Resource Include:resource.h
# 	2:37:CLASS: CCompensationViewLimitMoneyDlg:CCompensationViewLimitMoneyDlg
# 	2:38:IDD_DIALOG_COMPENSATION_VIEWLIMITMONEY:IDD_DIALOG_COMPENSATION_VIEWLIMITMONEY
# 	2:10:ENUM: enum:enum
# 	2:33:CompensationViewLimitMoneyDlg.cpp:CompensationViewLimitMoneyDlg.cpp
# 	2:19:Application Include:client.h
# 	2:31:CompensationViewLimitMoneyDlg.h:CompensationViewLimitMoneyDlg.h
# End Section
# Section Client : {ED580058-CA84-4407-B3C3-2F8FD9B0740D}
# 	1:21:IDD_DIALOG_FAMILY_END:119
# 	2:20:CLASS: CFamilyEndDlg:CFamilyEndDlg
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:16:FamilyEndDlg.cpp:FamilyEndDlg.cpp
# 	2:19:Application Include:client.h
# 	2:14:FamilyEndDlg.h:FamilyEndDlg.h
# 	2:21:IDD_DIALOG_FAMILY_END:IDD_DIALOG_FAMILY_END
# End Section
# Section Client : {4BD47DFD-57D2-426F-B966-4BBCB48AAB0D}
# 	1:27:IDD_DIALOG_EVENTSEASONEVENT:118
# 	2:16:Resource Include:resource.h
# 	2:23:EventSeasonEventDlg.cpp:EventSeasonEventDlg.cpp
# 	2:27:IDD_DIALOG_EVENTSEASONEVENT:IDD_DIALOG_EVENTSEASONEVENT
# 	2:27:CLASS: CEventSeasonEventDlg:CEventSeasonEventDlg
# 	2:10:ENUM: enum:enum
# 	2:21:EventSeasonEventDlg.h:EventSeasonEventDlg.h
# 	2:19:Application Include:client.h
# End Section
# Section Client : {BF29F704-3551-4B3A-99EF-40D88184A513}
# 	2:26:CLASS: CInviteUserListView:CInviteUserListView
# 	2:21:CLASS: SMALLUSERINFO1:SMALLUSERINFO1
# 	2:20:InviteUserListView.h:InviteUserListView.h
# 	2:22:InviteUserListView.cpp:InviteUserListView.cpp
# 	2:19:Application Include:client.h
# 	2:23:TYPEDEF: SMALLUSERINFO1:SMALLUSERINFO1
# End Section
# Section Client : {16F0E234-580D-446F-9824-CEA0829E77BF}
# 	1:16:IDD_DLG_ASKAGAIN:145
# 	2:16:Resource Include:resource.h
# 	2:19:CLASS: CAskAgainDlg:CAskAgainDlg
# 	2:13:AskAgainDlg.h:AskAgainDlg1.h
# 	2:16:IDD_DLG_ASKAGAIN:IDD_DLG_ASKAGAIN
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:client.h
# 	2:15:AskAgainDlg.cpp:AskAgainDlg1.cpp
# End Section
# Section Client : {4C84107B-2B2B-48D2-884A-DCD08057888A}
# 	1:19:IDD_CONSPIRE_DIALOG:159
# 	2:16:Resource Include:resource.h
# 	2:19:IDD_CONSPIRE_DIALOG:IDD_CONSPIRE_DIALOG
# 	2:23:Baduser_ConspireDlg.cpp:Baduser_ConspireDlg.cpp
# 	2:27:CLASS: CBaduser_ConspireDlg:CBaduser_ConspireDlg
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:client.h
# 	2:21:Baduser_ConspireDlg.h:Baduser_ConspireDlg.h
# End Section
# Section Client : {35195A83-A7C5-4305-9AF2-743A0F48DEE0}
# 	1:24:IDD_DIALOG_FAMILY_SELECT:153
# 	2:16:Resource Include:resource.h
# 	2:24:IDD_DIALOG_FAMILY_SELECT:IDD_DIALOG_FAMILY_SELECT
# 	2:10:ENUM: enum:enum
# 	2:17:FamilySelectDlg.h:FamilySelectDlg.h
# 	2:23:CLASS: CFamilySelectDlg:CFamilySelectDlg
# 	2:19:FamilySelectDlg.cpp:FamilySelectDlg.cpp
# 	2:19:Application Include:client.h
# End Section
# Section Client : {0C0C7EA6-8635-4291-BCE9-78FABDA92D26}
# 	1:23:IDD_DIALOG_COMPENSATION:157
# 	2:16:Resource Include:resource.h
# 	2:23:IDD_DIALOG_COMPENSATION:IDD_DIALOG_COMPENSATION
# 	2:29:CLASS: CFamilyCompensationDlg:CFamilyCompensationDlg
# 	2:25:FamilyCompensationDlg.cpp:FamilyCompensationDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:23:FamilyCompensationDlg.h:FamilyCompensationDlg.h
# 	2:19:Application Include:client.h
# End Section
# Section Client : {90C7DAB1-B855-454C-BE79-89D9996E9885}
# 	1:23:IDD_DIALOG_ASKSUBSCRIBE:146
# 	2:16:Resource Include:resource.h
# 	2:23:CLASS: CAskSubscribeDlg:CAskSubscribeDlg
# 	2:19:AskSubscribeDlg.cpp:AskSubscribeDlg1.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:ENUM: CONTENTS_KIND:CONTENTS_KIND
# 	2:19:Application Include:client.h
# 	2:23:IDD_DIALOG_ASKSUBSCRIBE:IDD_DIALOG_ASKSUBSCRIBE
# 	2:17:AskSubscribeDlg.h:AskSubscribeDlg1.h
# End Section
# Section Client : {2178BC15-D29C-4F95-9496-813E32114F4A}
# 	1:26:IDD_DIALOG_RECOVERY_CHARGE:143
# 	2:16:Resource Include:resource.h
# 	2:19:RecoveryChargeDlg.h:RecoveryChargeDlg.h
# 	2:10:ENUM: enum:enum
# 	2:25:CLASS: CRecoveryChargeDlg:CRecoveryChargeDlg
# 	2:26:IDD_DIALOG_RECOVERY_CHARGE:IDD_DIALOG_RECOVERY_CHARGE
# 	2:21:RecoveryChargeDlg.cpp:RecoveryChargeDlg.cpp
# 	2:19:Application Include:client.h
# End Section
# Section Client : {6C6EB74D-267E-445F-BDF7-6202B2D9462D}
# 	1:23:IDD_DIALOG_HOTKEYSURVEY:161
# 	2:16:Resource Include:resource.h
# 	2:23:CLASS: CHotkeySurveyDlg:CHotkeySurveyDlg
# 	2:23:IDD_DIALOG_HOTKEYSURVEY:IDD_DIALOG_HOTKEYSURVEY
# 	2:10:ENUM: enum:enum
# 	2:17:HotkeySurveyDlg.h:HotkeySurveyDlg.h
# 	2:19:Application Include:client.h
# 	2:19:HotkeySurveyDlg.cpp:HotkeySurveyDlg.cpp
# End Section
# Section Client : {4B84CA53-1853-44CD-A907-2CFEF02A05B6}
# 	1:19:IDD_OWNLIMIT_DIALOG:165
# 	2:16:Resource Include:resource.h
# 	2:19:CLASS: COwnLimitDlg:COwnLimitDlg
# 	2:15:OwnLimitDlg.cpp:OwnLimitDlg.cpp
# 	2:16:ENUM: HANDOPOPUP:HANDOPOPUP
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:client.h
# 	2:13:OwnLimitDlg.h:OwnLimitDlg.h
# 	2:19:IDD_OWNLIMIT_DIALOG:IDD_OWNLIMIT_DIALOG
# End Section
# Section Client : {61E09F34-63C3-4B7F-8C3C-E02A0BD04D93}
# 	2:20:TYPEDEF: ITEMANIDATA:ITEMANIDATA
# 	2:11:Animation.h:Animation.h
# 	2:20:ENUM: ANIMATION_LIST:ANIMATION_LIST
# 	2:18:CLASS: ITEMANIDATA:ITEMANIDATA
# 	2:19:Application Include:client.h
# 	2:13:Animation.cpp:Animation.cpp
# 	2:17:CLASS: CAnimation:CAnimation
# End Section
# Section Client : {4BC8E56A-1D18-475A-8835-A5B24D6566E2}
# 	1:23:IDD_DIALOG_NEWUSEREVENT:126
# 	2:16:Resource Include:resource.h
# 	2:17:NewUserEventDlg.h:NewUserEventDlg.h
# 	2:23:IDD_DIALOG_NEWUSEREVENT:IDD_DIALOG_NEWUSEREVENT
# 	2:10:ENUM: enum:enum
# 	2:19:NewUserEventDlg.cpp:NewUserEventDlg.cpp
# 	2:23:CLASS: CNewUserEventDlg:CNewUserEventDlg
# 	2:19:Application Include:client.h
# End Section
# Section Client : {C143498F-9BF6-4399-9B96-38814203BBEE}
# 	1:17:IDD_BANNER_DIALOG:127
# 	2:16:Resource Include:resource.h
# 	2:17:CLASS: CBannerDlg:CBannerDlg
# 	2:17:IDD_BANNER_DIALOG:IDD_BANNER_DIALOG
# 	2:10:ENUM: enum:enum
# 	2:11:BannerDlg.h:BannerDlg.h
# 	2:19:Application Include:client.h
# 	2:13:BannerDlg.cpp:BannerDlg.cpp
# End Section
# Section Client : {92C94BE4-E636-48BF-B4C2-2FD56C006C1A}
# 	1:27:IDD_DIALOG_RESTORE_CUTMONEY:144
# 	2:16:Resource Include:resource.h
# 	2:26:CLASS: CRestoreCutMoneyDlg:CRestoreCutMoneyDlg
# 	2:20:RestoreCutMoneyDlg.h:RestoreCutMoneyDlg1.h
# 	2:10:ENUM: enum:enum
# 	2:27:IDD_DIALOG_RESTORE_CUTMONEY:IDD_DIALOG_RESTORE_CUTMONEY
# 	2:19:Application Include:client.h
# 	2:22:RestoreCutMoneyDlg.cpp:RestoreCutMoneyDlg1.cpp
# End Section
# Section Client : {09A44A54-1DDC-4F76-B844-0BB851560B32}
# 	1:25:IDD_DIALOG_BUYSUPERAVATAR:108
# 	2:16:Resource Include:resource.h
# 	2:25:IDD_DIALOG_BUYSUPERAVATAR:IDD_DIALOG_BUYSUPERAVATAR
# 	2:21:BuySuperAvatarDlg.cpp:BuySuperAvatarDlg.cpp
# 	2:25:CLASS: CBuySuperAvatarDlg:CBuySuperAvatarDlg
# 	2:19:BuySuperAvatarDlg.h:BuySuperAvatarDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:client.h
# End Section
# Section Client : {6ED464F8-E9C8-40A7-8B68-7A743CF19C0C}
# 	1:20:IDD_ADMIN_SETTINGDLG:154
# 	2:16:Resource Include:resource.h
# 	2:17:AdminSettingDlg.h:AdminSettingDlg.h
# 	2:10:ENUM: enum:enum
# 	2:20:IDD_ADMIN_SETTINGDLG:IDD_ADMIN_SETTINGDLG
# 	2:19:AdminSettingDlg.cpp:AdminSettingDlg.cpp
# 	2:23:CLASS: CAdminSettingDlg:CAdminSettingDlg
# 	2:19:Application Include:client.h
# End Section
# Section Client : {3F570B39-E6A3-4D95-B5D5-DE481EBCB166}
# 	1:25:IDD_BADUSER_REPORT_DIALOG:115
# 	2:16:Resource Include:resource.h
# 	2:25:IDD_BADUSER_REPORT_DIALOG:IDD_BADUSER_REPORT_DIALOG
# 	2:19:Baduser_Reportdlg.h:Baduser_Reportdlg.h
# 	2:25:CLASS: CBaduser_Reportdlg:CBaduser_Reportdlg
# 	2:10:ENUM: enum:enum
# 	2:21:Baduser_Reportdlg.cpp:Baduser_Reportdlg.cpp
# 	2:19:Application Include:client.h
# End Section
