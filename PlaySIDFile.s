; It's been long time since my active Amiga days.. so.. forgive... :-)

; This can be compiled at least with PhxAss assembler.
; I hope it can be still found in the Aminet..

; NOTICE! This version is still unfinished: It doesn't make every check if everything is OK,
; but you'll get started playing SID files converted with SIDConv in your own programs

; You will need PlaySID3.0.lha archive to get the needed include files
; https://aminet.net/package/mus/play/PlaySID3

; Also, please notice that after the given filename for the program, there must not be any spaces!

; CTRL+C to exit the program

; Amiga Developer CD v1.1 can be freely downloaded from the internet
; Use this instead, if you don't have ADCD_2.1

;               incdir   "Amiga_Dev_CD_v1.1:NDK_3.1/includes&libs/include_i"

                incdir  "ADCD_2.1:NDK/NDK_3.5/Include/include_i"
                include "dos/dos.i"                
                include "exec/exec.i"

; arrange these as you wish

                include "include/libraries/playsid_lib.i"
                include "include/libraries/playsidbase.i"

    section CODE,code

                movem.l d2-d7/a2-a6,-(sp)
                move.l  a0,cmdbuff
                clr.b   -1(a0,d0.l)         ; add 0 to end of filename
                move.l  d0,d7

                lea     dosname,a1
                moveq   #0,d0
                move.l  4,a6
                jsr     -$0228(a6)          ; Exec/OpenLibrary
                move.l  d0,dosbase
                beq     cleanup
   
                move.l  dosbase,a6
                jsr     -$003c(a6)          ; Dos/Output
                move.l  d0,outfile


                move.l  cmdbuff,a0
                tst.b   (a0)
                bne.s   name_not_null

                lea     text1,a0
                bsr     print
                bra     cleanup

name_not_null
                lea     sidname,a1
                moveq   #0,d0
                move.l  4,a6                
                jsr     -$0228(a6)          ; Exec/OpenLibrary
                move.l  d0,sidbase
                beq     cleanup

                move.l  cmdbuff,d1
                move.l  #1005,d2
                move.l  dosbase,a6
                jsr     -$001e(a6)          ; Dos/Open
                move.l  d0,fh
                beq    cleanup

                move.l  fh,d1
                move.l  #0,d2
                move.l  #OFFSET_END,d3
                move.l  dosbase,a6
                jsr     -$0042(a6)                  ; Dos/Seek

                move.l  fh,d1
                move.l  #0,d2
                move.l  #OFFSET_BEGINNING,d3
                move.l  dosbase,a6
                jsr     -$0042(a6)                  ; Dos/Seek
                move.w  d0,size

                move.l  sidbase,a6
                jsr     _LVOAllocEmulResource(a6)
                move.l  d0,emulrc

                move.l  #32*4,d0
                move.l  #1,d1
                move.l  4,a6
                jsr     -$00c6(a6)          ; Exec/AllocMem
                move.l  d0,header
                beq     cleanup

              
                move.l  cmdbuff,a0
                move.l  header,a1
                move.l  sidbase,a6
                jsr     _LVOReadIcon(a6)
                move.l  d0,ri


                move.l  header,a1
                move.l  sidbase,a6
                jsr    _LVOCheckModule(a6)
                move.l d0,cm


                moveq   #0,d0
                move.w  size,d0
                move.l  #1,d1
                move.l  4,a6
                jsr     -$00c6(a6)          ; Exec/AllocMem
                move.l  d0,sidfile
                beq     cleanup


                move.l  fh,d1
                move.l  sidfile,d2              
                moveq   #0,d3
                move.w  size,d3
                move.l  dosbase,a6
                jsr     -$002a(a6)          ; Dos/Read
                move.l  d0,fileread
                beq     cleanup


                move.l  sidbase,a6
                move.l  header,a0
                move.l  sidfile,a1
                moveq   #0,d0
                move.w  size,d0
                jsr     _LVOSetModule(a6)

               

                moveq   #0,d0
                move.w  #50,d0
                jsr     _LVOSetVertFreq(a6)

                lea     channels,a1
                jsr     _LVOSetChannelEnable(a6)

                moveq   #0,d0       ; nr of tune to be played
                jsr     _LVOStartSong(a6)


                move.l  4,a6
                move.l  #$1000,d0               ; signal on CTRL-C
                jsr     -$013e(a6)              ; Exec/Wait 

stop_music      move.l  sidbase,a6
                jsr     _LVOStopSong(a6)

cleanup         move.l  emulrc,d0
                cmp.l   #-1,d0
                beq.s   nxt1
                move.l  sidbase,a6
                jsr     _LVOFreeEmulResource(a6)

nxt1            move.l  fh,d0
                beq     nxt2         
                move.l  d0,d1
                move.l  dosbase,a6
                jsr    -$0024(a6)               ; Dos/Close
nxt2
                move.l  header,d0
                beq     nxt3
                move.l  d0,a1
                move.l  #32*4,d0
                move.l  4,a6
                jsr     -$00d2(a6)              ; Exec/FreeMem

nxt3
                move.l  dosbase,d0
                beq     nxt4
                move.l  d0,a1
                move.l  4,a6
                jsr     -$019e(a6)               ; exec/CloseLibrary

nxt4            move.l  sidfile,d0
                beq     nxt5
                moveq   #0,d0
                move.w  size,d0
                move.l  sidfile,a1
                move.l  4,a6
                jsr     -$00d2(a6)               ; exec/FreeMem

nxt5                          
                move.l  sidbase,d0
                beq     nxt6
                move.l  d0,a1
                move.l  4,a6
                jsr     -$019e(a6)                ; exec/CloseLibrary

nxt6            movem.l (sp)+,d2-d7/a2-a6
                rts

print          
                move.l a0,d2
find_zero       tst.b  (a0)+
                bne.s  find_zero
                subq.l #1,a0
                sub.l  d2,a0

                move.l a0,d3
                move.l outfile,d1
                move.l dosbase,a6
                jsr    -$0030(a6)
                rts

sidname         dc.b  "playsid.library",0
dosname         dc.b  "dos.library",0

text1           dc.b   "Usage: PlaySIDFile filaname",13,10,0


                even

    section variables,data

cmdbuff         dc.l     0

outfile         dc.l     0

channels        dc.w     1
                dc.w     1
                dc.w     1
                dc.w     1

dosbase         dc.l     0
sidbase         dc.l     0

sidfile         dc.l    0
emulrc          dc.l    -1
ri              dc.l    0
header          dc.l    0
tune            dc.w    0
size            dc.w    0
cm              dc.l    0
fh              dc.l    0
fileread        dc.l    0
