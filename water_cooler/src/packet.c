/*
 * packet.c
 *
 *  Created on: 2016/06/28
 *      Author: b4122024
 */
#include "mynet.h"
#include "water_cooler.h"

#define MSG_BUFSIZE 488
#define R_BUFSIZE 512

#define HELLO 1
#define HERE 2
#define JOIN 3
#define POST 4
#define MESSAGE 5
#define QUIT 6

packet r_packet;
static char Buffer[MSG_BUFSIZE];

void create_packet(u_int32_t type, char *message)
{
	switch(type)
	{
	case HELLO:
		snprintf(message, MSG_BUFSIZE, "HELO");
		break;
	case HERE:
		snprintf(Buffer, MSG_BUFSIZE, "HERE");
		break;
	case JOIN:
		snprintf(Buffer, MSG_BUFSIZE, "JOIN %s", message);
		break;
	case POST:
		snprintf(Buffer, MSG_BUFSIZE, "POST %s", message);
		break;
	case MESSAGE:
		snprintf(Buffer, MSG_BUFSIZE, "MESG %s", message);
		break;
	case QUIT:
		snprintf(Buffer, MSG_BUFSIZE, "QUIT");
		break;
	default:
		/* Undefined packet type */
		break;
	}
}

u_int32_t analize_header( char *header )
{
	if(strncmp(header, "HELO", 4) == 0)
	{
		return(HELLO);
	}
	if(strncmp(header, "HERE", 4) == 0)
	{
		return(HERE);
	}
	if(strncmp(header, "JOIN", 4) == 0)
	{
		return(JOIN);
	}
	if(strncmp(header, "POST", 4) == 0)
	{
		return(POST);
	}
	if(strncmp(header, "MESG", 4) == 0)
	{
		return(MESSAGE);
	}
	if(strncmp(header, "QUIT", 4) == 0)
	{
		return(QUIT);
	}

	return 0;
}


// 受信バッファを構造体に格納しなおす関数
packet re_packet(char *r_buf)
{
	char *p;
	packet r_packet;

	snprintf(r_packet.header, 4, "%s", r_buf);

	if((p = strchr(r_buf+4, (int)' ')) != NULL || (p = strchr(r_buf+4, (int)'0')) != NULL)
	{
		if(p == r_buf+5)
		{
			r_packet.sep = *p;
		}
	}

	strcpy(r_packet.data, r_buf+4);
	return r_packet;
}
