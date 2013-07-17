GPS Logger example.

This project uses EFM32 STK3700 board with a GPS module to gather location data on SD card.

The easiest way to build the IAR project is place the inner gps_module folder into your kits examples project (eg. C:\Users\<USERNAME>\AppData\Roaming\energymicro\kits\EFM32GG_STK3700\examples\).

If you want to run this project from other location you need to change paths to libraries:
 - BSP
 - CMSIS
 - Drivers of the STK you use (FatFS, LCD)
 - emlib
All necessary packages are included in your Simplicity Studio. (eg. C:\Users\<USERNAME>\AppData\Roaming\energymicro\)



Python script usage:
gps.py <input file> <output file>
where <input file> is a file created by gps module (on sdcard - GPS.TXT) and output file is a .kml file which could be imported into Google Earth/Maps



Board:  Energy Micro EFM32GG_STK3700 Starter Kit
Device: EFM32GG990F1024
