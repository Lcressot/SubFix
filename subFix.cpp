#include <iostream>
#include <array>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <fstream>

#include "optionParser.hpp"

// subFix.cpp



//========================================================================================
// functions declaration
//========================================================================================

array<int,4> str2time(string timeString);
string time2str(array<int,4> tarr);
array<float,2> compute_affine_transformation(float t1, float t2, float t1p, float t2p);
float time2seconds(array<int,4> time_array);
array<int,4> seconds2time(float s);
float lineSearch(string seq, ifstream& fsin);
void fixSubtitles(array<float,2> a_b, ifstream& fsin, ofstream& fsout);



//========================================================================================
// main
//========================================================================================
int main (int argc, char *argv[]){

	// create a OptionParser with options
	op::OptionParser opt;
	opt.add_option("h", "help", "shows option help"); // no default value means boolean options, which default value is false
	opt.add_option("if", "infile", "input subtitle file", ""); // no default value means boolean options, which default value is false
	opt.add_option("of", "outfile", "output subtitle file", "./fixed_subs.srt"); // no default value means boolean options, which default value is false
	opt.add_option("t1", "time1", "first time : hh:mm:ss,mms", "" );
	opt.add_option("s1", "seq1", "first sequence", "" );
	opt.add_option("t2", "time2", "second time hh:mm:ss,mms", "" );
	opt.add_option("s2", "seq2", "second sequence", "" );

	// parse the options and verify that all went well. If not, errors and help will be shown
	bool correct_parsing = opt.parse_options(argc, argv);

	if(!correct_parsing){
	    return EXIT_FAILURE;
	}

	// show help
	bool has_help = op::str2bool(opt["h"]);
	if(has_help) opt.show_help();

	// verify and convert arguments
	string s1 =  opt["s1"];
	string s2 =  opt["s2"];

	float t1 = time2seconds( str2time(opt["t1"]) );
	float t2 = time2seconds( str2time(opt["t2"]) );


	// TODO : also verify format

	// Open file and look for the sentences in it :
	std::ifstream fsin;
	fsin.open (opt["if"], std::fstream::in );

	if(!fsin){
		cerr << "Error opening input subs file !" << endl;
		return EXIT_FAILURE;
	}

	// look for the first sequence and return time in seconds
	float t1p = lineSearch(s1,fsin);
	if(t1p==-1.0f){
		cerr << "Error : sequence " << s1 << " not found in subtitles file !" << endl;
		return EXIT_FAILURE;
	}

	// look for the second sequence and return time in seconds
	float t2p = lineSearch(s2,fsin);
	if(t2p==-1.0f){
		cerr << "Error : sequence " << s2 << " not found in subtitles file !" << endl;
		return EXIT_FAILURE;
	}

	// compute affine transformation
	array<float,2> a_b = compute_affine_transformation(t1, t2, t1p, t2p);

	// now create new file and fill it with transformed time
	std::ofstream fsout;
	fsout.open (opt["of"], std::fstream::out );
	if(!fsout){
		cerr << "Error creating output subs file !" << endl;
		return EXIT_FAILURE;
	}
	fixSubtitles(a_b, fsin, fsout);


	// close files
	fsin.close();
	fsout.close();

	// print all OKay message
	cout << "Done !" << endl;

	return EXIT_SUCCESS;
}




//========================================================================================
// functions definition
//========================================================================================

/*
	converts string to array
	- TODO : throws string error if wrong format
*/
array<int,4> str2time(string timeString){
	array<int,4> timeArray;
	sscanf(timeString.c_str(), "%d:%d:%d,%d", &timeArray[0], &timeArray[1], &timeArray[2], &timeArray[3]);
	return timeArray;
}

/*
	converts array to string
*/
string time2str(array<int,4> tarr){
	char arr[14];
	sprintf(arr, "%02d:%02d:%02d,%03d", tarr[0], tarr[1], tarr[2], tarr[3]);
	return string(arr);
}

/*
	Computes the apropriate affine transformation's parameters a and b
		to map the subtitles time line (in seconds) from tp to t
*/
array<float,2> compute_affine_transformation(float t1, float t2, float t1p, float t2p){	
	array<float,2> a_b;

	a_b[0] = (t2-t1)/(t2p-t1p);
	a_b[1] = t1 - t1p*a_b[0];

	return a_b;	
}

/*
	converts a time array into seconds
*/
float time2seconds(array<int,4> ta){
	return 3600*ta[0] + 60*ta[1] + ta[2] + 0.001f*ta[3];
}


/*
	converts seconds into a time array
*/
array<int,4> seconds2time(float s){

	int hh = (int) s/3600;
	s -= 3600*hh;

	int mm = (int) s/60;
	s -= 60*mm;

	int ss = (int) s;
	s -= ss;

	int mms = (int) (s*1000.0f);

	array<int,4> timeArray;
	timeArray[0]=hh;
	timeArray[1]=mm;
	timeArray[2]=ss;
	timeArray[3]=mms;
	return timeArray;
}

/*
	look for a sequence in the file and return corresponding time in seconds
*/
float lineSearch(string seq, ifstream& fsin){
	fsin.seekg(0);
	string timeLine, line;
	std::size_t pos, pos_save;
	bool found=false;

	while(getline(fsin, line)) {
		cout << line << endl;
		pos = line.find("-->", 0);
	    if (pos != string::npos) {
	    	timeLine = line;
	    	pos_save = pos;
	    }
	    else if(line.find(seq, 0) != string::npos){
	    	found=true;
	    	break;
	    }
	}
	if(!found) return -1.0f; // not found

	string timestr = timeLine.substr(0, pos_save-1 );
	return time2seconds ( str2time(timestr) );
}

/*
	Fix the subtitles in fsin with affine a_b transformation and saves them in fsout
*/
void fixSubtitles(array<float,2> a_b, ifstream& fsin, ofstream& fsout){
	fsin.seekg(0, fsin.beg);
	fsout.clear();
	fsout.seekp(0, fsout.beg);

	string line, str1, str2;
	array<int,4> tarr1, tarr2 ;
	float s1, s2;

	while(getline(fsin, line)){
		
	    if (line.find("-->", 0) != string::npos) {	    	
			sscanf(line.c_str(), "%d:%d:%d,%d --> %d:%d:%d,%d", &tarr1[0], &tarr1[1], &tarr1[2], &tarr1[3], &tarr2[0], &tarr2[1], &tarr2[2], &tarr2[3]);
	    	
			s1 = time2seconds(tarr1);
			s2 = time2seconds(tarr2);

			s1 = a_b[0]*s1 + a_b[1];
			s2 = a_b[0]*s2 + a_b[1];

	    	str1 = time2str( seconds2time(s1) );
	    	str2 = time2str( seconds2time(s2) );
	    	
	    	line = str1 + " --> " + str2 + "\n";
	    }	
	   	fsout << line;	   	
	}
}