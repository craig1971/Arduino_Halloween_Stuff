
void MonsterSound::initialize()
{
  Serial.println("Initializing Sounds...");
  if(!card.init())        Serial.println(F("Card init. failed!"));
  if(!vol.init(card))     Serial.println(F("No partition!"));
  if(!root.openRoot(vol)) Serial.println(F("Couldn't open dir"));
  Serial.println(F("Files found:"));
  root.ls();
  Serial.println("Sound Init Complete.");
}


void  MonsterSound::playRoar()
{
  this->playfile("ROAR0001.WAV");
}

void  MonsterSound::playSnore()
{
  char filename[] = "SNORE001.WAV";
  this->playfile(filename);
}

void MonsterSound::stopAll()
{
  wave.stop(); // Stop any currently-playing WAV
}

// -------------------------------------------------------------------
// playfile()    
// Open and start playing a WAV file
// -------------------------------------------------------------------
void MonsterSound::playfile(char *name) 
{    
  PgmPrint("Playing '");
  Serial.print(name); 
  PgmPrintln("'");
  
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }

  if (!file.open(root, name)) {
    PgmPrint("Couldn't open file ");
    Serial.println(name); 
    
    if(!root.openRoot(vol)) Serial.println(F("Couldn't open dir"));
    Serial.println(F("Files found:"));
    root.ls();
    
    return;
  }

  if (!wave.create(file)) {
    PgmPrintln("Not a valid WAV");
    return;
  }
  // ok time to play!
  wave.play();
}

