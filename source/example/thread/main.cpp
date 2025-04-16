#include <iostream>
#include "ash.memory.unique.h"
#include "ash.concurrency.h"
#include "ash.ascii.h"


class BackgroundThread : public Ash::Concurrency::Thread
{
public:
	using Function = Ash::Memory::Unique::Pointer<Ash::Callable::Generic::Function<void>>;

	BackgroundThread() : Ash::Concurrency::Thread(), m_Queue() { run(this); }

	~BackgroundThread()
	{
		send(Function());
		join();
	}

	void send(Function &&function) { m_Queue.add(std::move(function)); }

	void operator ()()
	{
		for (;;)
		{
			Function function = m_Queue.remove();
			if (function.isNull())
			{
				break;
			}

			(*function)();
		}
	}

private:
	Ash::Concurrency::Queue1x1<Function> m_Queue;
};


static void helloWorld()
{
	std::cout << "hello world\n";
}

static Ash::Callable::Function helloWorldFunction = helloWorld;


int main(int argumentCount, const char *argumentArray[])
{
	BackgroundThread backgroundThread;

	backgroundThread.send(Ash::Memory::Unique::at(helloWorldFunction));

	backgroundThread.send
	(
		Ash::Memory::Unique::newValue
		(
			Ash::Callable::Function
			(
				[](Ash::Memory::View<Ash::Ascii::Literal::Value> arguments)
				{
					for (Ash::Ascii::Literal::Value value : arguments)
					{
						std::cout << "argument: " << value << "\n";
					}
				},
				Ash::Memory::View<Ash::Ascii::Literal::Value>(argumentArray, argumentCount)
			)
		)
	);

	return 0;
}
