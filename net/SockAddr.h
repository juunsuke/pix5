#pragma once


class SockAddr
{
public:

	sockaddr_in sin;
	// The actual data


	SockAddr()
	{
		// Clear
		memset(&sin, 0, sizeof(sin));
	}

	SockAddr(const SockAddr& o)
	{
		// Copy constructor
		memcpy(&sin, &o.sin, sizeof(sin));
	}

	SockAddr(const sockaddr_in& sin)
	{
		// Copy a real sin
		memcpy(&this->sin, &sin, sizeof(sin));
	}

	SockAddr(sockaddr_in *sin)
	{
		// Copy a real sin
		memcpy(&this->sin, sin, sizeof(this->sin));
	}

	SockAddr(int fam, uint32_t addr, uint16_t port)
	{
		memset(&sin, 0, sizeof(sin));
		set(fam, addr, port);
	}

	SockAddr(uint32_t addr, uint16_t port)
	{
		memset(&sin, 0, sizeof(sin));
		set(addr, port);
	}

	SockAddr(uint32_t addr)
	{
		memset(&sin, 0, sizeof(sin));
		set(addr);
	}

	~SockAddr()
	{
	}


	SockAddr& operator=(const SockAddr& o)
	{
		// Copy
		memcpy(&sin, &o.sin, sizeof(sin));
		return *this;
	}

	SockAddr& operator=(const sockaddr_in& sin)
	{
		// Copy
		memcpy(&this->sin, &sin, sizeof(sin));
		return *this;
	}

	SockAddr& operator=(sockaddr_in *sin)
	{
		// Copy
		memcpy(&this->sin, sin, sizeof(this->sin));
		return *this;
	}

	SockAddr& operator=(uint32_t addr)
	{
		// Empty address with only the given address
		memset(&sin, 0, sizeof(sin));
		set(addr);
		return *this;
	}

	operator const sockaddr_in() const { return sin; }
	operator const sockaddr_in*() const { return &sin; }

	operator const sockaddr*() const { return (sockaddr*)&sin; }


	inline int family() const { return sin.sin_family; }
	inline uint16_t port() const { return ntohs(sin.sin_port); }
	inline uint32_t addr() const { return ntohl(sin.sin_addr.s_addr); }

	inline void set_family(int fam) { sin.sin_family = fam; }
	inline void set_port(uint16_t port) { sin.sin_port = htons(port); }
	inline void set_addr(uint32_t addr) { sin.sin_addr.s_addr = htonl(addr); }

	void set(int fam, uint32_t addr, uint16_t port)
	{
		set_family(fam);
		set_addr(addr);
		set_port(port);
	}

	void set(uint32_t addr, uint16_t port) { set(AF_INET, addr, port); }
	void set(uint32_t addr) { set(addr, 0); }


	inline Str format_ip() const
	{
		uint32_t addr = ntohl(sin.sin_addr.s_addr);
		return Str::build("%i.%i.%i.%i", (addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, addr&0xFF);
	}

	inline Str format_ip_port() const
	{
		uint32_t addr = this->addr();
		return Str::build("%i.%i.%i.%i:%i", (addr>>24)&0xFF, (addr>>16)&0xFF, (addr>>8)&0xFF, addr&0xFF, port());
	}
};

