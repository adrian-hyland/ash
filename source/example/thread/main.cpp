#include <cstdio>
#include "ash.ascii.h"
#include "ash.concurrency.queue.h"
#include "ash.concurrency.h"


class OutputThread
{
public:
	OutputThread() : m_Queue(), m_Thread(&m_Queue) { m_Thread.start(); }

	~OutputThread() { send(""); }

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
	Ash::Concurrency::Thread<Runnable>            m_Thread;
};


int main()
{
	OutputThread outputThread;

	outputThread.send("Hello");
	outputThread.send("World");

	return 0;
}
