#include "network_service.h"
#include "game_object.h"

int main(int agrc, const char* argv[]) {
	paradox::network_service service;
	service.initialize();
	paradox::logic_object logic_object(service);
	service.subscribe(logic_object);
	service.run();
	return 0;
}