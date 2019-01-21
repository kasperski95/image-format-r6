@echo off
IF NOT EXIST "output" ( mkdir "output")
echo BMP to R6 to BMP
echo ============================================================
echo DEDICATED PALETTE
r6.exe source.bmp output/dedicated-palette.r6
r6.exe output/dedicated-palette.r6 output/dedicated-palette.bmp
echo ------------------------------------------------------------
echo FIXED PALETTE
r6.exe source.bmp output/fixed-palette.r6 -m fixed
r6.exe output/fixed-palette.r6 output/fixed-palette.bmp
echo ------------------------------------------------------------
echo GRAYSCALE
r6.exe source.bmp output/grayscale.r6 -m grayscale
r6.exe output/grayscale.r6 output/grayscale.bmp
echo ------------------------------------------------------------
echo DEDICATED PALETTE + DITHERING
r6.exe source.bmp output/dedicated-palette-dithering.r6 -d
r6.exe output/dedicated-palette-dithering.r6 output/dedicated-palette-dithering.bmp
echo ------------------------------------------------------------
echo FIXED PALETTE + DITHERING
r6.exe source.bmp output/fixed-palette-dithering.r6 -m fixed -d
r6.exe output/fixed-palette-dithering.r6 output/fixed-palette-dithering.bmp
echo ------------------------------------------------------------
echo GRAYSCALE + DITHERING
r6.exe source.bmp output/grayscale-dithering.r6 -m grayscale -d
r6.exe output/grayscale-dithering.r6 output/grayscale-dithering.bmp
pause