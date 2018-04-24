#pragma once
#include "Definitions.h"

#include "Rainbow.h"
#include "Fluid.h"
#include "Pong.h"

void (*draw)();

void (*demoSetups[])() =  {
  Rainbow::setup,
  Fluid::setup,
  Pong::setup,
  Pong::setup
};

void (*demoLoops[])() =   {
  Rainbow::draw,
  Fluid::draw,
  Pong::draw,
  Pong::draw
};
