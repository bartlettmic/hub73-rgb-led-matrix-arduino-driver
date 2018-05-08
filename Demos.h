#pragma once

#include "Demos/Rainbow.h"
#include "Demos/Fluid.h"
#include "Demos/Pong.h"
#include "Demos/Snake.h"
#include "Demos/EtchASketch.h"
#include "Demos/Compass.h"
#include "Demos/Cube.h"

void (*draw)();

void (*demoSetups[])() =  {
  Rainbow::init,
  Fluid::init,
  Pong::init_pong,
  Pong::init_portal,
  Snake::init,
  EtchASketch::init,
  Compass::init,
  Cube::init,
};

void (*demoLoops[])() =   {
  Rainbow::draw,
  Fluid::draw,
  Pong::draw,
  Pong::draw,
  Snake::draw,
  EtchASketch::draw,
  Compass::draw,
  Cube::draw,
};
