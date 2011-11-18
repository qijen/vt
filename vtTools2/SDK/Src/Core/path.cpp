#include "Path.h"

char* 
XPath::GetFileName(){

	char *pc = data_.Str();
	int LastBackSlash =-1;
	int iChar = 0;
	while(pc[iChar] != 0){
		if(pc[iChar] == '\\')LastBackSlash = iChar;
			iChar++;
	}
	return pc + LastBackSlash +1;
}

char*
XPath::GetPath(){
	
	char* PathCopy = data_.Str();
	PathRemoveFileSpec(PathCopy);
	return PathCopy;

}

#include <STDLIB.H>
int
XPath::absolute_levels()const{

	//count path_sep´s
	/*
	using namespace std;


	string s = data_.CStr();
	string::iterator cit = find(s.begin(),s.end(),path_separator_);
	int n  =  0;//;-) or how do you make this?
	while ( cit != s.end()){
		++n;
		cit = find(cit+1 ,s.end(),path_separator_);
	}*/
	return 1;
}




