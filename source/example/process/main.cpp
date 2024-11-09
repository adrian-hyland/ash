#include <iostream>
#include "ash.filesystem.h"
#include "ash.concurrency.h"


int main(int argc, char *argv[], char *envp[])
{
	std::cout << "Process ID: " << Ash::Concurrency::Process::getCurrentIdentifier() << "\n";

	std::cout << "\nProcess arguments:\n";
	for (int n = 0; n < argc; n++)
	{
		std::cout << "argv[" << n << "]: " << argv[n] << "\n";
	}

	std::cout << "\nProcess environment variables:\n";
	if (envp != nullptr)
	{
		for (char **e = envp; *e != nullptr; e++)
		{
			std::cout << *e << "\n";
		}
	}

	if (argc <= 1)
	{
		Ash::Concurrency::Process::Environment::Block block
		(
			{
				Ash::Concurrency::Process::Environment::get("PATH"),
				Ash::Concurrency::Process::Environment::get("TMP"),
				{ "test1", "value1" },
			}
		);
		block.set({ "test2", "hello world" });

		std::cout << "\nSelf replicating...\n";
		Ash::Concurrency::Process process;
		// Variety of different command line arguments to check that they get passed as given when running in a Windows environment...
		if (!process.run({ Ash::Concurrency::Process::getCurrentName(), "argument 1", "I said \"hello world!\"", "argument\"3", "test\\path with\\spaces\\", "\\\"argument\\5\\" }, block))
		{
			std::cout << "Failed to start process\n";
		}
	}

	return 0;
}
