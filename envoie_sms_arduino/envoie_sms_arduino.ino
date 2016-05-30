#include <SoftwareSerial.h>

SoftwareSerial SoftSerial(2, 3);

int onModulePin=4;

int8_t answer; 
char aux_string[30];

//parametres de configuration
char phone_number[]="+221773675372"; // le numero vers lequel on envoie les coordonnees GPS *****

void setup() 
{ 
  
  // INITIALISATION DU MODULE GSM
    Serial.begin(9600);
    initGSM();

    //envoie de sms
    sendSMS("tutoriel du club robotech: envoie de sms avec arduino");
    wait(100);
    sendSMS("Maintenant tu sais envoyer des sms");
} 

void loop() { 
 
}


// initialiser le module GSM
void initGSM(){
    if (sendATcommand("AT", "OK", 100) == 0){
        while(sendATcommand("AT+CREG?", "+CREG: 0,1", 2000) == 0 );      
    }
}


/* Envoyer un sms */
void sendSMS(char* msg){
       
       if(sendATcommand("AT+CMGF=1", "OK", 1000)==1){
              sprintf(aux_string,"AT+CMGS=\"%s\"", phone_number);
              wait(50);
              sendATcommand(aux_string, ">", 2000);
              wait(50);
              Serial.print(msg);
              Serial.write(0x1A);
              sendATcommand("AT", "OK", 100);
       }
}

   
int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){
    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;
    
    // Initialisation de la chaine de caractère (string).
    memset(response, '\0', 100);
    
    wait(100);
    
    while( Serial.available() > 0)
      Serial.read();

    Serial.println(ATcommand);
    previous = millis();
    
    // Cette boucle attend la réponse du module GSM.
    do{  
          if(Serial.available() != 0){  
    
            response[x] = Serial.read();
            x++;
    
            // Comparaison des données
            if (strstr(response, expected_answer) != NULL)    
            {
                answer = 1;
            }
        }else{
          
          }
    
    // Attente d'une réponse.
    }while((answer == 0) && ((millis() - previous) < timeout)); 
       
    return answer;
}


static void wait(unsigned long ms){
  
  unsigned long start = millis();
  while (millis() - start < ms);
}


