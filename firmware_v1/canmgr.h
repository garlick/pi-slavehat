// See Meiko CS/2 "Overview of the Control Area Network (CAN)"
// The LCAN/XCAN/GCAN and extended addressing/routing scheme described
// in that document was borrowed here.

// LCAN addressing within a module:
// compute board: 00 01 02 03 04 05 06 07 08 09 0a 0b
// compute mgr:   10 11 12 13 14 15 16 17 18 19 1a 1b
// module mgr:    1d

// CAN header (11 byte):
//   pri:1 dst:5 src:5  (priority: 0=high, 1=low)
struct canmgr_header {
    uint16_t pri:1;
    uint16_t dst:5;
    uint16_t src:5;
};
#define CANMGR_DST_MASK (0b01111100000)

// CAN payload (4 byte header + 0-4 byte data)
//   pri:1 type:3 cluster:6 module:6 node:6 object:10 data:32
struct canmgr_payload {
    uint32_t pri:1;
    uint32_t type:3;
    uint32_t cluster:6;         // gcan address
    uint32_t module:6;          // mcan address
    uint32_t node:6;            // lcan address
    uint32_t object:10;
    uint8_t data[4];
};

// special 6-bit canmgr addresses for cluster/module/node
enum {
    CANMGR_ADDR_NOROUTE = 0b111111,
    CANMGR_ADDR_BCAST   = 0b111110,
};

// Message types:
enum {
    CANMGR_TYPE_RO = 0,         // read object (master only)
    CANMGR_TYPE_WO = 1,         // write object (master only)
    CANMGR_TYPE_WNA = 2,        // write object without ack (master only)
    CANMGR_TYPE_DAT = 3,        // data
    CANMGR_TYPE_ACK = 4,        // write acknowledge
    CANMGR_TYPE_NAK = 6,        // negative write acknowledge
    CANMGR_TYPE_SIG = 7,        // send a signal (slaves), obj in data:10
};

// canmgr objects
enum {
    CANOBJ_LED_IDENTIFY = 0,    // 1 byte (0=LED Off, 1=LED blinking)
    CANOBJ_TARGET_POWER = 1,    // 1 byte (0=5V off, 1=5V on)
    CANOBJ_TARGET_SHUTDOWN = 2, // 1 byte (0=normal, 1=begin shutdown sequence)
    CANOBJ_TARGET_RESET = 3,    // 1 byte (0=run, 1=hold in reset, 2=toggle)

    // todo:

    // console connect/disconnect/force_disconnect
    // (unlike the Meiko, compute mgr handle serial console for compute boards)

    // heartbeat

    // jtag/openocd protocol for remote gdb, etc..
};


void canmgr_setup (uint32_t addr);
void canmgr_finalize (void);
void canmgr_update (void);


/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
