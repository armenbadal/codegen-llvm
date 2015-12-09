
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//
void printInteger(int v)
{
	printf("%d", v);
}

//
void printBoolean(bool v)
{
	if( v )
		puts("TRUE");
	else
		puts("FALSE");
}

//
int inputInteger()
{
	printf("? ");
	int val = 0;
	scanf("%d", &val);
	// TODO կատարել ստուգումներ	
	return val;
}

//
bool inputBoolean()
{
	char text[64] = { 0 };
	scanf("%s", text);
	if( 0 == strcmp("TRUE", text) )
		return true;
	else if( 0 == strcmp("FALSE", text) )
		return false;

	// TODO կատարել ստուգումներ
	return false; 
}

