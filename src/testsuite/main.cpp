#include "../dlo/output.hpp"

#include "string_test.hpp"
#include "sig_test.hpp"
#include "conf_test.hpp"
#include "output_test.hpp"

int main(){
	using dlo::writeln;
	using dlo::writefln;
	
	unsigned int total_fails = 0;
	
	dlo::set_verbosity(1);
	writeln("Testing the output-lib");
	total_fails += output_test();
	dlo::set_verbosity(2);
	writeln("\nTesting the stringutils");
	total_fails += string_test();
	writeln("\nTesting the signalhandling-lib");
	total_fails += sig_test();
	writeln("\nTesting the config-lib");
	total_fails += conf_test();
	
	if(!total_fails){
		writeln("\nAll tests successfull");
		return 0;
	}
	else{
		writefln("\n%s tests failed", total_fails);
		return 1;
	}
}
