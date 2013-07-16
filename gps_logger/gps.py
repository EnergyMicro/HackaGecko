#!/usr/bin/python

#**************************************************************************//**
 # @file gps.py
 # @brief GPS-logger
 # @author Energy Micro AS
 # @version 1.0.2
 #*****************************************************************************
 # @section License
 # <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 #******************************************************************************
 #
 # Permission is granted to anyone to use this software for any purpose,
 # including commercial applications, and to alter it and redistribute it
 # freely, subject to the following restrictions:
 #
 # 1. The origin of this software must not be misrepresented; you must not
 #    claim that you wrote the original software.
 # 2. Altered source versions must be plainly marked as such, and must not be
 #    misrepresented as being the original software.
 # 3. This notice may not be removed or altered from any source distribution.
 # 4. The source and compiled code may only be used on Energy Micro "EFM32"
 #    microcontrollers and "EFR4" radios.
 #
 # DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 # obligation to support this Software. Energy Micro AS is providing the
 # Software "AS IS", with no express or implied warranties of any kind,
 # including, but not limited to, any implied warranties of merchantability
 # or fitness for any particular purpose or warranties against infringement
 # of any proprietary rights of a third party.
 #
 # Energy Micro AS will not be liable for any consequential, incidental, or
 # special damages, or any other relief, or for any claim by any third party,
 # arising from your use of this Software.
 #
#*****************************************************************************/


import sys
import math


def main():
    r"""gps.py <input file> <output file>
where <input file> is a file created by gps module (on sdcard - GPS.TXT) and output file is a .kml file which could be imported  into Google Earth/Maps"""

    if not (len(sys.argv) == 3):
        print 'Usage: gps.py data-file output-file'
    else:
        gpsFilePath = sys.argv[1]
        gpsFile = open(gpsFilePath, 'r')
        head = '''<?xml version="1.0" encoding="UTF-8"?>
        <kml xmlns="http://www.opengis.net/kml/2.2">
        <Document><name>My document</name>
        <description>Content</description>
        <Style id="Lump">
        <LineStyle><color>CD0000FF</color><width>0.5</width></LineStyle>
        <PolyStyle><color>9AFF0000</color></PolyStyle>
        </Style>
        <Style id="Path">
        <LineStyle><color>FF0000FF</color><width>3</width></LineStyle>
        </Style>
        <Style id="markerstyle">
        <IconStyle><Icon><href>
        http://maps.google.com/intl/en_us/mapfiles/ms/micons/red-dot.png
        </href></Icon></IconStyle>
        </Style>
        <Placemark><name>Geckon</name>
        <description>hack-a-gecko gps logger</description>
        <styleUrl>#Path</styleUrl>
        <LineString>
        <tessellate>1</tessellate>
        <altitudeMode>clampToGround</altitudeMode>
        <coordinates>'''

        tail = '''</coordinates>
        </LineString>
        </Placemark>
        </Document>
        </kml>'''

        for line in gpsFile:
            list = line.split(',')
            if list[0] == '$GPGGA' and len(list) > 4 and list[4] and list[2]:
                l1 = math.floor(float(list[4])/100)+(float(list[4])/100-math.floor(float(list[4])/100))*100/60
                l2 = math.floor(float(list[2])/100)+(float(list[2])/100-math.floor(float(list[2])/100))*100/60
                var1 = str(l1)
                var2 = str(l2)
                head += var1 + ',' + var2 + '\n'

        head += tail
        gpsFile.close()
        gpsFile = open(sys.argv[2], 'w')
        gpsFile.write(head)
        gpsFile.close()

        print 'Open '+sys.argv[2]+' file in Google Earth/Google Maps or any .kml supported software to see gps route'

main()
