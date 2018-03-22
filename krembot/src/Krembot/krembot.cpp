
#include "krembot.h"

void Krembot::setup(String master_ip, uint16_t port)
{
  Serial.begin(38400);
  Particle.subscribe("spark/", &Krembot::saveMyName, this);
  Particle.subscribe("reset", &Krembot::reset, this);
  Particle.publish("spark/device/name");

pub_battery();
  //init I2C
  Wire.begin();


  //rgba & imu sensors can only be init after wire.begin
  //IMU.init();
  RgbaFront.init(uint8_t(RGBAAddr::Front));
  RgbaRear.init(uint8_t(RGBAAddr::Rear));

  RgbaFrontRight.init(uint8_t(RGBAAddr::FrontRight));
  RgbaRight.init(uint8_t(RGBAAddr::Right));
  RgbaRearRight.init(uint8_t(RGBAAddr::RearRight));
  RgbaRearLeft.init(uint8_t(RGBAAddr::RearLeft));
  RgbaLeft.init(uint8_t(RGBAAddr::Left));
  RgbaFrontLeft.init(uint8_t(RGBAAddr::FrontLeft));

  master_ip_ = master_ip;
  port_ = port;

  id_was_sent_ = false;
  master_asks_for_data_ = true; //default is to send data right away
  skip_led_gui_cmds_ = false;
  skip_base_gui_cmds_ = false;
  bump_calib_mode_ = false;

  my_name_="";
}

void Krembot::loop()
{
  //Serial.println("my name: " + my_name_);
	
	//Serial.print("front right: ");
	//RgbaFrontRight.print();
	//delay(3000);

	//delay(20000);

  if (!bump_calib_mode_)
  {
    if (!com_.isConnected())
    {
      com_.connect(master_ip_, port_);
      id_was_sent_ = false;
    }
    else
    {
      if (!id_was_sent_) //send id only once after connection
      {
        id_was_sent_ = true;
        WKCKrembot2PC wkc_msg = createWKC();
        sendWKC(wkc_msg);
      }
      else if (master_asks_for_data_ && send_data_timer_.finished())
      {
        //sendWKC();
        WKCKrembot2PC wkc_msg = createWKC();
        sendWKC(wkc_msg);
        send_data_timer_.startOver();
      }

      if (com_.bytesWaiting())
      {
        rcveWKC();
      }
    }
  }
  else
  {
    bump_calib_mode_ = !Bumpers.calib();
    if (!bump_calib_mode_)
      Led.write(0, 0, 0);
  }

 

}


void Krembot::saveMyName(const char *topic, const char *data)
{
  Serial.println("received " + String(topic) + ": " + String(data));
  my_name_ = String(data);

}

void Krembot::sendWKC(WKCKrembot2PC& wkc_msg)
{
  byte buff[wkc_msg.size()];
  wkc_msg.toBytes(buff);
  com_.write(buff, wkc_msg.size());
}

WKCKrembot2PC Krembot::createWKC()
{
  //build WKC msg
  WKCKrembot2PC wkc_msg;

  //rgba sensors
  wkc_msg.rgba_front = RgbaFront.read();
  wkc_msg.rgba_rear = RgbaRear.read();
  wkc_msg.rgba_right = RgbaRight.read();
  wkc_msg.rgba_left = RgbaLeft.read();
  wkc_msg.rgba_front_right = RgbaFrontRight.read();
  wkc_msg.rgba_front_left = RgbaFrontLeft.read();
  wkc_msg.rgba_rear_right = RgbaRearRight.read();
  wkc_msg.rgba_rear_left = RgbaRearLeft.read();

  wkc_msg.bumps = Bumpers.read();

  wkc_msg.bat_lvl = Bat.getBatLvl();
  wkc_msg.bat_chrg_lvl = Bat.getChargeLvl();
  wkc_msg.is_bat_chrgng = Bat.isCharging();
  wkc_msg.is_bat_full = Bat.isFull();

  return wkc_msg;
}

void Krembot::rcveWKC()
{
  WKCPC2Krembot wkc_msg;
  byte buff[wkc_msg.size()];
  com_.read(buff, wkc_msg.size());
  wkc_msg.fromBytes(buff);
  handleWKCFromPC(wkc_msg);
}

void Krembot::handleWKCFromPC(WKCPC2Krembot wkc_msg)
{
  wkc_msg.print();

  if (wkc_msg.data_req)
  {
    master_asks_for_data_ = true;
    send_data_timer_.start(SEND_DATA_INTERVAL);
  }
  else
    master_asks_for_data_ = false;

  if (wkc_msg.toggle_led)
  {
    Led.write(wkc_msg.led_red,
              wkc_msg.led_green,
              wkc_msg.led_blue);
    skip_led_gui_cmds_ = false;
  }
  else if (!skip_led_gui_cmds_)
  {
    skip_led_gui_cmds_ = true;
    Led.write(0, 0, 0);
  }

  if (wkc_msg.joy_control)
  {
    int8_t linear_vel = wkc_msg.linear_vel_positive? wkc_msg.joy_x : -wkc_msg.joy_x;
    int8_t angular_vel = wkc_msg.angular_vel_positive? wkc_msg.joy_y : -wkc_msg.joy_y;
    Base.drive(linear_vel, angular_vel);
    skip_base_gui_cmds_ = false;
  }
  else if (!skip_base_gui_cmds_)
  {
    //Base.driveJoyCmd(128,128);
    Base.stop();
    skip_base_gui_cmds_ = true;
  }

  if (wkc_msg.base_offset)
  {
    EEPROM.write(BASE_RIGHT_OFFSET_ADDR, wkc_msg.base_right_offset);
    EEPROM.write(BASE_LEFT_OFFSET_ADDR, wkc_msg.base_left_offset);
    Base.drive(wkc_msg.joy_x, wkc_msg.joy_y); //call drive again to apply offsets
  }

  if (wkc_msg.bumps_calib)
  {
    Led.write(0, 255, 0);
    bump_calib_mode_ = true;
  }

  if (wkc_msg.user_msg_size > 0) //get user message
  {
    byte user_msg_buff[wkc_msg.user_msg_size];
    com_.read(user_msg_buff, wkc_msg.user_msg_size);
    for (int i=0; i < wkc_msg.user_msg_size; i++)
    {
      Serial.print((char)user_msg_buff[i]); //TODO: delete msg printing after done testing
    }
    Serial.println();
    //TODO: do something with user_msg_buff - contains user msg
  }
}
