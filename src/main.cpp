#include "../include/Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "ERROR: ./IRC [port] []" << std::endl;
		return 1;
	}

	try
	{
		Server server(argv);
	}
	catch(const std::exception& e)
	{
		std::cerr << "ERROR: "  << e.what() << '\n';
		return 1;
	}
	return 0;
	
}