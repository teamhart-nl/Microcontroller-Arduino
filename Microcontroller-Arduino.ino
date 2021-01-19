String command;

void setup()
{
  Serial.begin(19200);
  Serial.flush();
}

void loop()
{
  if (Serial.available() > 0)
  {
    command = Serial.readString();
    Serial.println(command);

    int indPin = command.indexOf(':');
    int pin = command.substring(0,indPin).toInt();
    int speed = command.substring(indPin+1).toInt();
    Serial.println(pin);
    Serial.println(speed);
    analogWrite(pin, speed);
  }
}
