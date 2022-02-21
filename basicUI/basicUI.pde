import controlP5.*; // import ControlP5 Library
import processing.serial.*; // serial communication w/ Arduino

// Initialize global fields
ControlP5 cp5;
Serial port;

// Colors
int background_color = #555555;
int title_color = #FFFF00;
int io_color = #8C8C8C;
int text_color = #000000;

// Dimensions
int scr_width = 1000, scr_height = 600;
int but_width = 100, but_height = 70;
int text_width = 150, text_height = 30;
int text_space = text_height + 10;
int col0 = 10, col1 = 180, col2 = 350, col3 = 530, col4 = 660, col5 = 800;
int row0 = 150, row1 = 300, row2 = 450;

// TEXTBOX code taken from https://www.youtube.com/watch?v=N753XIKAUPo
ArrayList<TEXTBOX> textboxes = new ArrayList<TEXTBOX>();
ArrayList<String> sensors = new ArrayList<String>();
boolean[] active = new boolean[6];

void addMotorToggle(ControlP5 c, String str,
                    int x, int y, int w, int h,
                    int clr) {
                      
  PFont font = createFont("Arial", 20);
  // print(PFont.list()); // see full list of available fonts
  
  c.addToggle(str, false)    // name of toggle
   .setPosition(x,y)         // upper-left corner
   .setSize(w,h)             // (width, height)
   .setFont(font)            // font type
   .setColorBackground(clr); // toggle color
}

void addTextbox(int x, int y, int w, int h) {
  TEXTBOX t = new TEXTBOX(x, y, w, h);
  textboxes.add(t);
}

void setup() {
  // Create initial window
  size(1000, 600); // same as scr_width and scr_height
  cp5 = new ControlP5(this);
  background(background_color);
  
  // Add all toggles
  textAlign(CENTER, CENTER);
  addMotorToggle(cp5, "DC\n(User)", col2, row0, but_width, but_height, #880000);
  addMotorToggle(cp5, "Stepper\n(User)", col2, row1, but_width, but_height, #008800);
  addMotorToggle(cp5, "Servo\n(User)", col2, row2, but_width, but_height, #000088);
  addMotorToggle(cp5, "DC\n(Sensor)", col3, row0, but_width, but_height, #880000);
  addMotorToggle(cp5, "Stepper\n(Sensor)", col3, row1, but_width, but_height, #008800);
  addMotorToggle(cp5, "Servo\n(Sensor)", col3, row2, but_width, but_height, #000088);
  for (int i=0; i<active.length; i++)
    active[i] = false;
  
  // Add all labels
  textFont(createFont("Arial", 20));
  textAlign(LEFT);
  textLeading(25);
  text("DC Pos (deg)",      col0, row0 + 20);
  text("DC Spd (rpm)",      col4, row0 + 20);
  text("Stepper Pos (deg)", col0, row1 + 20);
  text("Servo Pos (deg)",   col0, row2 + 20);
  
  text("Ultrasonic\nDistance (cm)", col4, row0 + text_height + 20);
  text("IR Proximity\nDistance (cm)", col4, row1 + 20);
  text("Potentiometer\nAngle (deg)", col4, row2 + 20);

  // Add all textboxes
  addTextbox(col1, row0, text_width, text_height); // DC
  addTextbox(col5, row0, text_width, text_height); // DC
  addTextbox(col1, row1, text_width, text_height); // Stepper
  addTextbox(col1, row2, text_width, text_height); // Servo
  
  
  // Add all initial sensor outputs
  sensors.add("N/A");
  sensors.add("N/A");
  sensors.add("N/A");
  
  // Step 1: run Serial.list() to find available serial ports
  // Step 2: change index of Serial.list() to match port connected to Arduino Uno 
  printArray(Serial.list());
  port = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  // Draw title and subtitles
  fill(title_color);
  textAlign(CENTER, CENTER);
  textFont(createFont("Arial", 40));
  text("MOTOR CONTROL", scr_width/2, 30);
  textFont(createFont("Arial", 30));
  text("USER CONTROLS", scr_width/4, 100);
  text("SENSOR CONTROLS", width - scr_width/4, 100);
  
  // Draw text boxes
  textAlign(LEFT);
  for (TEXTBOX t : textboxes)
    t.DRAW();
  
  // Draw sensor outputs
  fill(io_color);
  noStroke();
  rect(col5, row0 + text_height + 20, text_width, text_height);
  rect(col5, row1, text_width, text_height);
  rect(col5, row2, text_width, text_height);
  
  fill(text_color);
  textFont(createFont("Arial", 20));
  textAlign(LEFT);
  text(sensors.get(0), col5, row0 + text_height + 20, 200, 200);
  text(sensors.get(1), col5, row1, 200, 200);
  text(sensors.get(2), col5, row2, 200, 200);
  
  // Check toggle states
  checkToggles();
}


/**
  Toggle Functions (pass info to Arduino Sketch)
  lower-case means SENSOR-CONTROLLED
  upper-case means USER-CONTROLLED
**/

void mousePressed() {
  // Check if textbox clicked
  for (TEXTBOX t : textboxes) {
    t.PRESSED(mouseX, mouseY);
    if(t.selected) {
      t.Text = "";
      t.TextLength = 0;
    }
  }
}

void keyPressed() {
  for (TEXTBOX t : textboxes) {
    if (t.KEYPRESSED(key, keyCode)) {
      // use textboxes.get(0).Text;
      print("KEY PRESSED!\n");
    }
  }
}

void controlEvent(ControlEvent theEvent) {
  if(theEvent.isController()) {
    switch(theEvent.getController().getName()) {
      case "DC\n(User)":         onlyOneActive(0); break;
      case "Stepper\n(User)":    onlyOneActive(1); break;
      case "Servo\n(User)":      onlyOneActive(2); break;
      case "DC\n(Sensor)":       onlyOneActive(3); break;
      case "Stepper\n(Sensor)":  onlyOneActive(4); break;
      case "Servo\n(Sensor)":    onlyOneActive(5); break;
    }
  }
}

void onlyOneActive(int n) {
  for (int i=0; i<active.length; i++) {
    if (i == n)
      active[i] ^= true;
    else
    {
      active[i] = false;
    }
  }
}

void checkToggles() {
  if(active[0]){ // DC (User)
    port_write("D", textboxes.get(0).Text);
  }
  if(active[1]){ // Stepper (User)
    port_write("P", textboxes.get(2).Text);
  }
  if(active[2]){ // Servo (User)
    port_write("V", textboxes.get(3).Text);
  }
  if(active[3]){ // DC (Sensor)
    port_write("d", textboxes.get(1).Text);
    sensors.set(0, port_read());
  }
  if(active[4]){ // Stepper (Sensor)
    port_write("p ");
    sensors.set(1, port_read());
  }
  if(active[5]){ // Servo (Sensor)
    port_write("v ");
    sensors.set(2, port_read());
  }
  
}

void port_write(String... args) {
  for (String arg : args) {
    port.write(arg);
    print(arg + " ");
  }
  port.write('\n');
  print("\n");
}

String old_buf = "N/A";

String port_read() {
  String buffer = port.readStringUntil(10);
  if (buffer != null)
  {
    old_buf = buffer;
    return buffer;
  }
  else
    return old_buf;
}
