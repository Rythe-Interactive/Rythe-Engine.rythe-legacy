@Echo off
Setlocal EnableDelayedExpansion

Echo Setting local Git settings.
cd ..
git config --local core.autocrlf input

cd tools
Echo Git settings set to:
Echo 	core.autocrlf=input

:Prompt
Echo Adding file headers...
"./lgn_cleanup/exe/lgn_cleanup" "../legion" -ex="**/glm/" -ex="**/glad/" -ex="**/ini.c" -ex="**/imgui_impl/" -ex="**/KHR/" -ex="**/delete_this"

Echo Converting legion files...
Call :convertDir ../legion

Echo Finished converting legion files.
Echo Applications will be next.

Echo Converting applications files...
Call :convertDir ../applications

Echo Finished converting all files.
:End
pause
Endlocal
@Echo on
@exit /b

:convertDir
	sfk197 detab=4 %~1 .c .cpp .h .hpp .inl -yes
	sfk197 crlf-to-lf %~1 .c .cpp .h .hpp .inl -yes