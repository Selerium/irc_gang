#include "../include/Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "ERROR: ./IRC [port] []" << std::endl;
		return 1;
	}

	Server server(argv);
	return 0;
}