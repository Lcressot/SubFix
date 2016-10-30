# SubFix : fix your subtitles

This little soft operates an affine transformation on a subtitle file (i.e a translation and scale of time) to fix it.
Just give it 2 text sequences in the subtitle file to fix and the two corresponding times of the video.

Here are the options of the software:

```c++
- ("h", "help", "shows option help"); // no default value means boolean options, which default value is false
- ("if", "infile", "input subtitle file", ""); // no default value means boolean options, which default value is false
- ("of", "outfile", "output subtitle file", "./fixed_subs.srt"); // no default value means boolean options, which default value is false
- ("t1", "time1", "first time : hh:mm:ss,mms", "" );
- ("s1", "seq1", "first sequence", "" );
- ("t2", "time2", "second time hh:mm:ss,mms", "" );
- ("s2", "seq2", "second sequence", "" );
``