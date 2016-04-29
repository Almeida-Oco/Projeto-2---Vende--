#include "..\Headers\Declarations.h"

bool Date_t::operator> (const Date_t &D1)
{
	if (this->year > D1.year)
		return true;
	else if (this->year == D1.year)
	{
		if (this->month >= D1.month)
			return true;
		else if (this->day >= D1.day)
			return true;
		else
			return false;
	}
	return false;
}
bool Date_t::operator< (const Date_t &D1) 
{ return !(*this > D1); }



bool Trans_t::operator< (const Trans_t &T1)
{
	if (this->date.year > T1.date.year)
		return false;
	else if (this->date.year == T1.date.year)
	{
		if (this->date.month >= T1.date.month)
			return false;
		else if (this->date.day >= T1.date.day)
			return false;
		else
			return true;
	}
	return true;
}


bool Client_t::operator< (const Client_t &C1)
{
	return this->name < C1.name;
}