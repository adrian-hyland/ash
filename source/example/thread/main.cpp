#include <iostream>
#include "ash.pointer.h"
#include "ash.concurrency.h"
#include "ash.ascii.h"


class BackgroundThread : public Ash::Concurrency::Thread
{
public:
	using Function = Ash::Unique::Pointer<Ash::Callable::Generic::Function<void>>;

	BackgroundThread() : Ash::Concurrency::Thread(), m_Queue() { run(this); }

	~BackgroundThread()
	{
		send(Function());
		join();
	}

	void send(Function function) { m_Queue.add(std::move(function)); }

	void operator ()()
	{
		for (;;)
		{
			Function function = m_Queue.remove();
			if (function == nullptr)
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


int main(int argumentCount, const char *argumentArray[])
{
	BackgroundThread backgroundThread;

	backgroundThread.send(Ash::Unique::value(Ash::Callable::Function(helloWorld)));

	backgroundThread.send
	(
		Ash::Unique::value
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
