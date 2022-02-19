# Amiga-C64-SID-song-player

This plays C64's SID songs on the Amiga with playsid.library

You'll need PlaySID3.lha archive to use this code.
The archive can be found in the Aminet:
https://aminet.net/package/mus/play/PlaySID3

This program uses the playsid.library and the include files
that come with the archive. Please notice, that you'll need some
fd2pragma program, too.

Furthermore, in order to play the C64's SID files,
you'll need to use the SIDConv tool of PlaySID3 archive to
convert the .sid files into two (2) files like this:

SIDConv yoursidfile.sid yoursidfile ICON

More info in the PlaySID3 archive.

NOTICE -- CORRECTION !!

PlaySID3.0 can handle C64 songs, that have digitized instruments! The problem is, the last High Voltage Sid Collection that supported the PSID format was HVSC #49. Fortunally, it is still availabe. The later HVSC's use RSID as format. PlaySID3.0's SIDConv seems to handle "normal" SID songs with that later format, but not those songs that have samples...

I should put this correction notice inside the codes, too.

* PlaySIDFile.c can be compiled without problems at least with DICE.
* PlaySIDFile-VBCC.c can be compiled (with instructions in the source) at least with VBCC.

I'll try to find the time to improve this code...

Example picture of the program's output (C-version):

![GitHub-PlaySidFile](https://user-images.githubusercontent.com/61118857/118642778-6d5a6800-b7e4-11eb-90c5-4cf7a8843e82.jpg)

The assembly version is quite unfinished yet: It doesn't check, that absolutely everything is OK.
Also, the filename parameter in the assembly version must not contain any trailing spaces.
The number of tune to be played must be set in the code in the assembly version..
But you'll get started with the assembly version to play SID tunes on the Amiga...
CTRL+C to exit the assembly version.
