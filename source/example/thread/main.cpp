#include <cstdio>
#include "ash.ascii.h"
#include "ash.concurrency.queue.h"
#include "ash.concurrency.h"


class OutputThread : public Ash::Concurrency::Thread
{
public:
	OutputThread() : Ash::Concurrency::Thread(), m_Queue() { run<Runnable>(&m_Queue); }

	~OutputThread()
	{
		send("");
		join();
	}

	void send(Ash::Ascii::String<> &&message) { m_Queue.add(std::move(message)); }

protected:
	class Runnable
	{
	public:
		Runnable(Ash::Concurrency::Queue<Ash::Ascii::String<>> *queue) : m_Queue(queue) {}

		void run()
		{
			for (;;)
			{
				Ash::Ascii::String<> string;

				if (m_Queue->remove(string))
				{
					if (string.getLength() == 0)
					{
						break;
					}

					printf("%.*s\n", (int)string.getLength(), string.at(0));
				}
			}
		}

	private:
		Ash::Concurrency::Queue<Ash::Ascii::String<>> *m_Queue;
	};

private:
	Ash::Concurrency::Queue<Ash::Ascii::String<>> m_Queue;
};


int main()
{
	OutputThread outputThread;

	outputThread.send("Hello");
	outputThread.send("World");

	return 0;
}