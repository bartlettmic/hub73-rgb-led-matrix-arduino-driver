
#define TREE_COLOR 0b0000000000100000
#define TREE_LAYERS 5

extern trimpots knobs;


namespace Tree {

double angle = 0.3 * M_PI; //angle in radians
double shrink = 1.8; //relative size of new branches
uint8_t size = 15;

void recursion(double posX, double posY, double dirX, double dirY, double size, int n) {
 
  int x1, x2, y1, y2;
  x1 = int(posX);
  y1 = HEIGHT-int(posY);
  x2 = int(posX + size * dirX);
  y2 = HEIGHT-int(posY + size * dirY);
  safeLine(x1, y1, x2, y2, TREE_COLOR);
//   safeLine(WIDTH - x1, y1, WIDTH - x2, y2, TREE_COLOR);

  if(!--n) return;   
  
  double posX2, posY2, dirX2, dirY2, size2;
  posX2 = posX + size * dirX;
  posY2 = posY + size * dirY;
  size2 = size / shrink;
  dirX2 = cos(angle) * dirX + sin(angle) * dirY;
  dirY2 = -sin(angle) * dirX + cos(angle) * dirY;
  recursion(posX2, posY2, dirX2, dirY2, size2, n);
  dirX2 = cos(-angle) * dirX + sin(-angle) * dirY;
  dirY2 = -sin(-angle) * dirX + cos(-angle) * dirY;
  recursion(posX2, posY2, dirX2, dirY2, size2, n);
}


void init() {
    fillMatrix(0);
}

void draw() {
	uint8_t r = knobs.r>>4, l = knobs.l>>4;
    knobs.read();
    uint8_t r2 = knobs.r>>4, l2 = knobs.l>>4;

    // if (Serial.available()) Serial.println(r2);
    
    if (r != r2 || l != l2) {
        fillMatrix(0);
        angle = r2/128.0 * M_PI;
        size = 10 + (l2/12);
        recursion(WIDTH/2, 0, 0, 1, size, TREE_LAYERS);
    }
    delay(25);
}
}