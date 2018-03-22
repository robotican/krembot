
unsigned long pub_t=0;

bool init_color=false;

bool low_bat=false;

bool set_color_after_name(Krembot *k) {

  if (!init_color && k->have_name()) {
    //if got name and set 1-9 blue and 10 red
    if (k->getName().compareTo("Krembot10")==0) k->Led.write(255,0,0);
    else if (k->getName().compareTo("Krembot9")==0) k->Led.write(0,255,0);
    else k->Led.write(0,0,255);
    init_color=true;
  }
  return init_color;
}
void pub_RGBAD(String event,RGBAResult sensor,int interval) {
  if (millis()-pub_t>interval) {
    char str[100];
    sprintf(str,"R:%d   G:%d   B:%d   A:%d   D:%.1f",sensor.Red,sensor.Green,sensor.Blue,sensor.Ambient,sensor.Distance);
    Particle.publish(event,str);
    pub_t=millis();
  }
}


void init_dance(Krembot *k) {

  k->Led.write(255,0,0);
  delay(200);
  k->Led.write(0,255,0);
  delay(200);
  k->Led.write(0,0,255);
  delay(200);
  k->Led.write(255,255,255);
  delay(200);
  k->Base.drive(0,50);
  delay(200);
  k->Base.drive(0,-50);
  delay(200);
  k->Led.write(0,0,0);
  k->Base.stop();


}


void escape_obstacles(Krembot *k) {
  float f_dist=k->RgbaFront.read().Distance;
  int linear=30;
  int angular=0;
  if (f_dist<12)  angular=90; //hard turn left

  float fl_dist=k->RgbaFrontLeft.read().Distance;
  if (fl_dist<10)  angular=-70; //turn right

  float fr_dist=k->RgbaFrontRight.read().Distance;
  if (fr_dist<10)  angular=70; //turn left

  //if (angular!=0) k->Led.write(255,0,0);
  //else k->Led.write(0,255,0);

  k->Base.drive(linear,angular);
}


void catch_red_disease(Krembot *k) {

  if (k->getName().compareTo("Krembot9")==0) return; //green is immune

  int min_color=10000;
  int min_dist=10;

  float dist=k->RgbaFront.read().Distance;
  float color=k->RgbaFront.read().Red;
  if (dist<=min_dist && color>min_color)   k->Led.write(255,0,0);

  dist=k->RgbaLeft.read().Distance;
  color=k->RgbaLeft.read().Red;
  if (dist<=min_dist && color>min_color)     k->Led.write(255,0,0);

  dist=k->RgbaRight.read().Distance;
  color=k->RgbaRight.read().Red;
  if (dist<=min_dist && color>min_color)  k->Led.write(255,0,0);

  dist=k->RgbaRear.read().Distance;
  color=k->RgbaRear.read().Red;
  if (dist<=min_dist && color>min_color)  k->Led.write(255,0,0);

  dist=k->RgbaRearLeft.read().Distance;
  color=k->RgbaRearLeft.read().Red;
  if (dist<=min_dist && color>min_color)  k->Led.write(255,0,0);

  dist=k->RgbaFrontLeft.read().Distance;
  color=k->RgbaFrontLeft.read().Red;
  if (dist<=min_dist && color>min_color)   k->Led.write(255,0,0);

  dist=k->RgbaFrontRight.read().Distance;
  color=k->RgbaFrontRight.read().Red;
  if (dist<=min_dist && color>min_color)    k->Led.write(255,0,0);

  dist=k->RgbaRearRight.read().Distance;
  color=k->RgbaRearRight.read().Red;
  if (dist<=min_dist && color>min_color)     k->Led.write(255,0,0);

}

void heal_by_green(Krembot *k) {


  if (k->getName().compareTo("Krembot10")==0) {
    return; //init red cant be cured
  }
  else if (k->getName().compareTo("Krembot9")==0) {
    return; //green will stay green
  }

  int min_color=9000;
  int min_dist=10;

  float dist=k->RgbaFront.read().Distance;
  float color=k->RgbaFront.read().Green;
  if (dist<=min_dist && color>min_color)   k->Led.write(0,0,255);

  dist=k->RgbaLeft.read().Distance;
  color=k->RgbaLeft.read().Green;
  if (dist<=min_dist && color>min_color)   k->Led.write(0,0,255);

  dist=k->RgbaRight.read().Distance;
  color=k->RgbaRight.read().Green;
  if (dist<=min_dist && color>min_color)  k->Led.write(0,0,255);

  dist=k->RgbaRear.read().Distance;
  color=k->RgbaRear.read().Green;
  if (dist<=min_dist && color>min_color)   k->Led.write(0,0,255);

  dist=k->RgbaRearLeft.read().Distance;
  color=k->RgbaRearLeft.read().Green;
  if (dist<=min_dist && color>min_color)  k->Led.write(0,0,255);

  dist=k->RgbaFrontLeft.read().Distance;
  color=k->RgbaFrontLeft.read().Green;
  if (dist<=min_dist && color>min_color)  k->Led.write(0,0,255);

  dist=k->RgbaFrontRight.read().Distance;
  color=k->RgbaFrontRight.read().Green;
  if (dist<=min_dist && color>min_color)   k->Led.write(0,0,255);

  dist=k->RgbaRearRight.read().Distance;
  color=k->RgbaRearRight.read().Green;
  if (dist<=min_dist && color>min_color)   k->Led.write(0,0,255);

}

void check_reset_by_bumpers(Krembot *k) {
  BumpersRes bumpers=k->Bumpers.read();
  if(bumpers.front && bumpers.rear && bumpers.left && bumpers.right) System.reset();
}


bool check_low_bat(Krembot *k,int min_level) {
  int level=k->Bat.getBatLvl();
  if (level<min_level) {

    if (!low_bat) {
k->pub_battery();
      k->Led.write(50,50,0);
      k->Base.stop();
      init_color=false;
    }
      low_bat=true;

  }
  else low_bat=false;

  return low_bat;
}


void demo_loop(Krembot *k){
  if (!k->Bat.isCharging()) {
    if (!check_low_bat(k,10)) { //10%

      if (set_color_after_name(k)) {

        escape_obstacles(k);

        catch_red_disease(k);

        heal_by_green(k);

        check_reset_by_bumpers(k); //reset if all 4 bumbers are pressed
      }
    }
  }
  else k->Base.stop();
}
