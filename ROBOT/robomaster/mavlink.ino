
void mavlink_run() {
        /* The default UART header for your MCU */ 
 int sysid = 20;                   ///< ID 20 for this airplane
    int compid = MAV_COMP_ID_IMU;     ///< The component sending the message is the IMU, it could be also a Linux process
    int type = MAV_TYPE_GROUND_ROVER;   ///< This system is an airplane / fixed wing
 
// Define the system type, in this case an airplane
    uint8_t system_type = MAV_TYPE_GROUND_ROVER;
    uint8_t autopilot_type = MAV_AUTOPILOT_GENERIC;
 
    uint8_t system_mode = MAV_MODE_PREFLIGHT; ///< Booting up
    uint32_t custom_mode = 0;                 ///< Custom mode, can be defined by user/adopter
    uint8_t system_state = MAV_STATE_STANDBY; ///< System ready for flight
  
  // Initialize the required buffers
  mavlink_message_t msg;
  
  // Pack the message
  mavlink_msg_heartbeat_pack(sysid,compid, &msg, type, autopilot_type, system_mode, custom_mode, system_state);

  send_mav_message(&msg);

}


void mavlink_receive() {
  mavlink_message_t msg;
  mavlink_status_t status;
 
  // COMMUNICATION THROUGH EXTERNAL UART PORT (XBee serial)
  while(Serial3.available() > 0 ) {
    uint8_t c = Serial3.read();
    // Try to get a new message
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
      // Handle message
      Serial.println(msg.msgid);
      switch(msg.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT: {
          // E.g. read GCS heartbeat and go into
          // comm lost mode if timer times out
        }
        break;
        case MAVLINK_MSG_ID_COMMAND_LONG:
          // EXECUTE ACTION
        break;
        default:
          //Do nothing
        break;
      }
    }
  }
}
 
void send_mav_message(mavlink_message_t* msg) {
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  uint16_t len = mavlink_msg_to_send_buffer(buf, msg);
  Serial3.write(buf, len);
}
