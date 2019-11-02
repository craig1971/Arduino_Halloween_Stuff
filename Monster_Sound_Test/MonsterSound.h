#include <Arduino.h>
#include <Wire.h>
#include <WaveHC.h>
#include <WaveUtil.h>

class MonsterSound 
{
  private:  

    SdReader  card; // This object holds the information for the card
    FatVolume vol;  // This holds the information for the partition on the card
    FatReader root; // This holds the information for the volumes root directory
    FatReader file; // This object represent the WAV file for a phrase
    WaveHC    wave; // A single wave object -- only one sound is played at a time
  
    void playfile(char *name);

  public:
    void initialize();    
    void playRoar();    
    void playSnore();    
    void stopAll();
};
