#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <regex>

#include "tokens.h"

bool TokenizeVerb(const std::string& text, Verb& verb) {
	if (text == "look")
		verb = Verb::Look;
	else if (text == "open")
		verb = Verb::Open;
	else if (text == "push")
		verb = Verb::Push;
	else if (text == "enter")
		verb = Verb::Enter;
	else
		return false;

	return true;
}

TokenizeResult Tokenize(const std::string& text, Verb& verb, std::string& obj_name) {
	std::smatch matches;
	if (std::regex_search(text, matches, std::regex(R"foo(^\s*([^\s]+)\s+(?:at )?(?:the\s+)?(.+?)\s*$)foo"))) {
		std::string _1 = matches[1];
		if (TokenizeVerb(_1, verb)) {
			obj_name = matches[2];
			return TokenizeResult::Ok;
		} else {
			return TokenizeResult::InvalidVerb;
		}
	} else {
		return TokenizeResult::NotASentence;
	}
}
