#include "Combobox.h"

Combo::Combo() {
	list = { "1990", "1991", "1992" };
	deafult = "year";
}

vector<string> Combo::getList() 
{
	return list;
}

string Combo::getDeafult() 
{
	return deafult;
}

void Combo::setDeafult(string str)
{
	deafult = str;
}

Combo::~Combo() {};