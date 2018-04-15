#pragma once
#include "Definitions.h"

#include "Rainbow.h"
#include "Fluid.h"

void (*draw)(int16_t matrix[HEIGHT][WIDTH]);
void (*demoSetups[])(int16_t matrix[HEIGHT][WIDTH]) =  { Rainbow::setup, Fluid::setup };
void (*demoLoops[])(int16_t matrix[HEIGHT][WIDTH]) =   { Rainbow::draw,  Fluid::draw  };
