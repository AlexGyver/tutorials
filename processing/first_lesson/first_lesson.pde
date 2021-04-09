int rad = 200;
int amount = 10;
float x[] = new float[amount];
float y[] = new float[amount];
float velX[] = new float[amount];
float velY[] = new float[amount];
float R[] = new float[amount];
color col[] = new color[amount];

void setup() {
  size(600, 400, P3D);
  smooth(8);
  colorMode(HSB, 255, 255, 255);
  //frameRate(300);
  //noStroke(); 

  for (int i = 0; i < amount; i++) {
    R[i] = random(5, 10);
    x[i] = random(R[i], width - R[i]);
    y[i] = random(R[i], height - R[i]);
    velX[i] = random(2, 6) * ((int)random(0, 2) == 0 ? 1 : -1);
    velY[i] = random(2, 6) * ((int)random(0, 2) == 0 ? 1 : -1);
    col[i] = color(random(0, 255), random(0, 255), random(0, 255));
  }
}

void draw() {
  background(200); 
  //rotateX(radians(60));
  //translate(0, -250, -200);
  //noFill();
  //stroke(0);
  //strokeWeight(2);
  //rect(10, 10, width-20, height-20);


  stroke(0);

  for (int i = 0; i < amount; i++) {
    x[i] += velX[i];
    y[i] += velY[i];
    if (x[i] < R[i] || x[i] > width-R[i]) velX[i] = -velX[i];
    if (y[i] < R[i] || y[i] > height-R[i]) velY[i] = -velY[i];
  }
  for (int i = 0; i < amount; i++) {
    for (int j = 0; j < i; j++) {
      float distance = dist(x[i], y[i], x[j], y[j]);
      if (distance < rad) {
        strokeWeight(map(distance, 0, rad, 4, 0.2));
        //stroke(map(distance, 0, rad, 90, 0), 255, 180);
        line(x[i], y[i], x[j], y[j]);
      }
    }
  }

  for (int i = 0; i < amount; i++) {   
    noStroke();
    fill(0);
    circle(x[i], y[i], R[i]*2);
  }
}
