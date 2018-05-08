//  76543210
//D BGRBGR✕✕ Row 0-1
//C ✕✕BGRBGR Row 2-3

#define clockin(x)\
  rgb0 = matrix[rows[0]][x];\
  /*rgb0 = rgb0& 0b1000000000000000 ? 0 : rgb0;*/\
  rgb1 = matrix[rows[1]][x];\
  /*rgb1 = rgb1 & 0b1000000000000000 ? 0 : rgb1;*/\
  rgb2 = matrix[rows[2]][x];\
  /*rgb2 = rgb2 & 0b1000000000000000 ? 0 : rgb2 ;*/\
  rgb3 = matrix[rows[3]][x];\
  /*rgb3 = rgb3 & 0b1000000000000000 ? 0 : rgb3;*/\
  PORTD =       (Rmask & rgb0 ? 4   : 0) |\
                (Gmask & rgb0 ? 8   : 0) |\
                (Bmask & rgb0 ? 16  : 0) |\
                (Rmask & rgb1 ? 32  : 0) |\
                (Gmask & rgb1 ? 64  : 0) |\
                (Bmask & rgb1 ? 128 : 0)\
                ;\
  PORTC =       (Rmask & rgb2 ? 1  : 0) |\
                (Gmask & rgb2 ? 2  : 0) |\
                (Bmask & rgb2 ? 4  : 0) |\
                (Rmask & rgb3 ? 8  : 0) |\
                (Gmask & rgb3 ? 16 : 0) |\
                (Bmask & rgb3 ? 32 : 0)\
                ;\
  digitalWriteFast(12, HIGH);\
  digitalWriteFast(12, LOW);\

#define columnsin\
  clockin(0) clockin(1) clockin(2) clockin(3) clockin(4) clockin(5) clockin(6) clockin(7)\
  clockin(8) clockin(9) clockin(10) clockin(11) clockin(12) clockin(13) clockin(14) clockin(15)\
  clockin(16) clockin(17) clockin(18) clockin(19) clockin(20) clockin(21) clockin(22) clockin(23)\
  clockin(24) clockin(25) clockin(26) clockin(27) clockin(28) clockin(29) clockin(30) clockin(31)\

