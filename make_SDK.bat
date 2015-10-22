copy ".\Release\testPlay.exe" ".\RTSPFFmpegSDK\x86\bin"
copy ".\Release\RTSPFFmpeg.dll" ".\RTSPFFmpegSDK\x86\bin"
copy ".\Release\PlayH264DLL.dll" ".\RTSPFFmpegSDK\x86\bin"

copy ".\Release\PlayH264DLL.lib" ".\RTSPFFmpegSDK\x86\lib"
copy ".\Release\RTSPFFmpeg.lib" ".\RTSPFFmpegSDK\x86\lib"

copy ".\x64\Release\testPlay.exe" ".\RTSPFFmpegSDK\x64\bin"
copy ".\x64\Release\RTSPFFmpeg.dll" ".\RTSPFFmpegSDK\x64\bin"
copy ".\x64\Release\PlayH264DLL.dll" ".\RTSPFFmpegSDK\x64\bin"

copy ".\x64\Release\PlayH264DLL.lib" ".\RTSPFFmpegSDK\x64\lib"
copy ".\x64\Release\RTSPFFmpeg.lib" ".\RTSPFFmpegSDK\x64\lib"

copy ".\RTSPFFmpeg\RTSPFFmpeg.h" ".\RTSPFFmpegSDK\include"
copy ".\PlayH264Dll\PlayH264DLL.h" ".\RTSPFFmpegSDK\include"

copy ".\PlayH264Dll.docx" ".\RTSPFFmpegSDK\doc"
copy ".\RTSPFFmpeg.docx" ".\RTSPFFmpegSDK\doc"

copy ".\testPlay\*.h" ".\RTSPFFmpegSDK\doc\example\testPlay"
copy ".\testPlay\*.cpp" ".\RTSPFFmpegSDK\doc\example\testPlay"
copy ".\testPlay\testPlay.rc" ".\RTSPFFmpegSDK\doc\example\testPlay"
copy ".\testPlay\testPlay.vcxproj" ".\RTSPFFmpegSDK\doc\example\testPlay"
copy ".\testPlay\testPlay.vcxproj.filters" ".\RTSPFFmpegSDK\doc\example\testPlay"

copy ".\testPlay\res\*.*" ".\RTSPFFmpegSDK\doc\example\testPlay\res"