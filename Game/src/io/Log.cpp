#include "Log.h"

#include <iostream>

#include "../debug/Debug.h"

namespace io {

	void log(const std::string& message, const LogType& type)
	{
		switch (type)
		{
		case LOGTYPE_INFO: std::cout << "Game> "; break;
		case LOGTYPE_WARNING: std::cout << "Game, warning> "; break;
		case LOGTYPE_ERROR: std::cout << "Game, error> "; break;
		default: debug::crash("Invalid log type: " + std::to_string(type)); break;
		}
		std::cout << message << std::endl;
	}

}