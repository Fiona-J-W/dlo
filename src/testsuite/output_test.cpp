#include <iostream>
#include "../dlo/output.hpp"

//using namespace dlo


int main(int argc, char **argv){
	try{
		using namespace dlo;
		if(argc > 1){
			set_logfile(argv[argc-1]);
		}
		std::cout << "testing debug-statements; you should see 4 lines "
			"in debugmode, 0 otherwise:" << std::endl;
		set_debug_level(1);
		debugf(0, "debugf(1, str, arg=%s)", "‘a’");
		debugf(1, "debugf(2, str, arg=%s)", "‘a’");
		debugf(2, "debugf(3, str, arg=%s) you should not see this", "‘a’");
		debug(0, "debug(1, str, arg=", "‘a’", "\"");
		debug(1, "debug(2, str, arg=", "‘a’", "\"");
		debug(2, "you should not see this");
		
		std::cout << "\ntesting note() and notef(); you should see 4 lines:" << std::endl;
		set_verbosity(1);
		note(0, "note(level=", 0, ", msg)");
		note(1, "note(level=", 1, ", msg)");
		note(2, "note(level=", 2, ", msg), you should not see this");
		notef(0, "notef(level=%s, msg)",0);
		notef(1, "notef(level=%s, msg)",1);
		notef(2, "notef(level=%s, msg), you should not see this",2);
		
		
		std::cout << "\ntesting warn() and warnf(); you should see 2 lines:" << std::endl;
		warn("warn(str)");
		warnf("warnf(str, arg=%s)", "arg");
		
		std::cout << "\ntesting error() and errorf(); you should see 2 lines:" << std::endl;
		error("error(str)");
		errorf("errorf(str, arg=%s)", "arg");
		
		std::cout << "\ntesting writeln() and writefln(); you should see 2 lines:" << std::endl;
		writeln("writeln(str)");
		writefln("writefln(str, arg=%s)", "arg");
		
		std::cout << "\ntesting swriteln() and swritefln() with std::cout"
			"; you should see 2 lines:" << std::endl;
		swriteln(std::cout, "swriteln(str)");
		swritefln(std::cout, "swritefln(str, arg=%s)", "arg");
		//swriteln(std::cout, "write directly to a stream");
		//warn("And now: commit suicide!!!");
		
		writeln("");
		fatal("trying to terminate via fatal()");
		return 1;
	}
	catch(...){
		std::cout << "success" << std::endl;
		return 0;
	}
}
