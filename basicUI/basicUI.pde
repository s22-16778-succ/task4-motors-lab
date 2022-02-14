import controlP5.*; // import ControlP5 Library

ControlP5 cp5;

// Colors and Dimensions
int background_color = #555555;
int title_color = #FFFF00;
int scr_width = 500, scr_height = 600;
int but_width = 200, but_height = 100;


void addMotorButton(ControlP5 c, String str,
                    int x, int y, int w, int h,
                    int clr) {
                      
  PFont font = createFont("PTMono-Bold", 15);
  // print(PFont.list()); // see full list of available fonts
  
  c.addButton(str)           // name of button
   .setPosition(x,y)         // upper-left corner
   .setSize(w,h)             // (width, height)
   .setFont(font)            // font type
   .setColorBackground(clr); // button color
}

void setup() {
  // Create initial window
  size(500, 600); // same as scr_width and scr_height
  cp5 = new ControlP5(this);
  background(background_color);
  
  // Add all buttons
  int mid_width = (scr_width - but_width) / 2;
  addMotorButton(cp5, "DC", mid_width, 100, but_width, but_height, #880000);
  addMotorButton(cp5, "Servo", mid_width, 250, but_width, but_height, #008800);
  addMotorButton(cp5, "Stepper", mid_width, 400, but_width, but_height, #000088);
}

void draw() {
  // Create title
  fill(title_color);
  textAlign(CENTER, CENTER);
  textFont(createFont("Arial", 40));
  text("MOTOR CONTROL", scr_width/2, 30);
}

/** Button Functions (pass info to Arduino Sketch) **/

void DC() {
  print("DC pressed!\n");
}
void Servo() {
  print("Servo motor pressed!\n");
}
void Stepper() {
  print("Stepper motor pressed!\n");
}
