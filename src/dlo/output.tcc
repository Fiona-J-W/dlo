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
	_note( level, text(args...) );
}

template<typename... T>
void notef(int level, const string& formatstring, T...args){
	_note( level, textf(formatstring, args...) );
}


template<typename... T>
void warn(T...args){
	_warn( text(args...) );
}

template<typename... T>
void warnf(const string& formatstring, T...args){
	_warn( textf(formatstring, args...) );
}


template<typename... T>
void error(T...args){
	_error( text(args...) );
}

template<typename... T>
void errorf(const string& formatstring, T...args){
	_error( textf(formatstring, args...) );
}


template<typename... T>
void fatal(T...args){
	_fatal( text(args...) );
}

template<typename... T>
void fatalf(const string& formatstring, T...args){
	_fatal( textf(formatstring, args...) );
}


template<typename T>
void _text_helper(stringstream& stream, T arg){
	stream << arg;
}

template<typename T, typename... argT>
void _text_helper(stringstream& stream, T arg,  argT...args){
	stream << arg;
	_text_helper(stream, args...);
}

template<typename... T>
string text(T...args){
	stringstream returnstream;
	_text_helper(returnstream, args...);
	return returnstream.str();
}



template<typename T>
void _textf_helper(vector<string>& vec, T arg){
	stringstream tmpstream;
	tmpstream << arg;
	vec.push_back(tmpstream.str());
}

template<typename T, typename... argT>
void _textf_helper(vector<string>& vec, T arg, argT...args){
	stringstream tmpstream;
	tmpstream << arg;
	vec.push_back(tmpstream.str());
	_textf_helper(vec, args...);
}

string _textf_impl(const std::vector<string>& strings);

template<typename...T>
string textf(const string& formatstring, T...args){
	vector<string> strings;
	strings.push_back(formatstring);
	_textf_helper(strings, args...);
	return _textf_impl(strings);
}


} //logging namespace

#endif
