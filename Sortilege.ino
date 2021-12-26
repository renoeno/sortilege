/*

  Sortilege

  * Reno Almeida
  * Do Park
   
  OCAD University

*/

//Including libriries of MP3 Shield, SD card and LCD Display.
#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>
#include <Bounce2.h>
#include <LiquidCrystal.h>
SdFat sd;
SFEMP3Shield MP3player;

//Assigning the display pins to Arduino pins. Since it's kind of buggy trying to connect them direct to digital pins of MP3 Shield, 
//we also used analog pins (14, 15, 16, 17 and 19 are A0, A1, A2, A3 and A5).
LiquidCrystal lcd(14, 15, 16, 17, 3, 19);//12, 11, 5, 4, 3, 2

//The vector of strings are loaded in the display accordingly to the track played.
String text[12] = {" She came along the alley, and up the backstairs the way she always used to... ", " Does it ever end? ", " But hey, what the fuck does it matter? ", " Civil rights violation... ", " You're doing good Doc.", " And now began to make sense... ", " I don't know... ", " What's on your mind, Doc? ", " And once again, you're the bait. ", " Then go get'em. ", " 'course it does... ", " psst... "};

int potReader = 0;

//Variables holding the current and the last value of the audio file played.
int station = 0;
int lastStation = 0;

void setup() {
  Serial.begin(115200);

  //Initializing the display.
  lcd.begin(16, 2);

  //Initializing the SD card.
  if (!sd.begin(9, SPI_HALF_SPEED)) sd.initErrorHalt();
  if (!sd.chdir("/")) sd.errorHalt("sd.chdir");

  //Initializing the MP3 Shield.
  MP3player.begin();

  //Setting volume so it is not too low.
  MP3player.setVolume(10, 10);

}

void loop() {

  //Analog port 4, in which the potentiometer is connected, is read.
  potReader = analogRead(4);

  //Variable station is simply a mapping of potReader to a value from 1 to 12 (number of audio files).
  station = map(potReader, 0, 1023, 1, 12);

  //Test if nothing is being played.
  if (!MP3player.isPlaying()) {

    //If the current station is not the last one, plays the new audio file.
    if (station != lastStation) {

      MP3player.playTrack(station);

      //Audio tracks goes from 1 to 12 and since every vector begins at index 0, we decrease one from station so we can use it
      //as index.
      lcd.print(text[station - 1]);


      //The following for loop scrolls the text to the left, so larger amounts of texts can be totally displayed.
      for (int positionCounter = 0; positionCounter < text[station - 1].length(); positionCounter++) {
        // scroll one position left:
        lcd.scrollDisplayLeft();
        // wait a bit:
        delay(150);
      }

      //Clear the display
      lcd.clear();

      //Update lastStation variable.
      lastStation = station;
    }

    delay(500);

  }

}

