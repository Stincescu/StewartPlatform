#include <math.h>

class Motor {
  public:
    int16_t pos;
    int16_t req; //movement request
    uint8_t state;
    uint8_t pin_dir;
    uint8_t pin_step;
    uint8_t pin_index;
    Motor(uint8_t pin_dir, uint8_t pin_step, uint8_t pin_index ) {
      this->pin_dir = pin_dir;
      this->pin_step = pin_step;
      this->pin_index = pin_index;
      init();
    }
    void init() {
      pinMode(pin_dir, OUTPUT);
      pinMode(pin_step, OUTPUT);
      pinMode(pin_index, INPUT);
      state = 0;      
      req = 0;
    }
    bool Rising() {
      if(req) {
        if(req < 0)
        {
          digitalWrite(pin_dir,LOW);
          req++;
          pos--;
          if(pos<0)
          {
            pos = 2999;
          }
        }
        else
        {
          digitalWrite(pin_dir,HIGH);
          req--;
          pos++;
          if(pos>=3000)
          {
            pos=0;
          }
        }
        digitalWrite(pin_step, HIGH);
        return true;
      }
      return false;
    }
    void Falling() {      
        digitalWrite(pin_step,LOW);
    }
    bool Attempt_sync() {
      if(state == 0 && digitalRead(pin_index))
      {
        pos = 0;
        req = 0;
        state = 1;
        return true;
      }
      return false;
    }
}; 
class MotorManager {  
  static const uint16_t half_per_us = (1000000/1111)/2;
  public:
    uint8_t n;
    Motor *m;
  MotorManager(Motor *m, uint8_t n) {
    this->m = m;
    this->n = n;
  }
  bool Actuate() {
    bool r=false;
    for(uint8_t i=0; i<n; i++) { //prepare the rising edge of the pulse for all motors
      r |= m[i].Rising();
    }
    delayMicroseconds(half_per_us);//generate one pulse interval (for all active motors)
    for(uint8_t i=0; i<n; i++) { //Generate the falling edge of the pulse for each motor
      m[i].Falling();
    }
    return r;
  }
  void Accomplish(uint32_t pulse_low_us=half_per_us) {
    while(Actuate()) delayMicroseconds(pulse_low_us);
  }
  bool Sync(uint16_t max_steps=1500, uint32_t pulse_low_us=15*half_per_us) {
    for(int i = 0;i<n;i++)
    {
      if(i%2 == 0)      {
        m[i].req = -max_steps;
      }
      else {
         m[i].req = max_steps;
      }
    }
    while(Actuate()) {
      delayMicroseconds(pulse_low_us);
      for(int i = 1;i<n;i+=2){
        if(m[i].Attempt_sync()) {
          m[(i+1)%n].req = 0;      
        }
      }
    }
    bool r=true;
    for(int i = 1;i<n;i+=2){
      r &= (m[i].state == 1);
    }
    return r;
  }
  void Request(int16_t *data, bool rev=false, uint32_t pulse_low_us=15*half_per_us) {
    uint32_t low = pulse_low_us << 3;
    for(int i=0; i<n; i++)
    {      
      m[i].req = data[i];
      if(rev) {
        m[i].req = -m[i].req;
      }
    }
    while(Actuate()) { delayMicroseconds(low); if(low>pulse_low_us) low-=pulse_low_us; }
  }
};

double gamma[6] = {3.0658, 2.1702, 0.9714, 0.0758, -1.1198, -2.0154};
double x, y, z;
double roll = 0, pitch = 0, yaw = 0;
double D = 500, Rm = 100; //milimetri
double rotationMatrix[3][3];
double pi_original[3][6] = {
               {
                -455.79, -75, 75, 455.79, 380.79, -380.79  //coords X
               },
               {
                -176.54, 483, 483, -176.54, -306.45, -306.45    //coords Y
               },
               {
                580, 580, 580, 580, 580, 580                                    //coords Z
               }
};
double bi[3][6] = {
               {-547.674, -309.863, 309.863, 547.674, 237.810, -237.810       //coords X
               },
               { 41.599, 453.500, 453.500, 41.599, -495.099, -495.099    //coords Y
               },
               {101, 101, 101, 101, 101, 101                                 //coords Z
               }
};
double li[6];
double Ai[6], Bi[6], Ci[6];
double deltai[6] = {0,0,0,0,0,0};
int steps[6] = {0,0,0,0,0,0};
double pi[3][6]; //copie a lui pi_original ca sa am fixe coordonatele platformei

void setup() {
  Serial.begin(115200);
  Motor m[] = {Motor(13,2,A0), Motor(3,4,A1), Motor(5,6,A1), Motor(7,8,A2), Motor(9,10,A2), Motor(11,12,A0)};
  MotorManager mm = MotorManager(m, 6);
//Ridicare de la sol  
  if(!mm.Sync()) return; //gracefully return if init unsuccessful (e.g. bad sensor reading...)
  
  delay(2000);
  
  //Calcul comenzi
         
//TRANSLATIE PE X + la -        
    calculateCommands(75,0,0,0,0,0);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);
   
   calculateCommands(-75,0,0,0,0,0);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);
    
//TRANSLATIE PE Y + la -
    calculateCommands(0,75,0,0,0,0);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);
        
    calculateCommands(0,-75,0,0,0,0);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);
          
 //TRANSLATIE PE Z + la -
    calculateCommands(0,0,50,0,0,0);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);

    calculateCommands(0,0,-60,0,0,0);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);
    
//ROTATIE PE Z + la -
    calculateCommands(0,0,0,0.15,0,0);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);
        
    calculateCommands(0,0,0,-0.15,0,0);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);
    
//ROTATIE PE Y + la -  
    calculateCommands(0,0,0,0,0.05,0);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);
        
    calculateCommands(0,0,0,0,-0.06,0);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);
    
 //ROTATIE PE X + la - 
    calculateCommands(0,0,0,0,0,0.08);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);        

    calculateCommands(0,0,0,0,0,-0.08);
    mm.Request(steps);
    delay(2000); 
    mm.Request(steps, true);
    delay(2000);

    calculateCommands(0,0,-120,0,0,0);
    mm.Request(steps);
    delay(2000); 
}
void loop() {
   //MUST BE EMPTY
}
void calculateCommands(double X, double Y, double Z, double YAW, double PITCH, double ROLL)
{
  x = X;
  y = Y;
  z = Z;
  roll = ROLL;
  pitch = PITCH;
  yaw = YAW;
  
  calculateRotationMatrix();
  adjustPiWithTranslation();
  calculateLegLength();
  calculate_a_b_c();
  calculate_delta();
  
  for(int i = 0; i<6; i++)
  {
    steps[i] = deltai[i] * 57296 / 1000 / 0.12;
  }
}
void calculate_delta()
{
  for(int i = 0; i<6; i++)
  {
    deltai[i] = asin(Ci[i]/(sqrt(pow(Ai[i],2) + pow(Bi[i],2)))) - atan2(Bi[i],Ai[i]);
    if(isnan(deltai[i]) && Ci[i] < 0)
    {
      deltai[i] = -1.5708 - atan2(Bi[i],Ai[i]);
    }
    if(isnan(deltai[i]) && Ci[i] > 0)
    {
      deltai[i] = 1.5708 - atan2(Bi[i],Ai[i]);
    }
  }
}
void calculate_a_b_c()
{
  for(int i = 0; i<6; i++)
  {
      Ai[i] = 2 * Rm * (pi[2][i] - bi[2][i]);
      Bi[i] = 2 * Rm * (sin(gamma[i]) * (pi[0][i] - bi[0][i]) - cos(gamma[i]) * (pi[1][i] - bi[1][i]));
      Ci[i] = pow(li[i],2) - pow(D,2) + pow(Rm,2);
      
      if( i%2 == 0)
      {
        Ci[i]=-Ci[i]; 
      }
   }
}
void calculateLegLength()
{ 
  double yi[3][6];
  for(int i = 0; i<6; i++)
  {
    yi[0][i] = rotationMatrix[0][0]*pi_original[0][i] + rotationMatrix[0][1]*pi_original[1][i] + rotationMatrix[0][2]*pi_original[2][i];
    yi[1][i] = rotationMatrix[1][0]*pi_original[0][i] + rotationMatrix[1][1]*pi_original[1][i] + rotationMatrix[1][2]*pi_original[2][i];
    yi[2][i] = rotationMatrix[2][0]*pi_original[0][i] + rotationMatrix[2][1]*pi_original[1][i] + rotationMatrix[2][2]*pi_original[2][i]; 
  }
  for(int i = 0; i<6; i++)
  {
    yi[0][i] = yi[0][i] + x;
    yi[1][i] = yi[1][i] + y;
    yi[2][i] = yi[2][i] + z;
  }
  for(int i = 0; i<6; i++)
  {
    yi[0][i] = yi[0][i]-bi[0][i]; 
    yi[1][i] = yi[1][i]-bi[1][i]; 
    yi[2][i] = yi[2][i]-bi[2][i];
  } 
  for(int i = 0; i<6; i++)
  {
    li[i] = sqrt(pow(yi[0][i],2) + pow(yi[1][i],2) + pow(yi[2][i],2));
  }

}
void adjustPiWithTranslation()
{
  for(int i = 0; i<6; i++)
  {
    pi[0][i] = pi_original[0][i] + x;
    pi[1][i] = pi_original[1][i] + y;
    pi[2][i] = pi_original[2][i] + z;
  }
}
void calculateRotationMatrix()
{
  rotationMatrix[0][0] =  cos(yaw) * cos(pitch);
  rotationMatrix[0][1] = -sin(yaw) * cos(roll) + cos(yaw) * sin(pitch) * sin(roll);
  rotationMatrix[0][2] =  sin(yaw) * sin(roll) + cos(yaw) * sin(pitch) * cos(roll);

  rotationMatrix[1][0] =  sin(yaw) * cos(pitch);
  rotationMatrix[1][1] =  cos(yaw) * cos(roll) + sin(yaw) * sin(pitch) * sin(roll);
  rotationMatrix[1][2] = -cos(yaw) * sin(roll) + sin(yaw) * sin(pitch) * cos(roll);

  rotationMatrix[2][0] = -sin(pitch);
  rotationMatrix[2][1] =  cos(pitch)*sin(roll);
  rotationMatrix[2][2] =  cos(pitch)*cos(roll);
}
