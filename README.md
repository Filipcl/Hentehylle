# Hentehylle
Dette er et prosjekt i emnet IN1060 Bruksorientert design ved Universitetet i Oslo. 
Vår oppgave var å planlegge og gjennomføre et prosjekt hvor vi designet og bygge en digital prototype for og med en valgt brukergruppe. 
Vi valgte folkebibliotek som undersøkelsesdomene, og utviklet en digital prototype for folkebibliotekenes eksisterende hentehylleløsning. 

Prototypens utforming er en standar hylle utstyrt med to stk RFID-lesere, den ene leseren er integrert i bokhyllens skillevegg og leser bøkenes ID. Den andre RFID leseren er en kortleser som leser inn RFID kortenes ID, kortleseren er utstyrt med en NeoPixel Stick 8 som blinker grønt hvis boken din er i hyllen og rødt hvis den ikke er der. Hvis boken er i hyllen og det riktige lånekortet registreres, lyser en Luxorparts Adresserbar RGB LED-list opp hvilke hylleseksjon boken befinner seg. 

Min arbeidsoppgave var teknisk ansavrlig på dette prosjektet, som innebar å skrive koden og koble kretsen.     

Hovedkomponenter: 
- 3 stk Arduino Uno. 
- 2 stk MFRC522 RFID-lesere. 
- 2 stk RFID-kort. 
- 2 stk powerbank. 
- Luxorparts Adresserbar RGB LED-list 5 m.
- NeoPixel Stick 8 - 8X 5050 RGB LED  adresserbar-rgb-led-stav
- 2 stk Electrolytic Capacitor.
- 2 stk 220Ω resistor til RGB LED-stripe.

Koden er skrevet i Arduino språket

