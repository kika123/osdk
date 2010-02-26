This kit will allow you to produce a operating enviroment kernel using
Microsoft Visual C++. It will also allow you to produce a floppy image
that will run under BOCHS and VMWARE. This image should work fine if
written directly to a floppy disk.

The utility cbmdf.exe creates the floppy image format.
The utilitys pe2hod.exe converts a Microsoft PE32 format to a HOD format.

http://www.freewebs.com/kmcguire/hod/

The output floppy image is called 'floppy.bin'.
The input image hodboot.bin is a precompiled floppy boot sector that understands
the CBMDF and HOD formats. It switches to protected mode and executed the kernel or
2ndstage.

To make your life easier when debugging or running your kernel you can
edit the project properties and correct the path to BOCHS under the debuging
section. I have already included a default bochs config file that should work
correct as is.