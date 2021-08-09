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

   You'll need the following for the VBCC (use the appropriate paths):

   fd2pragma fd/playsid_lib.fd CLIB clib/playsid_protos.h TO inline SPECIAL 70
   fd2pragma fd/playsid_lib.fd CLIB clib/playsid_protos.h TO proto SPECIAL 38

   I've used AmiKIT's DevPack environment with VBCC.

   Command line for VBCC:
   vc -lamiga PlaySIDFile-VBCC.c -o PlaySIDFile-VBCC

   You'll get a lot of warnings from the compiler, but you'll get the executable. :-)

   
   NOTICE!!

   I noticed, that this program nor does the PlaySID3.0 handle the samples in SID songs.
   This may be an emulation issue, though. But "normal" SIDs should be ok.


   I'll try to find the time to improve this code...

*/


#include <exec/types.h>

#include <dos/stdio.h>
#include <proto/dos.h>

#include <libraries/playsidbase.h>
#include <clib/playsid_protos.h>
#include <proto/playsid.h>
#include <inline/playsid_protos.h>



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
