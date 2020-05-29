#ifndef __SIM_H
#define __SIM_H

#include "stm32f10x_conf.h"

typedef void (*sim_recv_handler)(char *, int);

extern void sim_init(void);
extern void sim_call_phone(char *Tel);
extern void sim_answer_phone(void);
extern uint32_t sim_ready(void);
extern uint32_t sim_connect(char *ip, char *port);
extern uint32_t sim_disconnect(void);
extern uint32_t sim_send(char *msg);
extern void sim_handler(unsigned char c);
extern void set_sim_recv_handler(sim_recv_handler h);
uint32_t sim_shut(void);
uint32_t sim_judge_status(void);
void sim_reopen_network(void);
uint32_t sim_get(char *msg, char *len);
void sim_get_ip(void);
#endif






