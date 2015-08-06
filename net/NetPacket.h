#pragma once


#define NET_PACKET_HEADER_SIZE 8
#define NET_PACKET_DATA_SIZE (512-NET_PACKET_HEADER_SIZE)


struct NetPacketData
{
	union
	{
		struct
		{
			uint32_t serial;
			// Packet serial number

			uint32_t flags;
			// Packet flags
			// Lowest 8 bits: Number of packets forming a message
			//                (same value for all packets of the message)

			uint8_t data[NET_PACKET_DATA_SIZE];
			// Actual data
		};

		uint8_t raw[512];
		// Raw data pointer
	};
};


class NetPacket
{
public:

	NetPacketData _data;
	// Packet data

	int _size;
	// Raw packet size

	int64_t _ts;
	// Timestamp

	NetPacket *_prev, *_next;
	// Used for inserting the packing in doubly linked lists

};

