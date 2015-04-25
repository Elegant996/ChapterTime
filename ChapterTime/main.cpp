#include "Worker.h"

using namespace std;

int main(int argc, char* argv[])
{
	vector<string> parameters;
	if (argc > 1)
		parameters.assign(argv + 1, argv + argc);

	Worker worker(parameters);
	worker.run();

	return 0;
}