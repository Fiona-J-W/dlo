#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include <string>
#include <map>
#include <utility>
#include <stdexcept>

namespace dlo{

/**
 * parse the given config-file
 * 
 * @param filename path to the configfile; a std::runtime_error will be thrown
 *   if it can't be read
 * @param sections enable or disable the use of sections; the values in a 
 *   section can be accessed via "section::key"
 */
std::map<std::string, std::string> readConfigFile(std::string filename, 
	bool sections=true);

/**
 * small class, that caches the configurations and makes the easy and fast 
 * accessible
 */
class settings{
	public:
		/**
		 * This function will safe the name of the whole application to
		 * indicate, which directory contains the configs.
		 * @param app_name the name of the programm
		 */
		static void init(std::string app_name);
		
		/**
		 * create an instance that enables access to the specified 
		 * configuration
		 * @param config name of the configfile in 
		 *   $XDG_CONFIG_HOME/app_name/
		 *   the ending '.ini' will be appended
		 * @throws dlo::uninitialised_config_error if settings::app_name is unset.
		 */
		settings(std::string config="");
		
		/**
		 * get the value to the given key in this->file
		 * @param key name of the key
		 * @throws dlo::uninitialised_config_error if settings::app_name is unset.
		 */
		std::string operator[](std::string key);
		
		/**
		 * get a Value without creating an instance of the class
		 * @param file name of the file in $XDG_CONFIG_HOME/app_name
		 *   the ending '.ini' will be appended
		 * @param key name of the key
		 * @throws dlo::uninitialised_config_error if settings::app_name is unset.
		 */
		static std::string getValue(std::string file, std::string key);
		
	private:
		/**
		 * the name of the whole application.
		 */
		static std::string app_name;
		
		/**
		 * filename of the current configuration
		 */
		std::string file;
		
		/**
		 * static cache for all read settings
		 */
		static std::map<std::string, 
			std::map<std::string,std::string> > globalSettings;
};

/**
 * Exception, that indicates, that the name of the program has not been set correctly.
 */
class uninitialised_config_error: public std::logic_error{
	/**
	 * Just reeimplement the inherited constructor from std::logic_error.
	 * @param what_arg an errormessage
	 */
	public: explicit uninitialised_config_error(const std::string& what_arg):
		logic_error(what_arg){}
};


} //namespace dlo

#endif
