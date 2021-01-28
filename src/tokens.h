#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>

enum Verb {
	Look,
	Open,
	Read,
};

bool TokenizeVerb(std::string text, Verb &verb);

enum TokenizeResult {
	Ok,
	NotEnoughWords,
	TooManyWords,
	InvalidVerb,
};

TokenizeResult Tokenize(std::string text, Verb &verb, std::string &obj_name);

#endif
