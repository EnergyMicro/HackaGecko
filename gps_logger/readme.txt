GPS Logger example.

This project uses EFM32 with a GPS module to gather location data on SD card.

The best way to build the IAR project is to base it on the lcd example from Simplicity Studio, EFM32GG_STK3700 kit. 


Python script usage:
gps.py <input file> <output file>
where <input file> is a file created by gps module (on sdcard - GPS.TXT) and output file is a .kml file which could be imported into Google Earth/Maps



Board:  Energy Micro EFM32GG_STK3700 Starter Kit
Device: EFM32GG990F1024
