//                              -*- Mode: C++ -*- 
// psort.cc -- Priority sort (for example a ~/.newsrc file)
// 
// $Header$
// 
// Copyright (C)  1991  The Board of Trustees of The Leland Stanford
//                                  Junior University.  All Rights Reserved.
// 
// Creator         : Tom Pavel  <pavel>     Wed Jan 15 14:15:03 1992
// Modifier        : Tom Pavel  <pavel>     Mon Dec  1 11:00:55 1997
// Update Count    : 58
// Status          : Rewritten to use STL
// 

// The idea is to use a file of regular expressions to sort another file (or
// the standard input).  All lines of the input which match the first regexp
// in the control file are sorted lexically, and printed to stdout.   All
// remaining lines of input which match the 2nd regexp are sorted and
// printed.  And so forth.  Finally, when the control file reaches EOF, all
// remaing lines of the input are sorted and printed.



#include <iostream.h>
#include <fstream.h>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <list>
#include <cstdlib>

#include "Regexp.h"


#ifndef lint
static char RCSid[] = "$Header$";
#endif

#ifdef __GNUG__
#pragma implementation 
#endif


// Error reporting functions:

static char* progname;

void Error(char* s)
{
   cerr << progname << ": " << s << endl;
   exit(1);
}

void Usage()
{
   cerr << "Usage: " << progname << " <control-file> [newsrc-file]\n";
   exit(1);
}



// Supporting classes:

class GroupMatch {
public:
    GroupMatch (const string& pattern) : _pat(pattern), _re(pattern.c_str()) {}
    GroupMatch (const GroupMatch& b) : _pat(b._pat), _re(b._pat.c_str()) {}
    ~GroupMatch() {}

    bool operator() (const string& a) {
	return _re.Match (a.c_str());
    }

private:
    operator= (const GroupMatch& b) {}

    string _pat;
    Regexp _re;
};



// Define a new algorithm to do a copy of all elements matching a predicate.
template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator copy_if(InputIterator first, InputIterator last,
		       OutputIterator result, Predicate pred) 
{
    while (first != last) {
	if (pred(*first))
	    *result++ = *first;
	++first;
    }
    return result;
    
}



typedef istream_iterator<string, ptrdiff_t> string_input;
typedef ostream_iterator<string> string_output;




int
main(int argc, char** argv)
{
   progname = argv[0];
   int inplace = 0;

   istream* input = &cin;
   ostream* output = &cout;

   switch (argc) {
   case 2:
       break;
   case 3:
       input =  new ifstream(argv[2]);
       inplace = 1;
       break;
   default:
       Usage();
   }


   // read in .newsrc file:
   list<string> newsgroups;
   //vector<string> newsgroups;
   string line;
   while ( getline (*input, line, '\n') ) {
       // should strip line if it is a '!' line...
       newsgroups.push_back (line);
   }


   // open output stream
   ifstream control(argv[1]);
   if (inplace) {
      output = new ofstream(argv[2]);
   }


   // now loop through the patterns
   string pattern;
   while (control >> pattern) {

       vector<string> matchlist;
       // find and remove all matches:
       copy_if (newsgroups.begin(), newsgroups.end(),
		back_inserter(matchlist), GroupMatch(pattern));
       newsgroups.erase( remove_if(newsgroups.begin(),
				   newsgroups.end(),
				   GroupMatch(pattern)),
			 newsgroups.end());

       // alphabetize and output:
       sort (matchlist.begin(), matchlist.end());
       copy (matchlist.begin(), matchlist.end(), string_output(*output, "\n"));

#ifdef DEBUG
       // DEBUGGING
       (*output) << "** end of pass **" << endl;
       (*output) << "** newsgroups (" << newsgroups.size() << ") = " << endl;
       copy (newsgroups.begin(), newsgroups.end(),
	     string_output(*output, "\n"));
       (*output) << "** end of debug **" << endl;
#endif

   }

   // now alphabetize and spit out the rest:
   //sort (newsgroups.begin(), newsgroups.end());
   newsgroups.sort();
   copy (newsgroups.begin(), newsgroups.end(), string_output(*output, "\n"));


   return (0);
}





