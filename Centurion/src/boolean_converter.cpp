#include "boolean_converter.h"

using centurion::tools::BooleanConverter;

bool BooleanConverter::Convert(SDL_bool b)
{
	return (b) ? true : false;
}

SDL_bool BooleanConverter::Convert(bool b)
{
	return (b) ? SDL_bool::SDL_TRUE : SDL_bool::SDL_FALSE;
}