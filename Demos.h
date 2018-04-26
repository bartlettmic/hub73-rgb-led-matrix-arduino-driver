#pragma once
#include "Definitions.h"

#include "Rainbow.h"
#include "Fluid.h"
#include "Pong.h"
#include "Snake.h"

void (*draw)();

void (*demoSetups[])() =  {
  Rainbow::setup,
  Fluid::setup,
  Pong::setup_pong,
  Pong::setup_portal,
  Snake::setup,  
};

void (*demoLoops[])() =   {
  Rainbow::draw,
  Fluid::draw,
  Pong::draw,
  Pong::draw,
  Snake::draw,
};
