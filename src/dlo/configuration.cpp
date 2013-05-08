#include "configuration.hpp"
#include "stringutils.hpp"
#include "output.hpp"

#include <list>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

namespace dlo{

using namespace std;
using namespace stringutils;

//put the static members here:
std::string settings::app_name = "";
unordered_map<string,unordered_map<string,string> > settings::global_settings;


unordered_map<string, string> read_config_file(std::string filename, bool sections){
	ifstream file(filename.c_str());
	if(! file.is_open()){
		throw runtime_error("read_config_file(): can't open file");
	}
	unordered_map<string, string> data;
	string line;
	string section="";
	pair<string,string> tmp;
	unsigned int lineNumber=0;
	list< pair<string, pair<int, string> > > unfound_references;
	while( getline(file, line) ){
		++lineNumber;
		line=strip(line);
		//ignore empty lines and comments:
		if(line.empty() || line[0] == '#'){
			continue;
		}
		if( sections && (line[0] == '[') && (line[line.size()-1] == ']') ){
			line.erase(0,1);
			line.erase(line.size()-1,1);
			section = strip(line);
			continue;
		}
		try{
			tmp = split_once(line,"=");
		}
		catch(const invalid_argument &e){
			errorf("invalid line in configfile (%s, %s): No delimiter (“%s”)", 
				filename, lineNumber, line);
			continue;
		}
		string value = strip( tmp.second );
		if( value.length() && value[0] != '"' ){
			if( data.count(value) ){
				value = data[value];
			}
			else{
				string key = strip(tmp.first);
				if( sections && !section.empty() ){
					key = section + "::" + key;
				}
				unfound_references.push_back( make_pair(key,
					make_pair(lineNumber,value)) );
				continue;
			}
		}
		else{
			try{
				value = decode( value );
			}
			catch(invalid_argument& e){
			errorf("invalid line in configfile (%s, %s): Cannot decode string (“%s”)", 
				filename, lineNumber, line);
				continue;
			}
		}
		if( !sections ||  section.empty() ){
			data[ strip( tmp.first ) ] = value;
		}
		else{
			data[ section+"::"+strip(tmp.first) ] = value;
		}
	}
	//now see whether some references can be resolved:
	bool found_reference = false;
	do{
		found_reference = false;
		for(list< pair<string, pair<int, string> > >::iterator it = unfound_references.begin();
			it!=unfound_references.end();){
			if(data.count(it->second.second)){
				found_reference = true;
				data[it->first] = data[it->second.second];
				it = unfound_references.erase(it);
			}
			else{
				++it;
			}
			
		}
	} while(found_reference);
	for(list< pair<string, pair<int, string> > >::iterator it= unfound_references.begin();
		it!=unfound_references.end();++it){
		errorf("undefined reference in configfile (%s, %s) for “%s” to “%s”",
			filename, it->second.first, it->first, it->second.second);
	}
	return data;
}

void settings::init(std::string app_name_){
	app_name = app_name_;
}

settings::settings(string config)
	: file( config )
{
	settings::enforce_initialization();
	if(config.empty()){
		return;
	}
	if(! global_settings.count(config) ){
		global_settings[config] = read_config_file( get_conf_dir() + config + ".ini");
	}
}

string settings::operator[](string key){
	settings::enforce_initialization();
	return global_settings[this->file][key];
}

std::string settings::get_value(std::string file, std::string key){
	settings::enforce_initialization();
	if( ! global_settings.count(file) ){
		global_settings[file] = read_config_file(get_conf_dir() + file + ".ini");
	}
	return global_settings[file][key];
}

std::string settings::get_conf_dir(){
	settings::enforce_initialization();
	std::string general_conf_dir;
	auto xdg_config_home = getenv("XDG_CONFIG_HOME");
	if(xdg_config_home){
		general_conf_dir = xdg_config_home;
	}
	else {
		auto home = getenv("HOME");
		if(!home){
			throw std::runtime_error{"neither $XDG_CONFIG_HOME nor $HOME are defined"};
		}
		general_conf_dir = home;
		general_conf_dir += "/.config";
	}
	return general_conf_dir + "/" + app_name + "/";
}

void settings::enforce_initialization(){
	if(app_name.empty()){
		throw uninitialised_config_error("The name of the programm was not set properly.");
	}
}

} //namespace dlo
