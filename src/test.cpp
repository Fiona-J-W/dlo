#include "output.hpp"

using namespace logging;

int main(){
	set_debug_level(10);
	set_logfile("test.log");
	debug(0,"foo ", "bar", " baz ", 3, " ", 4, ' ');
	note(0, "some note");
}
