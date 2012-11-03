#include <iostream>
#include <sstream>

#include <sys/stat.h>

#include "../dlo/output.hpp"
#include "../dlo/stringutils.hpp"

//using namespace dlo


int output_test(){

	using namespace dlo;
	using dlo::stringutils::textf;
	using std::cout;
	using std::cerr;
	using std::endl;
	using std::stringstream;
	int fails = 0;
	
	//set up tests:
	string testout;
	string testerr;
	dlo::set_stdout_fun([&testout](const string& str){testout += str;});
	dlo::set_stderr_fun([&testerr](const string& str){testerr += str;});
	string debug_prefix;
	const string raw_debug_prefix = "DEBUG(%s) [“" __FILE__ "”, %s]: ";
	
	//set default (test them later)
	set_debug_level(1);
	set_verbosity(1);
	
	//test debugstatements (two statements on one line, but more or less without alternatives...)
	
	////debugf
	debugf(0, "debugf(0, str, arg=%s)", "‘a’"); debug_prefix = textf(raw_debug_prefix, 0, __LINE__);
#	ifdef DEBUG
	if( testout == debug_prefix + "debugf(0, str, arg=‘a’)"){
#	else
	if( testout.empty() ){
#	endif
		//worked
	}
	else{
		cerr << "test #1 for debugf() failed." << endl;
		++fails;
	}
	testout = "";
	
	debugf(1, "debugf(1, str, arg=%s)", "‘a’"); debug_prefix = textf(raw_debug_prefix, 1, __LINE__);
#	ifdef DEBUG
	if( testout == debug_prefix + "debugf(1, str, arg=‘a’)"){
#	else
	if( testout.empty() ){
#	endif
		//worked
	}
	else{
		cerr << "test #2 for debugf() failed." << endl;
		++fails;
	}
	testout = "";
	
	//this should not be printed because the debuglevel is too low:
	debugf(2, "debugf(2, str, arg=%s)", "‘a’");
	if( !testout.empty() ){
		cerr << "test #3 for debugf() failed." << endl;
		++fails;
	}
	testout = "";
	
	////debug
	debug(0, "debug(0, str, arg=", "‘a’", ')'); debug_prefix = textf(raw_debug_prefix, 0, __LINE__);
#	ifdef DEBUG
	if( testout == debug_prefix + "debug(0, str, arg=‘a’)"){
#	else
	if( testout.empty() ){
#	endif
		//worked
	}
	else{
		cerr << "test #1 for debug() failed." << endl;
		++fails;
	}
	testout = "";
	
	debug(1, "debug(1, str, arg=", "‘a’", ')'); debug_prefix = textf(raw_debug_prefix, 1, __LINE__);
#	ifdef DEBUG
	if( testout == debug_prefix + "debug(1, str, arg=‘a’)"){
#	else
	if( testout.empty() ){
#	endif
		//worked
	}
	else{
		cerr << "test #2 for debug() failed." << endl;
		++fails;
	}
	testout = "";
	
	//this should not be printed because the debuglevel is too low:
	debug(2, "debug(2, str, arg=", "‘a’", ')');
	if( !testout.empty() ){
		cerr << "test #3 for debug() failed." << endl;
		++fails;
	}
	testout = "";
	
	//testing the note-functions:
	
	note(0,"note(0)");
	if(testout != "NOTE: note(0)"){
		cerr << "test #1 for note() failed" << endl;
		++fails;
	}
	testout = "";
	
	note(1,"note(","1",')');
	if(testout != "NOTE(1): note(1)"){
		cerr << "test #2 for note() failed" << endl;
		++fails;
	}
	testout = "";
	
	note(2,"note(2)");
	if(!testout.empty()){
		cerr << "test #3 for note() failed" << endl;
		++fails;
	}
	testout = "";
	
	////notef:
	notef(0,"notef(%s)",0);
	if(testout != "NOTE: notef(0)"){
		cerr << "test #1 for notef() failed" << endl;
		++fails;
	}
	testout = "";
	
	notef(1,"notef(%s)","1");
	if(testout != "NOTE(1): notef(1)"){
		cerr << "test #2 for notef() failed" << endl;
		++fails;
	}
	testout = "";
	
	notef(2,"notef()",'2');
	if(!testout.empty()){
		cerr << "test #3 for notef() failed" << endl;
		++fails;
	}
	testout = "";
	
	////test set_note_prefix_fun():
	set_note_prefix_fun([](int){return "";});
	note(0,"note(0)");
	if(testout != "note(0)"){
		cerr << "test #1 for set_note_prefix_fun() failed" << endl;
		++fails;
	}
	testout = "";
	
	set_note_prefix_fun();
	note(0,"note(0)");
	if(testout != "NOTE: note(0)"){
		cerr << "test #1 for set_note_prefix_fun() failed" << endl;
		++fails;
	}
	testout = "";
	
	
	/////now the whole error-family in one test:
	
	warn("warn()");
	warn("warn",'(',')');
	warnf("warnf(%s)","formatstring");
	error("error",'(',')');
	error("error()");
	errorf("errorf(%s)", "formatstring");
	if( testerr == 
			"WARNING: warn()"
			"WARNING: warn()"
			"WARNING: warnf(formatstring)"
			"ERROR: error()"
			"ERROR: error()"
			"ERROR: errorf(formatstring)"
	){
		//warning-functions succesfull
	}
	else{
		cerr << "one of the warning-tests failed" << endl;
		++fails;
	}
	testerr = "";
	
	//now test the fatal ones:
	try{
		fatal("fatal()");
		cerr << "testing fatal() failed" << endl;
		++fails;
	}
	catch(std::exception &e){
		cerr << "testing fatal() failed" << endl;
		++fails;
	}
	catch(...){
		if(testerr != "FATAL: fatal()"){
			cerr << "testing fatal() failed" << endl;
			++fails;
		}
	}
	testerr = "";
	
	try{
		fatalf("fatalf(%s)","formatstring");
		cerr << "testing fatalf() failed" << endl;
		++fails;
	}
	catch(std::exception &e){
		cerr << "testing fatalf() failed" << endl;
		++fails;
	}
	catch(...){
		if(testerr != "FATAL: fatalf(formatstring)"){
			cerr << "testing fatalf() failed" << endl;
			++fails;
		}
		//we need to get here
	}
	testerr = "";
	
	////testing writeln()
	writeln("writeln(str)\n");
	writeln("writeln(str1, ", "str2)");
	if(testout == "writeln(str)\nwriteln(str1, str2)"){
		
	}
	else{
		cerr << "testing writeln() failed" << endl;
		++fails;
	}
	testout = "";
	
	////testing writefln()
	writefln("writefln(%s)\n","formatstring");
	writefln("writefln(%s, %s)","formatstring", "arg");
	if(testout == "writefln(formatstring)\nwritefln(formatstring, arg)"){
		//everything is fine
	}
	else{
		cerr << "testing writefln() failed" << endl;
		++fails;
	}
	
	////testing swriteln()
	stringstream teststream;
	swriteln(teststream, "swriteln(stream, str)");
	swriteln(teststream, "swriteln(stream, str1, ", "str2)");
	if(teststream.str() == "swriteln(stream, str)\nswriteln(stream, str1, str2)\n"){
		
	}
	else{
		cerr << "testing swriteln() failed" << endl;
		++fails;
	}
	teststream.str("");
	
	////testing swritefln()
	teststream.str() = "";
	swritefln(teststream, "swritefln(stream, %s)","formatstring");
	swritefln(teststream, "swritefln(stream, %s, %s)","formatstring", "arg");
	if(teststream.str() == "swritefln(stream, formatstring)\nswritefln(stream, formatstring, arg)\n"){
		//everything is fine
	}
	else{
		cerr << "testing swritefln() failed" << endl;
		++fails;
	}
	teststream.str("");
	
	mkdir("test",0777);
	
	
	///////////////////////////////////////////////////////////////////////////////////
	//////// now we are done.
	//reset the output-functions:
	set_stdout_fun();
	set_stderr_fun();
	if(fails){
		errorf("%s tests failed", fails);
	}
	else{
		note(0, "all tests were succesfull");
	}
	
	return fails;
}
