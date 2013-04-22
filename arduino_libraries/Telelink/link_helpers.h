#ifndef  _LINK_HELPERS_H_
#define  _LINK_HELPERS_H_
#include "string.h"
#include <FastSerial.h>

#ifndef LINK_HELPER
#define LINK_HELPER
#endif

#include <inttypes.h>

#include "checksum.h"
#include "msgs/common.h"

LINK_HELPER uint16_t _link_finalize_message(linkmessage_t* msg, uint8_t length) {
	// This code part is the same for all messages;
	uint16_t checksum;
	msg->magic = LINK_PACKET_STX;
	msg->len = length;
	msg->sysid = 1;
	msg->compid = 2;
	// One sequence number per component
	msg->seq = 1;

	checksum = crc_calculate((uint8_t*)&msg->len, length + LINK_CORE_HEADER_LEN);

	mavlink_ck_a(msg) = (uint8_t)(checksum & 0xFF);
	mavlink_ck_b(msg) = (uint8_t)(checksum >> 8);

	return length + LINK_NUM_NON_PAYLOAD_BYTES;
}

LINK_HELPER void _link_finalize_message_send(FastSerial port, uint8_t msgid, const char *packet, uint8_t length) {
	uint16_t checksum;
	uint8_t buf[LINK_NUM_HEADER_BYTES];
	uint8_t ck[2];

	buf[0] = LINK_PACKET_STX;
	buf[1] = length;
	buf[2] = 1;
	buf[3] = 2;
	buf[4] = 3;
	buf[5] = msgid;

	checksum = crc_calculate((uint8_t*)&buf[1], LINK_CORE_HEADER_LEN);
	crc_accumulate_buffer(&checksum, packet, length);

	ck[0] = (uint8_t)(checksum & 0xFF);
	ck[1] = (uint8_t)(checksum >> 8);
	
	
	port.write((const uint8_t *)buf, LINK_NUM_HEADER_BYTES);
	port.write((const uint8_t *)packet, length);
	port.write((const uint8_t *)ck, 2);
	/*
	_link_send_uart(port, (const char *)buf, LINK_NUM_HEADER_BYTES);
	_link_send_uart(port, packet, length);
	_link_send_uart(port, (const char *)ck, 2);
	*/

}

LINK_HELPER void _link_send_uart(FastSerial port, const char *buf, uint16_t len) {
	uint16_t i;
	for (i = 0; i < len; i++) {
		//Serial3.write((uint8_t)buf[i]);
	}

}
#endif /* _LINK_HELPERS_H_ */
