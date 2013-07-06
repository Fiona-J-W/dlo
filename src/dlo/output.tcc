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

void _writeln(const string& text);
void _writeln(std::ostream& stream, const string& text);
void _note(int level, const string& text);
void _warn(const string& text);
void _error(const string& text);
void _fatal(const string& text);

#ifdef DEBUG
void _debug(const string& filename, const string& function_name, int line, int level, const string& text);
#endif

template<typename... T>
void writeln(T&&...args){
	_writeln( stringutils::text(std::forward<T>(args)...) );
}

template<typename... T>
void writefln(const string& formatstring, T&&...args){
	_writeln( stringutils::textf(formatstring, std::forward<T>(args)...) );
}

template<typename T>
void swriteln(std::ostream& stream,T arg){
	stream << arg << std::endl;
}


template<typename T1, typename... T2>
void swriteln(std::ostream& stream,T1 arg, T2&&...args){
	stream << arg;
	swriteln(stream, std::forward<T2>(args)...);
}

template<typename... T>
void swritefln(std::ostream& stream, const string& formatstring, T&&...args){
	stream << stringutils::textf(formatstring, std::forward<T>(args)...) << std::endl;
}

template<typename... T>
void note(int level, T&&...args){
	_note( level, stringutils::text(std::forward<T>(args)...));
}

template<typename... T>
void notef(int level, const string& formatstring, T&&...args){
	_note( level, stringutils::textf(formatstring, std::forward<T>(args)... ));
}


template<typename... T>
void warn(T&&... args){
	_warn( stringutils::text(std::forward<T>(args)...));
}

template<typename... T>
void warnf(const string& formatstring, T&&...args){
	_warn( stringutils::textf(formatstring, std::forward<T>(args)...));
}


template<typename... T>
void error(T&&...args){
	_error( stringutils::text(std::forward<T>(args)...));
}

template<typename... T>
void errorf(const string& formatstring, T&&...args){
	_error( stringutils::textf(formatstring, std::forward<T>(args)...));
}


template<typename... T>
void fatal(T&&...args){
	_fatal( stringutils::text(std::forward<T>(args)...));
}

template<typename... T>
void fatalf(const string& formatstring, T&&...args){
	_fatal( stringutils::textf(formatstring, std::forward<T>(args)...));
}

} //logging namespace

#endif
