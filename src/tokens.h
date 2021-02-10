#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>

enum Verb {
	Look,
	Open,
	Push,
	Enter,
};

bool TokenizeVerb(const std::string& text, Verb& verb);

enum TokenizeResult {
	Ok,
	InvalidVerb,
	NotASentence,
};

TokenizeResult Tokenize(const std::string& text, Verb& verb, std::string& obj_name);

#endif
