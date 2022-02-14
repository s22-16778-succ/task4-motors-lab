import controlP5.*; // import ControlP5 Library

ControlP5 cp5;

// Dimensions
int but_width = 100, but_height = 80;

// Colors
int background_color = #960096;
int text_color = #00FF00;

void addMotorButton(ControlP5 c, String str,
                    int x, int y, int w, int h) {
                      
  PFont font = createFont("PTMono-Bold", 15);
  
  c.addButton(str)   // name of button
   .setPosition(x,y) // upper-left corner
   .setSize(w,h)     // (width, height)
   .setFont(font);
}

void setup() {
  size(600, 600);
  cp5 = new ControlP5(this);
  
  addMotorButton(cp5, "DC", 100, 50, but_width, but_height);
  addMotorButton(cp5, "Servo", 100, 250, but_width, but_height);
  addMotorButton(cp5, "Stepper", 100, 450, but_width, but_height);
  PFont.list();
}

void draw() {
  // create initial window
  background(background_color);
  fill(text_color);
  text("MOTOR CONTROL", 100, 30);
}
