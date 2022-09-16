@echo off

pushd bin
cl.exe /nologo /Zi /GR- /EHa- /Odi /FC /MT ../src/app.c SDL2.lib SDL2main.lib SDL2_ttf.lib SDL2_image.lib /link /incremental:no /DLL /out:library.dll
cl.exe /nologo /Zi /MT ../src/main.c user32.lib SDL2.lib SDL2main.lib /link /incremental:no /out:win32_demo.exe
popd
