

//this might be used to create a library for debugging, so:
#ifndef DEBUG
#define DEBUG
#endif
#include "output.hpp"

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <ctime>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace dlo{

using std::to_string;
using std::cout;
using std::cerr;
using std::endl;

//in fact, those are like private members:
namespace impl{
	static int verbose_level = 0;
	static int logfile = 0; //filedescriptor
	static int debug_level = 0;
	static bool stdout_quiet = false;
	static bool stderr_quiet = false;
	static std::function<int(int)> log_close_function = close;
}

/**
 * internal function that closes the logfile and checks the states
 */
void close_logfile(){
	if(impl::logfile){
		//do this with a temp var to prevent race-conditions:
		volatile int tmp = impl::logfile;
		impl::logfile = 0;
		if(impl::log_close_function){
			impl::log_close_function( tmp );
		}
	}
}

void set_verbosity(int level){
	impl::verbose_level=level;
}

int set_logfile(const string& filename){
	close_logfile();
	if(!filename.empty()){
		impl::log_close_function = close;
		if( (impl::logfile = open(filename.c_str(),O_WRONLY | O_CREAT | O_APPEND, 0666)) == -1){
			impl::logfile = 0;
			error("Could not open logfile.");
			return -1;
		}
		else{
			return 0;
		}
	}
	return 0;
}

void set_logfile(int fd, std::function<int(int)> close_fun){
	close_logfile();
	impl::log_close_function = close_fun;
	impl::logfile = fd;
}


void _debug(string filename, int line, int level, string text){
	if(level<=impl::debug_level){
		string metadata = stringutils::textf("DEBUG(%s) [“%s”, %s]: ", level, filename, line);
		print_and_log(metadata, text);
	}
}

void set_debug_level(int d){
	impl::debug_level=d;
}

void _writeln(string text){
	print_and_log(text);
}

void _writeln(std::ostream& stream, string text){
	stream << text << endl;
}

void _note(int level, string text){
	if(level<=impl::verbose_level){
		if(level){
			print_and_log("NOTE(" + to_string(level) + "): ", text);
		}
		else{
			print_and_log("NOTE: ", text);
		}
	}
}

void _warn(string text){
	print_and_log("WARNING: ", text, false);
}

void _error(string text){
	print_and_log("ERROR: ", text, false);
}

class fatal_error_exception{
};

void _fatal(string text){
	print_and_log("FATAL: ", text, false);
	throw fatal_error_exception();
}


string get_timestamp(){
	char buffer[30];
	time_t t = time(NULL);
	
	strftime(buffer, 28, "%x %X", localtime(&t));
	
	
	string returnstr = buffer;
	return returnstr;
}

void print_and_log(const string& prefix, const string& msg, bool normal){
	print_and_log( stringutils::prefix_and_align(prefix, msg), normal);
}
void print_and_log(const string& msg, bool normal){
	if( normal ){
		if(!impl::stdout_quiet){
			cout << msg << std::endl;
		}
	}
	else{
		if(!impl::stderr_quiet){
			cerr << msg << std::endl;
		}
	}
	if( impl::logfile ){
		string str = stringutils::prefix_and_align( "[" + get_timestamp() + "] ", msg + '\n');
		if( write( impl::logfile , str.c_str(), str.size()) == -1 ){
			cerr << "Error: Could not write to logfile, Errorcode:" << errno << endl;
		}
	}
}

void set_stdout_quiet(bool quiet){
	impl::stdout_quiet = quiet;
}

void set_stderr_quiet(bool quiet){
	impl::stderr_quiet = quiet;
}


} //namespace logging
