#include "gd32f30x.h"
#include "GPS.h"

void tcp_init(void);
void close(void);
void tcp_login(gps_t *gh);
void tcp_assemble(gps_t *gh, char *rfid);
void removeAsterisks(char **str);
