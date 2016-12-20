#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "canmgr_proto.h"
#include "canmgr_dump.h"
#include "lxcan.h"

static const uint8_t myaddr = 0; // lie

int main (int argc, char *argv[])
{
    struct canmgr_frame in, out;
    int m, n;
    int s;
    char dump[80]; 

    if (argc != 3) {
        fprintf (stderr, "Usage: canpower m,n 0|1\n");
        exit (1);
    }
    if (sscanf (argv[1], "%d,%d", &m, &n) != 2
            ||  m < 0 || m >= 0x10 || n < 0 || n >= 0x10) {
        fprintf (stderr, "improperly specified target\n");
        exit (1);
    }
    /* construct request
     * for now, no routing - cluster and module are ignored
     */
    in.id.pri = 1;
    in.id.dst = n | 0x10;
    in.id.src = myaddr;

    in.hdr .pri = 1;
    in.hdr.type = CANMGR_TYPE_WO;
    in.hdr.node = in.id.dst;
    in.hdr.module = m;
    in.hdr.object = CANOBJ_TARGET_POWER;
    in.data[0] = strtoul (argv[2], NULL, 10); /* 0=off, 1=on */
    in.dlen = 1;

    /* send frame on can0
     * wait for ack/nak response
     */
    if ((s = lxcan_open ("can0")) < 0) {
        fprintf (stderr, "lxcan_open: %m\n");
        exit (1);
    }
    canmgr_dump (&in, dump, sizeof (dump));
    printf ("%s\n", dump);
    if (lxcan_send (s, &in) < 0) {
        fprintf (stderr, "lxcan_send: %m\n");
        exit (1);
    }
    // TODO timeout
    for (;;) {
        if (lxcan_recv (s, &out) < 0) {
            fprintf (stderr, "lxcan_recv: %m\n");
            exit (1);
        }
        canmgr_dump (&out, dump, sizeof (dump));
        printf ("%s\n", dump);
        if (out.id.src != in.id.dst || out.id.dst != in.id.src)
            continue;
        if (out.hdr.object != CANOBJ_TARGET_POWER)
            continue;
        if (out.hdr.module != in.hdr.module || out.hdr.node != in.hdr.node)
            continue;
        if (out.hdr.type == CANMGR_TYPE_ACK) {
            fprintf (stderr, "OK\n");
            exit (0);
        }
        if (out.hdr.type == CANMGR_TYPE_NAK) {
            fprintf (stderr, "Received NAK response\n");
            exit (1);
        }
    }
    can_close (s);

    exit (0);
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
