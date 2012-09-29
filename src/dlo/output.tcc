#ifndef output_hpp
	#error "don't include output.tcc directly, use output.hpp instead."
#endif

#ifndef output_tcc
#define output_tcc

namespace dlo {

using std::string;
using std::stringstream;
using std::vector;
using std::tuple;


// implementation-functions:

void _note(int level, string text);
void _warn(string text);
void _error(string text);
void _fatal(string text);

#ifdef DEBUG
/*
 * prints a debugmessage; this is an internal function never call it directly.
 * @param filename sourcefile, that calls the function
 * @param line line of the functioncall
 * @param level the debuglevel
 * @param text the debugmessage
 */
void _debug(string filename, int line, int level, string text);
#endif

template<typename... T>
void note(int level, T...args){
	_note( level, stringutils::text(args...) );
}

template<typename... T>
void notef(int level, const string& formatstring, T...args){
	_note( level, stringutils::textf(formatstring, args...) );
}


template<typename... T>
void warn(T...args){
	_warn( stringutils::text(args...) );
}

template<typename... T>
void warnf(const string& formatstring, T...args){
	_warn( stringutils::textf(formatstring, args...) );
}


template<typename... T>
void error(T...args){
	_error( stringutils::text(args...) );
}

template<typename... T>
void errorf(const string& formatstring, T...args){
	_error( stringutils::textf(formatstring, args...) );
}


template<typename... T>
void fatal(T...args){
	_fatal( stringutils::text(args...) );
}

template<typename... T>
void fatalf(const string& formatstring, T...args){
	_fatal( stringutils::textf(formatstring, args...) );
}

} //logging namespace

#endif
