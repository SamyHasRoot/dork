#include <vector>
#include <sstream>
#include <iterator>

#include "tokens.h"

bool TokenizeVerb(std::string text, Verb &verb) {
	if (text == "look")
		verb = Verb::Look;
	else if (text == "open")
		verb = Verb::Open;
	else if (text == "read")
		verb = Verb::Read;
	else
		return false;

	return true;
}

TokenizeResult Tokenize(std::string text, Verb &verb, std::string &obj_name) {
	// split on ' ' (also wtf, c++)
	std::istringstream iss(text);
	std::vector<std::string> text_vec{std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>{}};

	if (text_vec.size() != 2)
		return text_vec.size() > 2 ? TooManyWords : NotEnoughWords;

	obj_name = text_vec[1];

	if (!TokenizeVerb(text_vec[0], verb))
		return TokenizeResult::InvalidVerb;

	return TokenizeResult::Ok;
}
