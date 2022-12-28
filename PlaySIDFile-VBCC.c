/* You'll need PlaySID3.lha archive to use this code.
   The archive can be found in the Aminet:
   https://aminet.net/package/mus/play/PlaySID3

   This program uses the playsid.library and the include files
   that come with the archive.

   Furthermore, in order to play the C64's SID files,
   you'll need to use the SIDConv tool of PlaySID3 archive to
   convert the .sid files into two (2) files like this:

   SIDConv yoursidfile.sid yoursidfile ICON

   More info in the PlaySID3 archive.

   NDK3.9 os include files:
   http://www.haage-partner.de/download/AmigaOS/NDK39.lha

   In my Amiga working environemnt I have set up the environment for VBCC as follows:
   
   assign VBCC: Work:Dev/DevPack/vbcc
   assign vinclude3: VBCC:targets/m68k-amigaos/include
   assign vinclude3: Work:Dev/DevPack/SDK/NDK_3.9/include/include_h ADD
   assign vlibos3: VBCC:targets/m68k-amigaos/lib
   
   path VBCC:bin
   alias vbcc vc
   
   The include files from PlaySID3.lha are in the root, where I have this code.
   I have copied from PlaySID3.0/fd/playsid_lib.h to this root directory's include/libraries
      
   Hope this helps others too! :-)

   Command line for VBCC:
   vc -lamiga PlaySIDFile-VBCC.c -o PlaySIDFile-VBCC

   You'll get a lot of warnings from the compiler, but you'll get the executable. :-)

   
   PlaySID3 can handle C64's SID songs with digitized instruments, but the format of the
   .sid files must be PSID. The last High Voltage SID Collection that supported PSID files
   was #49. At the time of this writing, #49 is still available.


   I'll try to find the time to improve this code...

*/

#include <exec/types.h>

#include <dos/stdio.h>
#include <proto/dos.h>

#include "include/libraries/playsid_lib.h"
#include "include/libraries/playsidbase.h"
#include "include/clib/playsid_protos.h"
#include "include/proto/playsid.h"



struct PlaySidBase *PlaySidBase=NULL;
void cleanexit(void);

APTR sidfile = NULL;
LONG emulrc = -1;
APTR header = NULL;
BPTR fh = NULL;



int main(int argc, char *argv[]) {
  
    BOOL channel[4];
    
    LONG ri;
    LONG cm;
    int fileread = 0;
    
    UWORD tune = 0;
    UWORD size = 0;

    if (argc == 1) {
        printf("Usage: PlaySIDfile-VBCC <file> <tuneNr>\n");
        exit(0);
    }

    if (argc >= 2) {
        /* check if the filename ends with .sid suffix */
        if (strlen(argv[1]) > 4) {
            if (strcmp(argv[1] + strlen(argv[1]) - 4, ".sid") == 0) {
                printf("The filename must not end with .sid suffix!\nUse the SIDConv tool to convert the .sid file.\n");
                exit(0);
            }
        }
    }

    if (argc >= 3) tune = atoi(argv[2]); else tune = 0;

 
    /* allocate memory for the SID header; size == sizeof(SIDHeader) */

    header = (APTR)malloc(sizeof(SIDHeader));
 

    if (header == NULL) cleanexit();

    /* the channels */
    channel[0] = TRUE;
    channel[1] = TRUE;
    channel[2] = TRUE;
    channel[3] = TRUE;

    if ((PlaySidBase=(struct PlaySidBase *) OpenLibrary("playsid.library",0))==NULL) {
      printf("Could not open playsid.library.\n");
      cleanexit();
    }

    printf("playsid.library opened!\n");

    fh = Open(argv[1],MODE_OLDFILE);

    if (fh) {
        /* get file size */
        Seek(fh,0,OFFSET_END);
        size = Seek(fh, 0, OFFSET_BEGINNING);

        /* Allocate emulation resources */
        emulrc = AllocEmulResource();

        printf("Emulation resources error code: %i\n",emulrc);
        if (emulrc == 0) {
            printf("Emulation resources allocated!\n");
        }
        
        /* filename without .info suffix */
        ri = ReadIcon(argv[1], header);

        cm = CheckModule(header);
        
        /* is file OK? */
        if (cm == 0) {

            printf("File header OK.\n");

            sidfile = (APTR)malloc(size);

            if (sidfile == NULL) cleanexit();

            /* read SID data into memory */
            fileread = Read(fh, (APTR)sidfile, size);

            /* APTR header, APTR file location, UWORD size*/
            SetModule(header, sidfile, size);

            printf("Memory allocated for the SID data and module set.\n");

            SetVertFreq(50);              /* PAL */
            SetChannelEnable(channel);    /* channels */
            
            StartSong(tune);

            printf("\n");
            printf("SONG: %s\n",(char *)(header)+22);
            printf("AUTHOR: %s\n",(char *)(header)+54);
            printf("COPYRIGHT: %s\n",(char *)(header)+86);
        
        } else {
            printf("Icon file not OK.\n");
            cleanexit();
        }

        printf("\nPress return / enter to exit.\n");

        getchar();  /* wait for char input */

        StopSong();

        cleanexit();
    }

    return 0;
}

void cleanexit(void) {
    if (emulrc == 0) FreeEmulResource();
    if (fh) Close(fh);
    if (header != NULL) free(header);
    if (sidfile != NULL) free(sidfile);
	if (PlaySidBase) CloseLibrary((struct Library *)PlaySidBase);


}
