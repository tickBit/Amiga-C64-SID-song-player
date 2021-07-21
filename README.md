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

NOTICE!!

I noticed, that this program nor does the PlaySID3.0 handle
C64 songs, that have digitzed instruments!

Back in the day I paid the shareware fee for DICE C compiler.
With DICE I'm able to compile this project, but I've not managed
to compile it with VBCC in my Amiga emulation setup yet..
I hope you will have better luck!

I'll try to find the time to improve this code...

Example picture of the program's output:

![GitHub-PlaySidFile](https://user-images.githubusercontent.com/61118857/118642778-6d5a6800-b7e4-11eb-90c5-4cf7a8843e82.jpg)
