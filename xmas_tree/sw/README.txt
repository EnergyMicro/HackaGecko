This project should be built with full speed optimization.


The project in this application note expects certain resources to be available at 
specific locations in the folder hierachy. Please make sure to install this 
application note at the correct place, normally (Simplicity Studio paths): 

For win7:
C:\Users\'USERNAME'\AppData\Roaming\energymicro\an

For winXP:
C:\Documents and Settings\'USERNAME'\Application Data\energymicro\an

The resulting path for a given application note will be:

....\energymicro\an\<application_note_name>


The surrounding hierarchy must be as follows:

root_folder (e.g. C:\Documents and Settings\'USERNAME'\Application Data\energymicro\)
          |
          -->an
          |   |
          |   --> <application_note_name>
          |
          -->CMSIS
          |
          |
          -->efm32lib
          |
          |
          -->kits
         
The other resources can be obtained through Simplicity Studio
or downloaded from the Energy Micro webpage: 