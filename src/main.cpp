#include <iostream>

int main(int argc, char* argv[])
{
	while (1)
	{
		std::cout << "Choice: 't' for tests, 's' to save image, 'q' to quit" << std::endl;
	
		char c;
		std::cin >> c;

		if (c=='t')
		{
			int print_tests();
			print_tests();
		}
		else if (c=='s')
		{
			std::cout << "path to the image? (TGA file)" << std::endl;
			char szPath[260];
			std::cin >> szPath;
			bool raytrace_MPL(char* outputName);
			raytrace_MPL(szPath);
		}
		else
		{
			return 0;
		}
	}
}