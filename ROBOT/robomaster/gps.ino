
void readGPS() {
  if(readGPSData()) {
    Serial.println(gps.hdop());
  }
}

static bool readGPSData() {
  while (Serial1.available())
  {
    char in = Serial1.read();
    Serial.print(in);
    if (gps.encode(in))
      return true;
  }
  return false;
}

void send_mav_gps_raw_int() {
  mavlink_message_t msg;
  float lat, lon;
  unsigned long age;
  gps.f_get_position(&lat, &lon, &age);
  mavlink_msg_gps_raw_int_pack(20, 20, &msg, 
  0,
  3,
  lat*1e7,
  lon*1e7,
  gps.f_altitude()*1000,
  gps.hdop(),
  65535,
  65535,
  gps.f_course()*100,
  gps.satellites()
  );
  send_mav_message(&msg);
}
