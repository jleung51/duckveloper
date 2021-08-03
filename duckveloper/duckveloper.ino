/**
 * This Arduino sketch lights up an LED and plays a random MP3 track from an
 * SD card when a button is pressed.
 *
 * The SD card must be loaded onto the Arduino using a shield or other
 * component.
 * Each MP3 track must be named sequentially in the format track001.mp3,
 * track002.mp3, etc.
 * 
 */

#include <SPI.h>
#include <FreeStack.h>
#include <SdFat.h>
#include <vs1053_SdFat.h>


// Below is not needed if interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
  #include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
  #include <SimpleTimer.h>
#endif



// ------------------------------------------
// HARDWARE SETTINGS
// ------------------------------------------

// Schematic:
// 5v -> Button -> D5
//              -> 10k Ohm resistor - > GND
// D6 -> LED  -> 330k Ohm resistor -> GND

// Pin connections

const uint8_t PIN_LED = 6;
const uint8_t PIN_BTN = 5;

// Used for generating a random seed for the RNG
// Choose any unconnected analog pin
const uint8_t PIN_ANALOG_RNG = A0;

// ------------------------------------------
// GLOBAL VARIABLES
// ------------------------------------------

// Total number of MP3 tracks on the SD card.
const uint8_t NUMBER_OF_TRACKS = 3;
uint8_t last_played = 9999;

// Instances of the libraries
SdFat sd;
vs1053 MP3player;



// ------------------------------------------
// ARDUINO FUNCTIONS
// ------------------------------------------

// Returned error codes are typically passed up from MP3player,
// which in turn creates and initializes the SdCard objects
void setup() {

  Serial.begin(115200);

  Serial.println(F("To look up error codes, navigate to: https://mpflaga.github.io/Arduino_Library-vs1053_for_SdFat/index.html#Error_Codes"));

  Serial.print(F("F_CPU = "));
  Serial.println(F_CPU);
  Serial.print(F("Free RAM = ")); // Available in Version 1.0 F() bases the string to into Flash, to use less SRAM.
  Serial.print(FreeStack(), DEC);  // Provided by SdFat
  Serial.println(F(" Should be a base line of 1028, on ATmega328 when using INTx"));


  // Initialize Serial port and MP3player objects with sd.begin()

  // Initialize the SdCard
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // Dsepending upon your SdCard environment, SPI_HAVE_SPEED may work better
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  // Initialize the MP3 Player Shield
  uint8_t result = MP3player.begin();
  if(result != 0) {
    Serial.print(F("Error code: "));
    Serial.print(result);
    Serial.println(F(" when trying to start MP3 player"));
  }

#if defined(__BIOFEEDBACK_MEGA__) // or other reasons, of your choosing.
  // Typically not used by most shields, hence commented out.
  Serial.println(F("Applying ADMixer patch."));
  if(MP3player.ADMixerLoad("admxster.053") == 0) {
    Serial.println(F("Setting ADMixer Volume."));
    MP3player.ADMixerVol(-3);
  }
#endif

  seed_rng();

  // LED and button setup  
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_BTN, INPUT);

}  // End of setup()

// Main loop of the program
void loop() {

  // Upon button press:
  uint8_t buttonState = digitalRead(PIN_BTN);
  if (buttonState == HIGH) {

    Serial.println(F("Button status: Pressed"));

    // Turn on light
    digitalWrite(PIN_LED, HIGH);

    // Select and play a random track (different from the previous)
    uint8_t track_num;
    do {
      track_num = random(1, NUMBER_OF_TRACKS+1);
    } while (track_num == last_played);
    last_played = track_num;

    play_track(track_num);

    // Wait until track ends
    do {
      delay(100);
    } while (MP3player.getState() == playback);
    Serial.println(F("Track finished"));

    // When done, turn off light
    digitalWrite(PIN_LED, LOW);

  } else {
    Serial.println(F("Button status: Not pressed"));
  }

  delay(100);
}



// ------------------------------------------
// CUSTOM FUNCTIONS
// ------------------------------------------

// This function seeds the random number generator with a partially random value.
void seed_rng() {
  randomSeed(analogRead(PIN_ANALOG_RNG));
}

// This function plays a track numbered as "track00[track_num].mp3".
//
// Throws an error if the track number is invalid (based on the value)
// from NUMBER_OF_TRACKS).
void play_track(uint8_t track_num) {

#if USE_MULTIPLE_CARDS
  sd.chvol();  // Assign desired sdcard's volume
#endif

  // Validate that the track number is valid
  if (track_num > NUMBER_OF_TRACKS) {
    Serial.print(F("Error: Track "));
    Serial.print(track_num);
    Serial.print(" is invalid; there are a maximum of ");
    Serial.print(NUMBER_OF_TRACKS);
    Serial.println(" tracks.");
    return;
  }

  Serial.print(F("Playing track \"track00"));
  Serial.print(track_num);
  Serial.println(F(".mp3\""));
  uint8_t result = MP3player.playTrack(track_num);

  // Error codes reference:
  // https://mpflaga.github.io/Arduino_Library-vs1053_for_SdFat/index.html#Error_Codes
  if(result != 0) {
    Serial.print(F("VS1053 Error Code "));
    Serial.print(result);
    Serial.print(F(" when trying to play track "));
    Serial.println(track_num);
    return;
  }
  
  // display_track_info();
}


// This function displays the title, artist, and album of the current track.
void display_track_info() {

  if (MP3player.getState() != playback) {
    Serial.println(F("No track currently playing."));
  }

  char title[30];
  MP3player.trackTitle((char*)&title);

  char artist[30];
  MP3player.trackArtist((char*)&artist);

  char album[30];
  MP3player.trackAlbum((char*)&album);

  Serial.print(F("Playing "));
  Serial.write((byte*)&title, 30);
  Serial.print(F(" by "));
  Serial.write((byte*)&artist, 30);
  Serial.print(F(" from album "));
  Serial.write((byte*)&album, 30);

  Serial.println();
}
