#ifndef COMMON_H
#define COMMON_H

typedef struct __linkmessage_t {
	uint16_t checksum; /// sent at end of packet
	uint8_t magic;   ///< protocol magic marker
	uint8_t len;     ///< Length of payload
	uint8_t seq;     ///< Sequence of packet
	uint8_t sysid;   ///< ID of message sender system/aircraft
	uint8_t compid;  ///< ID of the message sender component
	uint8_t msgid;   ///< ID of message in payload
	uint64_t payload64[(LINK_MAX_PAYLOAD_LEN+LINK_NUM_CHECKSUM_BYTES+7)/8];
} linkmessage_t;


#endif // COMMON_H
