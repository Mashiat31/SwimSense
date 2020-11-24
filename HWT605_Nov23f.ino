//#include <HWT605.h>

// HWT605 main
//#include <Wire.h>
byte tot_data_rx[10];
byte data_rx[9];
byte data_sent[5];
byte check_sum;

//------The following is extracted data for use--------

//-----------Upper arm---------------
// upper arm acceleration
float ua_acc_x;   //    unit:m/s^2
float ua_acc_y;   //    unit:m/s^2
float ua_acc_z;   //    unit:m/s^2
float ua_acc_tem; //  unit:cels
//upper arm angular velocity
float ua_av_x;   //     unit:degree/s
float ua_av_y;   //     unit:degree/s
float ua_av_z;   //     unit:degree/s
float ua_av_tem; //   unit:cels
//upper arm angle
float ua_an_roll; //     unit:degree
float ua_an_pit;  //     unit:degree
float ua_an_yaw;  //     unit:degree
float ua_an_tem;  //  unit:cels

//-----------Lower arm---------------
//lower arm acceleration
float la_acc_x;   //    unit:m/s^2
float la_acc_y;   //    unit:m/s^2
float la_acc_z;   //    unit:m/s^2
float la_acc_tem; //  unit:cels

//lower arm angular velocity
float la_av_x;   //     unit:degree/s
float la_av_y;   //     unit:degree/s
float la_av_z;   //     unit:degree/s
float la_av_tem; //   unit:cels

//lower arm angle
float la_an_roll; //     unit:degree
float la_an_pit;  //     unit:degree
float la_an_yaw;  //     unit:degree
float la_an_tem;  //  unit:cels


//-----------Belly---------------
//belly acceleration
float acc_x;   //    unit:m/s^2
float acc_y;   //    unit:m/s^2
float acc_z;   //    unit:m/s^2
float acc_tem; //  unit:cels

//belly angular velocity
float av_x;   //     unit:degree/s
float av_y;   //     unit:degree/s
float av_z;   //     unit:degree/s
float av_tem; //   unit:cels

//belly angle 
float an_roll; //     unit:degree
float an_pit;  //     unit:degree
float an_yaw;  //     unit:degree
float an_tem;  //  unit:cels

//-----END of extracted data


void setup()
{
  // put your setup code here, to run once:
  pinMode(14, OUTPUT); // tx3 (belly)
  pinMode(15, INPUT);  // rx3

  pinMode(16, INPUT);  // rx4 (upper arm)
  pinMode(17, OUTPUT); // tx4

  pinMode(20, OUTPUT); //tx5  (lower arm)
  pinMode(21, INPUT);  // rx5

  Serial3.begin(9600);//  belly
  Serial4.begin(9600); //ua (upper arm)
  Serial5.begin(9600); // la (lower arm)
}

void loop()
{

  //json code for belly, upper arm and lower arm data seperately

  // String data_acceleration = String("\"acc\": {\"x\":"+String(acc_x, 2)+","+"\"y\":"+String(acc_y, 2)+","+"\"z\":"+String(acc_z, 2)+"}");
  // String data_angular_velocity = String("\"av\": {\"x\":"+String(av_x,2)+","+"\"y\":"+String(av_y, 2)+","+"\"z\":"+String(av_z, 2)+"}");
  // String data_angle = String("\"ang\": {\"roll\":"+String(an_roll, 2)+","+"\"pit\":"+String(an_pit, 2)+","+"\"yaw\":"+String(an_yaw, 2)+"}");
  // String json = String("{"+data_acceleration+","+data_angular_velocity+","+data_angle+"}");
  // Serial.print(json);
  // Serial.print('\n');
  
  
   String data_acceleration_ua = String("\"ua_acc\": {\"x\":"+String(ua_acc_x, 2)+","+"\"y\":"+String(ua_acc_y, 2)+","+"\"z\":"+String(ua_acc_z, 2)+"}");
   String data_angular_velocity_ua = String("\"ua_av\": {\"x\":"+String(ua_av_x,2)+","+"\"y\":"+String(ua_av_y, 2)+","+"\"z\":"+String(ua_av_z, 2)+"}");
   String data_angle_ua = String("\"ua_ang\": {\"roll\":"+String(ua_an_roll, 2)+","+"\"pit\":"+String(ua_an_pit, 2)+","+"\"yaw\":"+String(ua_an_yaw, 2)+"}");
 //  String json_ua = String("{"+data_acceleration_ua+","+data_angular_velocity_ua+","+data_angle_ua+"}" + ",");
//   Serial.print(json_ua);
  // Serial.print('\n');


   String data_acceleration_la = String("\"la_acc\": {\"x\":"+String(la_acc_x, 2)+","+"\"y\":"+String(la_acc_y, 2)+","+"\"z\":"+String(la_acc_z, 2)+"}");
   String data_angular_velocity_la = String("\"la_av\": {\"x\":"+String(la_av_x,2)+","+"\"y\":"+String(la_av_y, 2)+","+"\"z\":"+String(la_av_z, 2)+"}");
   String data_angle_la = String("\"la_ang\": {\"roll\":"+String(la_an_roll, 2)+","+"\"pit\":"+String(la_an_pit, 2)+","+"\"yaw\":"+String(la_an_yaw, 2)+"}");
//   String json = String("{"+data_acceleration_ua+","+data_angular_velocity_ua+","+data_angle_ua +"," +data_acceleration_la+","+data_angular_velocity_la+","+data_angle_la+"}");
   String json = String("{"+data_acceleration_ua+","+data_angular_velocity_ua+","+data_angle_ua +"," +data_acceleration_la+","+data_angular_velocity_la+","+data_angle_la+"}");
   Serial.println(json);
  // Serial.print('\n');

  //--------Begin of receive and show data----
  rx_b(); // receive data from sensor connected to b (belly)
  
  // display belly acceleration, angular velocity and angle seperately.(uncomment them for display)
  
  //disp_Acc();  
  //disp_Av();
  //disp_An();
  
  rx_ua();  // receive data from sensor connected to upper arm

  // display upper arm acceleration, angular velocity and angle seperately.(uncomment them for display)
  
  //disp_Acc_ua();
  //disp_Av_ua();
  // disp_An_ua();

  rx_la();  // receiver data from lower arm sensor

  // display lower arm acceleration, angular velocity and angle seperately.(uncomment them for display)

  //disp_Acc_la();
  // disp_Av_la();
  // disp_An_la();
}

// rx_onepack is for testing error
void rx_onepack()
{
  while (Serial3.read() != 0x55)
  {
  };
  //while(Serial3.read()!=0x50){Serial.print("Wait0x50");};
  Serial3.readBytes(data_rx, 10);
}

// data convert from raw data to redable data
void data_Acc()
{
  acc_x = (((short)(data_rx[1] << 8) | data_rx[0]) / 32768.0) * 16 * 9.8;
  acc_y = (((short)(data_rx[3] << 8) | data_rx[2]) / 32768.0) * 16 * 9.8;
  acc_z = (((short)(data_rx[5] << 8) | data_rx[4]) / 32768.0) * 16 * 9.8;
  acc_tem = ((short)(data_rx[7] << 8) | data_rx[6]) / 100.0;
}

void data_Av()
{
  av_x = (((short)(data_rx[1] << 8) | data_rx[0]) / 32768.0) * 2000;
  av_y = (((short)(data_rx[3] << 8) | data_rx[2]) / 32768.0) * 2000;
  av_z = (((short)(data_rx[5] << 8) | data_rx[4]) / 32768.0) * 2000;
  av_tem = ((short)(data_rx[7] << 8) | data_rx[6]) / 100.0;
}

void data_An()
{
  an_roll = (((short)(data_rx[1] << 8) | data_rx[0]) / 32768.0) * 180;
  an_pit = (((short)(data_rx[3] << 8) | data_rx[2]) / 32768.0) * 180;
  an_yaw = (((short)(data_rx[5] << 8) | data_rx[4]) / 32768.0) * 180;
  an_tem = ((short)(data_rx[7] << 8) | data_rx[6]) / 100.0;
}

// receiver data from serial3(for belly now)
void rx_b()
{
 char read_data;

  //char head[] = {0x51, 0x52, 0x053, 0x54};

  for (int i = 0; i < 3; i++)
  {
  wait:
    while (Serial3.read()!=0x55)
    {
    };
    Serial3.readBytes(tot_data_rx,10);
    //Serial.println(tot_data_rx[0],HEX);
    read_data = (char)tot_data_rx[0];
    for(int j = 0;j<8;j++)
    {
      data_rx[j] = tot_data_rx[j+1];
    }
    switch (read_data)
    {
    case 0x51:
      data_Acc();
      break;
    case 0x52:
      data_Av();
      break;
    case 0x53:
      data_An();
      break;
    case 0x54:
      break;
    default:
      goto wait;
      break;
    }
  }
}

//---------Serial4 (upper arm)

//for testing
void rx4_onepack()
{
  while (Serial4.read() != 0x55)
  {
  };
  //while(Serial3.read()!=0x50){Serial.print("Wait0x50");};
  Serial4.readBytes(data_rx, 10);
}
void visual4_disp()
{
  for (int i = 0; i < 9; i++)
  {
    Serial.print(data_rx[i], HEX);
    Serial.print(" ");
  };
  Serial.println();
}

// data convert from raw data to redable data

void data_Acc_ua()
{
  ua_acc_x = (((short)(data_rx[1] << 8) | data_rx[0]) / 32768.0) * 16 * 9.8;
  ua_acc_y = (((short)(data_rx[3] << 8) | data_rx[2]) / 32768.0) * 16 * 9.8;
  ua_acc_z = (((short)(data_rx[5] << 8) | data_rx[4]) / 32768.0) * 16 * 9.8;
  ua_acc_tem = ((short)(data_rx[7] << 8) | data_rx[6]) / 100.0;
 
}
void data_Av_ua()
{
  ua_av_x = (((short)(data_rx[1] << 8) | data_rx[0]) / 32768.0) * 2000;
  ua_av_y = (((short)(data_rx[3] << 8) | data_rx[2]) / 32768.0) * 2000;
  ua_av_z = (((short)(data_rx[5] << 8) | data_rx[4]) / 32768.0) * 2000;
  ua_av_tem = ((short)(data_rx[7] << 8) | data_rx[6]) / 100.0;
}
void data_An_ua()
{
  ua_an_roll = (((short)(data_rx[1] << 8) | data_rx[0]) / 32768.0) * 180;
  ua_an_pit = (((short)(data_rx[3] << 8) | data_rx[2]) / 32768.0) * 180;
  ua_an_yaw = (((short)(data_rx[5] << 8) | data_rx[4]) / 32768.0) * 180;
  ua_an_tem = ((short)(data_rx[7] << 8) | data_rx[6]) / 100.0;
}


// receiver data from serial4(for upper arm now)

void rx_ua()
{
  char read_data;

  //char head[] = {0x51, 0x52, 0x053, 0x54};

  for (int i = 0; i < 3; i++)
  {
  wait_1:
    while (Serial4.read()!=0x55)
    {
    };
    Serial4.readBytes(tot_data_rx,10);
    //Serial.println(tot_data_rx[0],HEX);
    read_data = (char)tot_data_rx[0];
    for(int j = 0;j<8;j++)
    {
      data_rx[j] = tot_data_rx[j+1];
    }
    switch (read_data)
    {
    case 0x51:
      data_Acc_ua();
      break;
    case 0x52:
      data_Av_ua();
      break;
    case 0x53:
      data_An_ua();
      break;
    case 0x54:
      break;
    default:
      goto wait_1;
      break;
    }
  }
}
//---------Serial5

// for testing
void rx5_onepack()
{
  while (Serial5.read() != 0x55)
  {
  };
  //while(Serial3.read()!=0x50){Serial.print("Wait0x50");};
  Serial5.readBytes(data_rx, 10);
}
void visual5_disp()
{
  for (int i = 0; i < 9; i++)
  {
    Serial.print(data_rx[i], HEX);
    Serial.print(" ");
  };
  Serial.println();
}
//// receiver data from serial5(for lower arm now)
void rx_la()
{
  char read_data;

  //char head[] = {0x51, 0x52, 0x053, 0x54};

  for (int i = 0; i < 3; i++)
  {
  wait_2:
    while (Serial5.read()!=0x55)
    {
    };
    Serial5.readBytes(tot_data_rx,10);
    //Serial.println(tot_data_rx[0],HEX);
    read_data = (char)tot_data_rx[0];
    for(int j = 0;j<8;j++)
    {
      data_rx[j] = tot_data_rx[j+1];
    }
    switch (read_data)
    {
    case 0x51:
      data_Acc_la();
      break;
    case 0x52:
      data_Av_la();
      break;
    case 0x53:
      data_An_la();
      break;
    case 0x54:
      break;
    default:
      goto wait_2;
      break;
    }
  }
}
//convert to redable data
void data_Acc_la()
{
  la_acc_x = (((short)(data_rx[1] << 8) | data_rx[0]) / 32768.0) * 16 * 9.8;
  la_acc_y = (((short)(data_rx[3] << 8) | data_rx[2]) / 32768.0) * 16 * 9.8;
  la_acc_z = (((short)(data_rx[5] << 8) | data_rx[4]) / 32768.0) * 16 * 9.8;
  la_acc_tem = ((short)(data_rx[7] << 8) | data_rx[6]) / 100.0;
}
void data_Av_la()
{
  la_av_x = (((short)(data_rx[1] << 8) | data_rx[0]) / 32768.0) * 2000;
  la_av_y = (((short)(data_rx[3] << 8) | data_rx[2]) / 32768.0) * 2000;
  la_av_z = (((short)(data_rx[5] << 8) | data_rx[4]) / 32768.0) * 2000;
  la_av_tem = ((short)(data_rx[7] << 8) | data_rx[6]) / 100.0;
}
void data_An_la()
{
  la_an_roll = (((short)(data_rx[1] << 8) | data_rx[0]) / 32768.0) * 180;
  la_an_pit = (((short)(data_rx[3] << 8) | data_rx[2]) / 32768.0) * 180;
  la_an_yaw = (((short)(data_rx[5] << 8) | data_rx[4]) / 32768.0) * 180;
  la_an_tem = ((short)(data_rx[7] << 8) | data_rx[6]) / 100.0;
}

//testing
void visual_disp()
{
  for (int i = 0; i < 9; i++)
  {
    Serial.print(data_rx[i], HEX);
    Serial.print(" ");
  };
  Serial.println();
}

//disp belly acceleration,angular velocity, angle

void disp_Acc()
{
  Serial.println("The acceleration of belly:");
  Serial.print("acc_x = ");
  Serial.print(acc_x, DEC);
  Serial.print(" ");
  Serial.print("acc_y = ");
  Serial.print(acc_y, DEC);
  Serial.print(" ");
  Serial.print("acc_z = ");
  Serial.print(acc_z, DEC);
  Serial.print(" ");
  //Serial.print("tem = ");
  //Serial.print(acc_tem, DEC);
  Serial.println();
}
void disp_Av()
{
  Serial.println("The Angular velocity of belly:");
  Serial.print("Av_x = ");
  Serial.print(av_x, DEC);
  Serial.print(" ");
  Serial.print("Av_y = ");
  Serial.print(av_y, DEC);
  Serial.print(" ");
  Serial.print("Av_z = ");
  Serial.print(av_z, DEC);
  Serial.print(" ");
  //Serial.print("tem = ");
  //Serial.print(av_tem, DEC);
  Serial.println();
}

void disp_An()
{
  Serial.println("The Angle of belly:");
  Serial.print("An_roll = ");
  Serial.print(an_roll, DEC);
  Serial.print(" ");
  Serial.print("An_pit = ");
  Serial.print(an_pit, DEC);
  Serial.print(" ");
  Serial.print("An_yaw = ");
  Serial.print(an_yaw, DEC);
  Serial.print(" ");
  //Serial.print("tem = ");
  //Serial.print(an_tem, DEC);
  Serial.println();
}

//disp upper arm acceleration,angular velocity, angle
void disp_Acc_ua()
{
  Serial.println("The acceleration of upper arm:");
  Serial.print("acc_x = ");
  Serial.print(ua_acc_x, DEC);
  Serial.print(" ");
  Serial.print("acc_y = ");
  Serial.print(ua_acc_y, DEC);
  Serial.print(" ");
  Serial.print("acc_z = ");
  Serial.print(ua_acc_z, DEC);
  Serial.print(" ");
  //Serial.print("tem = ");
  //Serial.print(ua_acc_tem, DEC);
  Serial.println();
}
void disp_Av_ua()
{
  Serial.println("The Angular velocity of upper arm:");
  Serial.print("Av_x = ");
  Serial.print(ua_av_x, DEC);
  Serial.print(" ");
  Serial.print("Av_y = ");
  Serial.print(ua_av_y, DEC);
  Serial.print(" ");
  Serial.print("Av_z = ");
  Serial.print(ua_av_z, DEC);
  Serial.print(" ");
  //Serial.print("tem = ");
  //Serial.print(ua_av_tem, DEC);
  Serial.println();
}

void disp_An_ua()
{
  Serial.println("The Angle of upper arm:");
  Serial.print("An_roll = ");
  Serial.print(ua_an_roll, DEC);
  Serial.print(" ");
  Serial.print("An_pit = ");
  Serial.print(ua_an_pit, DEC);
  Serial.print(" ");
  Serial.print("An_yaw = ");
  Serial.print(ua_an_yaw, DEC);
  Serial.print(" ");
  //Serial.print("tem = ");
  //Serial.print(ua_an_tem, DEC);
  Serial.println();
}
//disp lower arm acceleration,angular velocity, angle
void disp_Acc_la()
{
  Serial.println("The acceleration of lower arm:");
  Serial.print("acc_x = ");
  Serial.print(la_acc_x, DEC);
  Serial.print(" ");
  Serial.print("acc_y = ");
  Serial.print(la_acc_y, DEC);
  Serial.print(" ");
  Serial.print("acc_z = ");
  Serial.print(la_acc_z, DEC);
  Serial.print(" ");
  //Serial.print("tem = ");
  //Serial.print(la_acc_tem, DEC);
  Serial.println();
}
void disp_Av_la()
{
  Serial.println("The Angluar velocity of lower arm:");
  Serial.print("Av_x = ");
  Serial.print(la_av_x, DEC);
  Serial.print(" ");
  Serial.print("Av_y = ");
  Serial.print(la_av_y, DEC);
  Serial.print(" ");
  Serial.print("Av_z = ");
  Serial.print(la_av_z, DEC);
  Serial.print(" ");
  //Serial.print("tem = ");
  //Serial.print(la_av_tem, DEC);
  Serial.println();
}

void disp_An_la()
{
  Serial.println("The Angle of lower arm:");
  Serial.print("An_roll = ");
  Serial.print(la_an_roll, DEC);
  Serial.print(" ");
  Serial.print("An_pit = ");
  Serial.print(la_an_pit, DEC);
  Serial.print(" ");
  Serial.print("An_yaw = ");
  Serial.print(la_an_yaw, DEC);
  Serial.print(" ");
  //Serial.print("tem = ");
  //Serial.print(la_an_tem, DEC);
  Serial.println();
}
