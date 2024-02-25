#pragma once

#include <glad/glad.h>
#include <iostream>

struct WindowHint
{
	int windowArgument;
	int windowArgumentValue;

	WindowHint(int arg, int val) : windowArgument(arg), windowArgumentValue(val) { };
};