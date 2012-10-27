#ifndef I18N_HPP
#define I18N_HPP

#include <string>
#include <unordered_map>

namespace dlo{
namespace i18n{

using std::string;

/**
 * get the localisation of a string.
 * @param str the original text
 * @returns the localisation of the string or the string itself if 
 *         no localisation could be found.
 */
string i18n(const string& str);

/*
 * sets the language that will be used for localisation.
 * @param lang the name of the language; if empty, localisation will be disabled
 * @throws 
 */
//void set_lang(string lang);

/**
 * set the used dictionary.
 * @param dict the new dictionary
 */
void set_dict(const std::unordered_map<string, string>& dict);

}} //namespaces

#endif // I18N_HPP
