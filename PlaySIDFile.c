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

   NOTICE!!

   I noticed, that this program nor does the PlaySID3.0 handle
   C64 songs, that have digitzed instruments!


   I'll try to find the time to improve this code...

*/

#include <libraries/playsidbase.h>
#include <clib/playsid_protos.h>
#include <proto/playsid.h>
#include <pragmas/playsid_pragmas.h>
#include <pragma/playsid_lib.h> /* Please notice, that you'll need some fd2pragma program to create this header! */
#include <proto/dos.h>
#include <stdio.h>


struct PlaySidBase *PlaySidBase=NULL;


int main(int argc, char *argv[]) {

    if (argc == 1) {
        printf("Usage: PlaySIDfile file tuneNr\n");
        exit(0);
    }

    UWORD tune;

    if (argc >= 3) tune = atoi(argv[2]); else tune = 0;

    APTR sidfile = NULL;
    LONG emulrc = -1;
    LONG ri;
    APTR header = NULL;

    /* allocate memory for the SID header; size == sizeof(SIDHeader) */

    header = (APTR)malloc(sizeof(SIDHeader));
    BPTR fh; 
    LONG cm;

    if (header == NULL) goto cleanexit;

    /* the channels */
    BOOL channel[4];
    channel[0] = TRUE;
    channel[1] = TRUE;
    channel[2] = TRUE;
    channel[3] = TRUE;

    if ((PlaySidBase=(struct PlaySidBase *) OpenLibrary("playsid.library",0))==NULL) {
      printf("Could not open playsid.library.\n");
      goto cleanexit;
    }

    printf("playsid.library opened!\n");

    fh = Open(argv[1],MODE_OLDFILE);

    if (fh) {
        /* get file size */
        Seek(fh,0,OFFSET_END);
        UWORD size = Seek(fh, 0, OFFSET_BEGINNING);

        /* Allocate emulation resources */
        emulrc = AllocEmulResource();

        /* filename without .info suffix */
        ri = ReadIcon(argv[1], header);

        cm = CheckModule(header);
        
        /* is file OK? */
        if (cm == 0) {

            printf("File header OK.\n");

            APTR sidfile = NULL;
            sidfile = (APTR)malloc(size);

            if (sidfile == NULL) goto cleanexit;

            /* read SID data into memory */
            int fileread = Read(fh, (APTR)sidfile, size);

            /* if reading the file failed, goto cleanexit */
            if (fileread < 0) goto cleanexit;

            /* APTR header, APTR file location, UWORD size*/
            SetModule(header, sidfile, size);

            printf("Memory allocated for the SID data and module set.\n");
            
            SetVertFreq(50);              /* PAL */
            SetChannelEnable(channel);    /* channels */
            SetReverseEnable(FALSE);

            StartSong(tune);

            printf("\n");
            printf("SONG: %s\n",(char *)(header)+22);
            printf("AUTHOR: %s\n",(char *)(header)+54);
            printf("COPYRIGHT: %s\n",(char *)(header)+86);
            
        } else {
            printf("Icon file not OK.\n");
            goto cleanexit;
        }

        printf("\nPress return / enter to exit.\n");

        getchar();  /* wait for char input */

        StopSong();

    }

cleanexit:
    if (emulrc == 0) FreeEmulResource();
    if (fh) Close(fh);
    if (header != NULL) free(header);
    if (sidfile != NULL) free(sidfile);
	if (PlaySidBase) CloseLibrary((struct Library *)PlaySidBase);

    return 0;

}