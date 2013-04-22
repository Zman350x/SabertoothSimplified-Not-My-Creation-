#include "../link_helpers.h"


#define LINK_MSG_ID_TEST 1
typedef struct __link_msg_testmessage {
  int8_t  var1;
  int16_t var2;
} link_testmessage_t;

#define LINK_MSG_TESTMESSAGE_LEN 3
#define LINK_MSG_ID_01_LEN 3

static inline void link_msg_test_send(FastSerial port, int8_t var1, int16_t var2){

	link_testmessage_t packet;
	packet.var1 = var1;
	packet.var2 = var2;

	_link_finalize_message_send(port, LINK_MSG_ID_TEST, (const char *)&packet, LINK_MSG_TESTMESSAGE_LEN);
}

static inline uint16_t link_msg_test_pack(linkmessage_t* msg, int8_t var1, int16_t var2){
	link_testmessage_t packet;
	packet.var1 = var1;
	packet.var2 = var2;

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 3);

	msg->msgid = LINK_MSG_ID_TEST;
	return _link_finalize_message(msg, 3);
}