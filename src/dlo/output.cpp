

//this might be used to create a library for debugging, so:
#ifndef DEBUG
#define DEBUG
#endif
#include "output.hpp"

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <ctime>
#include <mutex>
#include <atomic>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace dlo{

using std::to_string;
using std::cout;
using std::cerr;
using std::endl;


string default_get_note_prefix(int level);

// TODO: fix thread-safety

//in fact, those are like private members:
namespace impl{
	static std::atomic_int verbose_level{0};
	static std::atomic_int debug_level{0};
	static int logfile{-1}; //filedescriptor
	static std::function<int(int)> log_close_function = close;
	static std::mutex write_mutex;
	static std::mutex note_prefix_mutex;
	
	static std::function<void(const std::string&)> write_normal
		= [](const std::string& text){cout << text << endl;};
	static std::function<void(const std::string&)> write_error
		= [](const std::string& text){cerr << text << endl;};
	static std::function<string(int level)> get_note_prefix = default_get_note_prefix;
}

/**
 * internal function that closes the logfile and checks the states
 */
void close_logfile(){
	if(impl::logfile != -1){
		if(impl::log_close_function){
			impl::log_close_function(impl::logfile);
		}
		impl::logfile = -1;
	}
}

void set_verbosity(int level){
	impl::verbose_level=level;
}

int set_logfile(const string& filename){
	std::lock_guard<std::mutex> guard{impl::write_mutex};
	close_logfile();
	if(!filename.empty()){
		impl::log_close_function = close;
		if( (impl::logfile = open(filename.c_str(),O_WRONLY | O_CREAT | O_APPEND, 0666)) == -1){
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
	std::lock_guard<std::mutex> guard{impl::write_mutex};
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
	if(level <= impl::verbose_level ){
		std::string prefix;
		/* synchronize */{
			std::lock_guard<std::mutex> guard{impl::note_prefix_mutex};
			prefix = impl::get_note_prefix(level);
		}
		print_and_log(prefix, text);
	}
}

void _warn(string text){
	print_and_log("WARNING: ", text, false);
}

void _error(string text){
	print_and_log("ERROR: ", text, false);
}

namespace {
class fatal_error_exception{
};
}

void _fatal(string text){
	print_and_log("FATAL: ", text, false);
	throw fatal_error_exception();
}

string get_timestamp(){
	static char buffer[30]="\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
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
		std::lock_guard<std::mutex> guard{impl::write_mutex};
		impl::write_normal( msg );
	}
	else{
		std::lock_guard<std::mutex> guard{impl::write_mutex};
		impl::write_error( msg );
	}
	std::lock_guard<std::mutex> guard{impl::write_mutex};
	if( impl::logfile != -1 ){
		string str = stringutils::prefix_and_align( "[" + get_timestamp() + "] ", msg + '\n');
		if( write(impl::logfile, str.c_str(), str.size()) == -1 ){
			cerr << "Error: Could not write to logfile, Errorcode:" << errno << endl;
		}
	}
}

void set_stdout_fun(std::function<void(const string&)> fun){
	std::lock_guard<std::mutex> guard{impl::write_mutex};
	impl::write_normal = fun;
}

void set_stderr_fun(std::function<void(const string&)> fun){
	std::lock_guard<std::mutex> guard{impl::write_mutex};
	impl::write_error = fun;
}

void set_note_prefix_fun(std::function<string(int)> fun){
	if(fun == nullptr){
		std::lock_guard<std::mutex> guard{impl::note_prefix_mutex};
		impl::get_note_prefix = default_get_note_prefix;
	}
	else{
		std::lock_guard<std::mutex> guard{impl::note_prefix_mutex};
		impl::get_note_prefix = fun;
	}
}

string default_get_note_prefix(int level){
	if(level == 0){
		return "";
	}
	else{
		return "NOTE(" + to_string(level) + "): ";
	}
}


} //namespace logging
