# SubFix : fix your subtitles !

This soft carries out an affine transformation on a subtitle file (i.e a translation and a scale of time) in order to fix it.
Just give as paramaters 2 text sequences of the subtitle file and the two corresponding times of the video, and the soft will calculate the transformation and fix the file.

## Project
+ This soft aims at fixing subtitles files (.srt).
It carries out an affine transformation on a subtitle file (i.e a translation and a scale of time) in order to fix it.

+ Input : 2 text sequences of the subtitle file, and the 2 corresponding times of the video (parameters t1,s1, t2, s2), or simply a time translation (parameter t alone).
+ Output : Transformed subtitles file.

## Options
Here are the options of the software:
```c++
- ("h", "help", "shows option help"); // no default value means boolean options, which default value is false
- ("if", "infile", "input subtitle file", ""); // no default value means boolean options, which default value is false
- ("of", "outfile", "output subtitle file", "./fixed_subs.srt"); // no default value means boolean options, which default value is false
- ("t1", "time1", "first time : hh:mm:ss,mms", "" );
- ("s1", "seq1", "first sequence", "" );
- ("t2", "time2", "second time hh:mm:ss,mms", "" );
- ("s2", "seq2", "second sequence", "" );
- ("t", "translate", "translate the subtitles of duration : hh:mm:ss,mms", "" );
```

## TO DO : 
+ there seem to be format error for the output srt files because they are not recognised by VLC media player. For the moment I copy paste the output to a valid srt file.
+ Some files don't work, why ??
