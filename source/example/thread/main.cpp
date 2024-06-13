#include <iostream>
#include "ash.memory.unique.h"
#include "ash.concurrency.h"


class BackgroundThread : public Ash::Concurrency::Thread
{
public:
	BackgroundThread() : Ash::Concurrency::Thread(), m_Queue() { run(this); }

	~BackgroundThread()
	{
		send(Ash::Memory::Unique::Pointer<Ash::Callable::Generic::Function>());
		join();
	}

	void send(Ash::Memory::Unique::Pointer<Ash::Callable::Generic::Function> fn) { m_Queue.add(std::move(fn)); }

	void operator ()()
	{
		for (;;)
		{
			Ash::Memory::Unique::Pointer<Ash::Callable::Generic::Function> function = m_Queue.remove();
			if (function.isNull())
			{
				break;
			}

			(*function)();
		}
	}

private:
	Ash::Concurrency::Queue1x1<Ash::Memory::Unique::Pointer<Ash::Callable::Generic::Function>> m_Queue;
};


void helloWorld()
{
	std::cout << "hello world\n";
}


int main(int argumentCount, const char *argumentArray[])
{
	BackgroundThread backgroundThread;

	Ash::Callable::Function helloWorldFunction = helloWorld;
	backgroundThread.send(Ash::Memory::Unique::at(helloWorldFunction));

	backgroundThread.send
	(
		Ash::Memory::Unique::newValue
		(
			Ash::Callable::Function
			(
				[](Ash::Memory::View<const char *> arguments)
				{
					for (const char *s : arguments)
					{
						std::cout << "argument: " << s << "\n";
					}
				},
				Ash::Memory::View<const char *>(argumentArray, argumentCount)
			)
		)
	);

	return 0;
}
