#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <inttypes.h>
#include <stdint.h>
#include <sprite.h>
#include <graphics.h>


/**
 * @brief subscribes to mouse interrupts
 * 
 * @param *bit_no -> activated bit in interrupt mask
 * 
 * @return 0 on success
 */ 
int mouse_subscribe_int(uint8_t *bit_no);


/**
 * @brief unsubscribes to mouse interrupts
 * 
 * @return 0 on success
 */ 
int mouse_unsubscribe_int();


/**
 * @brief writes a certain command to kbc using command WRITE_BYTE_TO_MOUSE
 * 
 * @param byte -> command to be written
 * 
 * @return 0 on success
 */
int write_mouse_command(uint8_t byte);



/**
 * @brief parses info of a mouse packet into a struct
 * 
 * @param *p -> info to be stored
 * @param *counter -> nº of the current packet of the mouse to be stored
 * @param *packet -> nº of sets of packets processed
 * 
 */
void parse_mouse_byte(struct packet *p, uint32_t *counter, uint32_t *packetCount);

void mouse_movement(struct packet* p, sprite* sp);

void (mouse_ih)(void);

#endif
