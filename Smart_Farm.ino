//Inputs
int Moisture_1 = A0;
int Moisture_2 = A1;
int Temperature = A2;
int RainFall = A3;
int IR=7;
int IR2=8;

//Input Variables
int Mois1;
int Mois2;
int Temp;
int RnFl;
int Moisture;
int Temp_C;
int Time;

//Outputs
int WaterPump=3;
int PesticidePump=4;
int WindingMotorF = 5;
int WindingMotorR = 6;

//Functions
float Celsius();
int Pump();
float CheckForRain();

void setup() {
Serial.begin(9600);
pinMode(WaterPump, OUTPUT);
pinMode(PesticidePump, OUTPUT);
pinMode(WindingMotorF,OUTPUT);
pinMode(WindingMotorR,OUTPUT);
pinMode(IR,INPUT);

}

void loop() {
const unsigned long int OneMin=1;
const unsigned long long TwoMin=2;
static unsigned long long lastSampleTime=0;

unsigned long now =millis();
Serial.print("Time is :");
Serial.println(now/60000);
now/=60000;
if(now-lastSampleTime >= OneMin)
{
  Serial.println("Pesticide Pump is On");
  digitalWrite(PesticidePump,HIGH);
}
if(now-lastSampleTime >= TwoMin)
{
  Serial.println("Pesticide Pump is OFF");
  digitalWrite(PesticidePump,LOW);
  lastSampleTime=now;
}



Mois1 = analogRead(Moisture_1);
Mois2 = analogRead(Moisture_2);
Temp = analogRead(Temperature);
RnFl = analogRead(RainFall);
Mois1 =map(Mois1,1050,200,0,100);
Mois2 =map(Mois2,1050,200,0,100);
Serial.print("Moist1: ");
Serial.println(Mois1);
Serial.print("Moist2 :");
Serial.println(Mois2);


Temp_C=Celsius(Temp);
RnFl = map(RnFl,0,1024,0,5);

if( Temp_C <40 && (Mois1 >=50 || Mois2 >=50))
{
  Pump(0);
}
else if(Mois1 <50 || Mois2 <50 || Temp_C >40)
{
  Pump(1);
}

delay(2000);
//CheckForRain(RnFl);

}


//Function to convert temperature equalent analog input to Temperature in celsius
float Celsius(int T)
{
  float MlVolts = (T/1024.0)*5000;
  float Temp_c=MlVolts/10;
  Serial.println("Temperature is");Serial.println(Temp_c);
  return Temp_c;
  
}

//Function to Control Water PUMP
int Pump(int M)
{
  if(M)
  {
    digitalWrite(WaterPump,HIGH);
    Serial.println("Soil Moisture is Low or the Temperature is HIGH Hence the Pump is turned ON");
  }
  else
  {
    digitalWrite(WaterPump,LOW);
    Serial.println("Motor Turned OFF");
  }
  return 0;
}


//Function to Check for Rain fall
float CheckForRain(float rain)
{
  int stat=digitalRead(IR);
  int stat1=digitalRead(IR2);
  if (rain>=1 && (stat == 0 || stat1==0))
  {
    Serial.println("Raining");
    
    digitalWrite(WindingMotorF,HIGH);
    digitalWrite(WaterPump,LOW);
    while(stat1==0)
    {
      delay(1000);
    }
    digitalWrite(WindingMotorF,LOW);
  }
  else if(rain<1 && (stat == 1 || stat1 ==1))
  {
    digitalWrite(WindingMotorR,HIGH);
  }
  while(stat==1)
    {
      delay(1000);
    }
    digitalWrite(WindingMotorF,LOW);
  return 0;
}
