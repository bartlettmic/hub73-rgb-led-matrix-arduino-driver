#pragma once
#include "Definitions.h"

#include "Rainbow.h"
#include "Fluid.h"
#include "Pong.h"
#include "Snake.h"
#include "EtchASketch.h"
#include "Tetris.h"

void (*draw)();

void (*demoSetups[])() =  {
  Rainbow::init,
  Fluid::init,
  Pong::init_pong,
  Pong::init_portal,
  Snake::init,
  EtchASketch::init,
  Tetris::init,
};

void (*demoLoops[])() =   {
  Rainbow::draw,
  Fluid::draw,
  Pong::draw,
  Pong::draw,
  Snake::draw,
  EtchASketch::draw,
  Tetris::draw,
};
