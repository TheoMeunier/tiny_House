/************************************************
 *WITTIG Théo le 10/02/2021 Projet Tiny House
 ************************************************/
//on ping les brahce des capteur
const int potar=0;
const int lumi=1;
const int led=8;

//on défini nos variable
bool etat_chauffage;
bool Jour;
int valeur_potar;
int valeur_lumi;
int temp;
int r,g,b;

//on import nos librairie
#include "DHT.h"
#include <Wire.h>
#include "DFRobot_RGBLCD.h"

//on définie les differents capteur
#define DHTPIN A2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
DFRobot_RGBLCD lcd(16,2);


void setup() {
    // on initialise le moniteur et nos proche
    Serial.begin(9600);
    pinMode(potar,INPUT);
    pinMode(lumi,INPUT);
    pinMode(led,OUTPUT);
    lcd.init();
    dht.begin();

    //on défini nos canal pour la couleur de l'ecran et on défini la couleur
    r=55;
    g=50;
    b=151;
    lcd.setRGB(r, g, b);
}

void loop() {
    valeur_lumi=analogRead(lumi); // Récupération de la luminosité
    temperature(); // Exécution de la fonction
    consigne(); // Exécution de la fonction
    ecran(); // Exécution de la fonction
    chauffage(); // Exécution de la fonction
}

//on s'occupe du chauffage
void chauffage() {
    //on regarde si il fait jour
    if (Jour==1) {
        //on regarde si la température et supérieur a la consigne
        if (temp>valeur_potar) {
            //on défini la variable etat_chauffage  à 0
            etat_chauffage=0;
            // on eteint la led
            digitalWrite(led, LOW);
            //on lance la fonctione ecrant
            ecran();
        }
            // on regarde si la température est inférieur à la consigne
        else if (temp<valeur_potar) {
            //on met la varaible a 1
            etat_chauffage=1;
            //on met la led sur ONE
            digitalWrite(led, HIGH);
            // on appel la fonction
            ecran();
            //on met un delait
            delay(3000);
        }
    }
        // on regarde si il fait nuit
    else if (Jour==0) {
        //on regarde si la températue est inférieur a 17
        if (temp<17) {
            //on met la variable sur 1
            etat_chauffage=1;
            //on allume la led
            digitalWrite(led, HIGH);
            //on appel la fonction ecrant
            ecran();
            //on met un delai
            delay(3000);
        }
            //on regarde si la température est supérieur a 17
        else if (temp>17) {
            etat_chauffage=0;
            // on eteint la led
            digitalWrite(led, LOW);
            ecran();
        }
    }
}


// on s'occupe de la consigne
void consigne() {
    if (Jour == 1) {
        // on recupère la valeyr du potentiomèrre en de 15, 25
        valeur_potar=map(analogRead(potar), 0, 1023, 15, 25);
        //si la veur est inféireur a 10 alors on affiche la veur
        if (valeur_potar<10) {
            lcd.setCursor(10,1);
            lcd.print(" ");
            lcd.setCursor(11,1);
        }
            //Sinon afficher la valeur avec les untiés et dizaines
        else {
            lcd.setCursor(10,1);
        }
        // on Afficge la valeur de valeur_polar
        lcd.print(valeur_potar);
        lcd.setCursor(12,1);
        lcd.print("C");
        //on met un delay
        delay(500);
    }else {//sinon on affiche 17C si il fait nuit
        lcd.setCursor(10,1);
        lcd.print("17C");
    }

}

//on s'occupe de la température
void temperature() {
    //on récupere la température extèrieur et on l'affiche
    temp= dht.readTemperature();
    lcd.setCursor(0, 0);
    lcd.print("TEMP:");
    lcd.setCursor(10, 0);
    lcd.print(temp);
    lcd.setCursor(12, 0);
    lcd.print("C");
}

//on s'occupe de l'ecran
void ecran() {
    //o,n affiche et on indique l'emplacement de ce qu'on veux afficher
    lcd.setCursor(13, 0);
    lcd.print("|");
    lcd.setCursor(13, 1);
    lcd.print("|");
    lcd.setCursor(0, 1);
    lcd.print("CONSIGNE:");
    lcd.setCursor(15, 1);

    // on regarde si la luminosité est supérieur a 400lux alors on affiche J
    if (valeur_lumi>400) {
        lcd.print("J");
        Jour=1;
    }
        // sinon on indique qu"il fait nuit
    else {
        lcd.print("N");
        Jour=0;
    }
    lcd.setCursor(15, 0);
    //on regarde si le chauffage fonctionne et on affiche le chauffage
    if (etat_chauffage==1) {
        //on affiche c
        lcd.print("C");
    }
        //sinon on affiche rien
    else {
        lcd.print(" ");
    }
}