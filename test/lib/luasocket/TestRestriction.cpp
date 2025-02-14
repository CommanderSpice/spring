#include "lib/luasocket/src/restrictions.h"

#include <string>
#include <cstdio>

#define CATCH_CONFIG_MAIN
#include "lib/catch.hpp"


TEST_CASE("TestRestriction")
{
	CLuaSocketRestrictions rest = CLuaSocketRestrictions();
	#define CheckAccess(result, type, host, port) \
		CHECK((result == rest.isAllowed((CLuaSocketRestrictions::RestrictType)type, host, port)));

	for(int i=0; i<CLuaSocketRestrictions::SPRING_ALL_RULES; i++) {
		CheckAccess(false, i, "localhost", 80);
		CheckAccess(false, i, "localhost", -1);
		CheckAccess(false, i, "springrts.com", 80);
		CheckAccess(false, i, "springrts.com", -1);
		CheckAccess(false, i, "94.23.170.70", 80);
		CheckAccess(false, i, "94.23.170.70", -1);
		CheckAccess(false, i, "zero-k.info", 80);
		CheckAccess(false, i, "zero-k.info", -1);
	}

	rest.addRule(CLuaSocketRestrictions::SPRING_TCP_CONNECT, "springrts.com", 80, true);

	for(int i=0; i<CLuaSocketRestrictions::SPRING_ALL_RULES; i++) {
		CheckAccess(false, i, "localhost", 80);
		CheckAccess(false, i, "localhost", -1);
		CheckAccess((CLuaSocketRestrictions::SPRING_TCP_CONNECT == i), i, "springrts.com", 80);
		CheckAccess(false, i, "springrts.com", -1);
		CheckAccess(false, i, "94.23.170.70", 80);
		CheckAccess(false, i, "94.23.170.70", -1);
		CheckAccess(false, i, "zero-k.info", 80);
		CheckAccess(false, i, "zero-k.info", -1);
	}

	rest.addIP("springrts.com", "94.23.170.70");

	for(int i=0; i<CLuaSocketRestrictions::SPRING_ALL_RULES; i++) {
		CheckAccess(false, i, "localhost", 80);
		CheckAccess(false, i, "localhost", -1);
		CheckAccess((CLuaSocketRestrictions::SPRING_TCP_CONNECT == i), i, "springrts.com", 80);
		CheckAccess(false, i, "springrts.com", -1);
		CheckAccess((CLuaSocketRestrictions::SPRING_TCP_CONNECT == i), i, "94.23.170.70", 80);
		CheckAccess(false, i, "94.23.170.70", -1);
		CheckAccess(false, i, "zero-k.info", 80);
		CheckAccess(false, i, "zero-k.info", -1);

	}

	rest.addRule(CLuaSocketRestrictions::SPRING_UDP_LISTEN, "localhost", 80, true);

	for(int i=0; i<CLuaSocketRestrictions::SPRING_ALL_RULES; i++) {
		CheckAccess(false, i, "localhost", -1);
		CheckAccess((CLuaSocketRestrictions::SPRING_UDP_LISTEN == i), i, "localhost", 80);
		CheckAccess((CLuaSocketRestrictions::SPRING_TCP_CONNECT == i), i, "springrts.com", 80);
		CheckAccess(false, i, "springrts.com", -1);
		CheckAccess((CLuaSocketRestrictions::SPRING_TCP_CONNECT == i), i, "94.23.170.70", 80);
		CheckAccess(false, i, "94.23.170.70", -1);
		CheckAccess(false, i, "zero-k.info", 80);
		CheckAccess(false, i, "zero-k.info", -1);
	}
	rest.addRule(CLuaSocketRestrictions::SPRING_UDP_CONNECT, "*", -1, true);
	rest.addRule(CLuaSocketRestrictions::SPRING_TCP_CONNECT, "*", -1, true);
	rest.addRule(CLuaSocketRestrictions::SPRING_UDP_LISTEN, "*", -1, true);
	rest.addRule(CLuaSocketRestrictions::SPRING_TCP_LISTEN, "*", -1, true);

	for(int i=0; i<CLuaSocketRestrictions::SPRING_ALL_RULES; i++) {
		CheckAccess(true, i, "localhost", -1);
		CheckAccess(true, i, "localhost", 80);
		CheckAccess(true, i, "springrts.com", 80);
		CheckAccess(true, i, "springrts.com", -1);
		CheckAccess(true, i, "94.23.170.70", 80);
		CheckAccess(true, i, "94.23.170.70", -1);
		CheckAccess(true, i, "zero-k.info", 80);
		CheckAccess(true, i, "zero-k.info", -1);
	}

//blacklist has priority
	rest.addRule(CLuaSocketRestrictions::SPRING_UDP_CONNECT, "*", -1, false);
	rest.addRule(CLuaSocketRestrictions::SPRING_TCP_CONNECT, "*", -1, false);
	rest.addRule(CLuaSocketRestrictions::SPRING_UDP_LISTEN, "*", -1, false);
	rest.addRule(CLuaSocketRestrictions::SPRING_TCP_LISTEN, "*", -1, false);

	for(int i=0; i<CLuaSocketRestrictions::SPRING_ALL_RULES; i++) {
		CheckAccess(false, i, "localhost", -1);
		CheckAccess(false, i, "localhost", 80);
		CheckAccess(false, i, "springrts.com", 80);
		CheckAccess(false, i, "springrts.com", -1);
		CheckAccess(false, i, "94.23.170.70", 80);
		CheckAccess(false, i, "94.23.170.70", -1);
		CheckAccess(false, i, "zero-k.info", 80);
		CheckAccess(false, i, "zero-k.info", -1);
	}

}

