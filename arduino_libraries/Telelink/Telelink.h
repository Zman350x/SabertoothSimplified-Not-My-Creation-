#include <inttypes.h>
#include <FastSerial.h>

#define LINK_MAX_PAYLOAD_LEN 255 ///< Maximum payload length
#define LINK_NUM_CHECKSUM_BYTES 2
#define LINK_PACKET_STX 254
#define LINK_CORE_HEADER_LEN 5 ///< Length of core header (of the comm. layer): message length (1 byte) + message sequence (1 byte) + message system id (1 byte) + message component id (1 byte) + message type id (1 byte)
#define LINK_NUM_HEADER_BYTES (LINK_CORE_HEADER_LEN + 1) ///< Length of all header bytes, including core and checksum
#define LINK_NUM_NON_PAYLOAD_BYTES (LINK_NUM_HEADER_BYTES + LINK_NUM_CHECKSUM_BYTES)

#define _MAV_PAYLOAD(msg) ((const char *)(&((msg)->payload64[0])))
#define _MAV_PAYLOAD_NON_CONST(msg) ((char *)(&((msg)->payload64[0])))

// checksum is immediately after the payload bytes
#define mavlink_ck_a(msg) *((msg)->len + (uint8_t *)_MAV_PAYLOAD_NON_CONST(msg))
#define mavlink_ck_b(msg) *(((msg)->len+(uint16_t)1) + (uint8_t *)_MAV_PAYLOAD_NON_CONST(msg))


#include "checksum.h"

#define LINK_HELPER static inline
#include "link_helpers.h"

#include "msgs/common.h"
#include "msgs/msg_01_test.h"

