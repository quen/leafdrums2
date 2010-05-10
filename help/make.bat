@echo off
rem Doesn't work in 1.5 (the running HHC bit) and I don't know why!
"C:\Program Files\Java\j2re1.4.2\bin\java" -cp "C:\Documents and Settings\Samuel Marshall\My Documents\Java projects\helpsystem\classes" helpsystem.Main ld-help.xml content output ..\leafDrums2.chm
pause