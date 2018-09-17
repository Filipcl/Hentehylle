
// Del 1 : Deklarering av globale variable og import av biblioteker

/*
  MFRC522.h: Dette biblioteket tillater Arduino aa
  kommunisere med et Serial PeripheralInterface (SPI), RC522.
  SPI.h lar deg lese av og skrive til ulike typer
  Radio-Frequency IDentification (RFID) kort
  koblet til Arduino med en RC522-baser kortleser koblet til SPI.
  Adafruit NeoPixel er et bibliotek som gir deg kontroll over til
  deres NeoPixel LED serie.
  NB: Serial.print() brukes kun til debugging,
  og har ingen paavirknig paa systemet ellers.

*/
#include <MFRC522.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

/*
  Arduino Uno inn-og utganger defineres her
*/

#define PIN 4     // Adafruit NeoPixel Stick 8 (paa kortleser)
#define KORTLESER_LED 8 // Antall ledlys til kortleseren
#define PIN 7           // Digitalporten til hylle led
#define HYLLE_LED 97  // antall ledlys til hyllen tot: 97 leds. 
            // Hylleseksjon 1: 0-49  Hylleseksjon 2: 50-97

constexpr uint8_t RST_PIN = 10;     // Reset
constexpr uint8_t SS_1_PIN = 9;     // SDA pin til Bokleseren.
constexpr uint8_t SS_2_PIN = 8;     // SDA pin til Kortleseren.
constexpr uint8_t NR_OF_READERS = 2;// Setter antall lesere til maks 2.


/*
   Instanser vi trenger opprettes her.
   En hyllelys-peker av Adafruit_NeoPixel. (hyllelys)
   En kortleserlys-peker av Adafruit_NeoPixel (kortleserlys)
   Et MFRC522-peker for RFID-leserne.
*/

byte ssPins[] = {SS_1_PIN, SS_2_PIN};     
MFRC522 mfrc522[NR_OF_READERS];          

Adafruit_NeoPixel hyllelys = Adafruit_NeoPixel(HYLLE_LED, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel kortleserlys = Adafruit_NeoPixel(KORTLESER_LED, PIN, NEO_GRB + NEO_KHZ800);

/*
 Setter globale variable til bruk i loopen.
 For aa ta vare paa tilstandene til henholdsvis
 bok og kort bruker vi boolean.
*/

boolean bok = false;          // Om boken er lest
boolean riktigkort = false;   // Om kortet finnes i systemet
boolean match = false;        // Om det finnes en bok tilhoerende kortet
boolean feilkort = false;     // Om kortet ikke har tilhoerende boeker i hyllen.
boolean skannetKort = false;  // Om et kort er avlest
boolean ikkeTattUt = true;    // Om boken fremdeles er i hyllen.





void setup() {
  hyllelys.begin();      // Klargjoer digitalpin'en for output.
 hyllelys.show();        // Initialiserer alle pikslene i hyllen
  kortleserlys.begin();     //gjoer det samme for kortleserlysene
 kortleserlys.show();
  Serial.begin(9600);      //  Starter Serial
 while (!Serial);        //  For debugging. Gjoer ingenting til serial er kalt paa.
 SPI.begin();             // Starter seriell-kommunikasjon.
 
 // Oppretter og leser ut versjonnummer og leser til serial.
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
   mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); 
   Serial.print(F("Reader"));
   Serial.print(reader);
   Serial.print(F(":"));
   mfrc522[reader].PCD_DumpVersionToSerial(); // *** Til debugging skriver  ut versjonen og nummeret til leseren i serial.
 }
 }







void loop() {
/*

  Loekken aktiverer hver RFID-leser, og starter soek
  etter kort.
  Innleste korts unike permanente id (UID)
  konverteres fra hexadesimaler til en mer
  haandterlig type, String. 
  Boken og kortets UID legges i egne string variable.
  Strengene settes ogsaa inn i det tilhoerende arrayet.

  NB: Serial.print() brukes kun til debugging.
*/
  int reader = 0;
 String innhold = "";
 String skannerArray[2];
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
   if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
    
     Serial.print(F("Leser"));
     Serial.print(reader); 
     Serial.print(F(":KortUID:"));
     Serial.println();
     Serial.print(F("PICCtype:"));
    
     MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
     Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));
    
     for (byte i = 0; i < mfrc522[reader].uid.size; i++){
       Serial.print(mfrc522[reader].uid.uidByte[i] < 0x10 ? "0" : "");
       Serial.print(mfrc522[reader].uid.uidByte[i], HEX);
       innhold.concat(String(mfrc522[reader].uid.uidByte[i] < 0x10 ? "0" : ""));
       innhold.concat(String(mfrc522[reader].uid.uidByte[i], HEX)); 2;
     }
     innhold.toUpperCase();
     skannerArray[reader] = innhold;  }
   }
   String boken = skannerArray[1].substring(1);
   String kortet = skannerArray[0].substring(1);
  
  
/*
     Siden vi allerede vet at boken er der naar vi
    evaluerer med
     brukeren dobbeltsjekker vi bare dette foerst.
     Vi kan ogsaa forutsi at det neste steget
     er at brukeren vil forsoeke
     aa lese av kortet sitt. betingelsen for
     aa kunne skru paa lyset er at baade kort og bok er der.
     Brukeren faar  beskjed om at innlest kort
     samsvarer med boken i hyllen og faar feedback
     i form av lys i kortleseren.
     Vi bruker variabelen ikkeTattUt som indikerer
     at bokleseren slutter aa lese. Hvis den blir
     satt til false, skrus lyset av og systemet
     gaar tilbake til aa lytte.
     Hvis boken blir tatt ut slutter det aa lyse
     i hyllenseksjonen.
     Saa lenge ingen av disse tilfellene inntreffer
     skal lyset i hyllen vaere av og
     kortleserlyset skal vaere satt til gult.
   */   
  
   if(boken == "64 34 32 F9" && ikkeTattUt){
     bok = true;                             
     ikkeTattUt = true;
     Serial.println("boken er her");          
   }
  
   if(kortet == "A0 BF AC 56"){
     riktigkort = true;                         
   }

   // 
   if(bok && riktigkort && !skannetKort){ 
     colorWipeKortleser(kortleserlys.Color(0,127,0),0);
     delay(800);                           
     colorWipeKortleser(kortleserlys.Color(0,0,0),0);  
     Serial.println("MATCH");             
     match = true;                         
     skannetKort = true;                   
   }
   else if(bok && riktigkort && skannetKort && ikkeTattUt){
     match = true;
   }
   else if(bok && riktigkort && skannetKort && !ikkeTattUt){
     match = false;
     riktigkort = false;
   }
   else{   
     match = false;
   }
  
   if(riktigkort && match && ikkeTattUt){
     colorWipe(hyllelys.Color(0, 127, 0), 0);
     hyllelys.show();
     delay(4000);
     match = false;                   
     Serial.println("lyset er paa");
    
   }
  
   if(!match){ 
     colorWipe(hyllelys.Color(0, 0, 0), 0);
     hyllelys.show();
     colorWipeKortleser(kortleserlys.Color(127, 127, 0), 0);
     Serial.println("lyset er av");
     bok = false;
    
   }
  
   mfrc522[reader].PICC_HaltA();       //Stopper innlesing paa sist leste kort.
   mfrc522[reader].PCD_StopCrypto1();  // Stop encryption on PCD
 }
 
 /*
 colorWipe setter RGB verdien til hver enkelt
 LED i ledstrip og stick 8.
 Verdien som settes blir foerst synlig lys naar man kaller .show()
*/
 void colorWipe(uint32_t c, uint8_t wait) {
   for(uint16_t i=50; i<hyllelys.numPixels(); i++) { 
     hyllelys.setPixelColor(i, c);
     delay(wait);
   }
 }
 void colorWipeKortleser(uint32_t c1, uint8_t wait) {
   for(uint16_t i=0; i<kortleserlys.numPixels(); i++) {
     kortleserlys.setPixelColor(i, c1);
     delay(wait);
   }
   kortleserlys.show();
 }

