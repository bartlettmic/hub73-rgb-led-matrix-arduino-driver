#pragma once
#define CUBE_SIZE 450
#define CUBE_COLOR 0b0000000000100001

extern trimpots knobs;

float d = 3;
float px[] = { -d,  d,  d, -d, -d,  d,  d, -d };
float py[] = { -d, -d,  d,  d, -d, -d,  d,  d };
float pz[] = { -d, -d, -d, -d,  d,  d,  d,  d };

float p2x[] = {0,0,0,0,0,0,0,0};
float p2y[] = {0,0,0,0,0,0,0,0};

float r[] = {0,0,0};

namespace Cube {

void init() {
	fillMatrix(0);
}

void draw() {
	knobs.read();
  	// r[0]=r[0]+M_PI/180.0; // Add a degree
	// r[1]=r[1]+M_PI/180.0; // Add a degree
	// r[2]=r[2]+M_PI/180.0; // Add a degree
	
	r[0]=M_PI * knobs.r/1024;
	r[1]=M_PI * knobs.l/1024;

	for (int i=0;i<8;i++)	{
		float px2 = px[i];
		float py2 = cos(r[0])*py[i] - sin(r[0])*pz[i];
		float pz2 = sin(r[0])*py[i] + cos(r[0])*pz[i];

		float px3 = cos(r[1])*px2 + sin(r[1])*pz2;
		float py3 = py2;
		float pz3 = -sin(r[1])*px2 + cos(r[1])*pz2;

		float ax = cos(r[2])*px3 - sin(r[2])*py3;
		float ay = sin(r[2])*px3 + cos(r[2])*py3;
		float az = pz3-150;

		p2x[i] = WIDTH/2+ax*CUBE_SIZE/az;
		p2y[i] = HEIGHT/2+ay*CUBE_SIZE/az;
	}
	
	fillMatrix(0);
	
	for (int i=0;i<3;i++)	{
		line(p2x[i],p2y[i],p2x[i+1],p2y[i+1],CUBE_COLOR);
		line(p2x[i+4],p2y[i+4],p2x[i+5],p2y[i+5],CUBE_COLOR);
		line(p2x[i],p2y[i],p2x[i+4],p2y[i+4],CUBE_COLOR);
	}
	line(p2x[3],p2y[3],p2x[0],p2y[0],CUBE_COLOR);
	line(p2x[7],p2y[7],p2x[4],p2y[4],CUBE_COLOR);
	line(p2x[3],p2y[3],p2x[7],p2y[7],CUBE_COLOR);
}
}