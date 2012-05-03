// string -- template string support functions
#include <locale>
#include <istream>
#include <stdexcept>

// report a length_error
void __cdecl throw_length_exception()
{
	_THROW(std::length_error, "string too long"); 
}

// report an out_of_range error
void __cdecl throw_range_exception()
{
	_THROW(std::out_of_range, "invalid string position"); 
}