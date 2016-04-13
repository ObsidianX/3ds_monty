class Scheduler:
    active_threads = []
    scheduled_threads = []

    def add_thread(self, thread):
        gen = thread.main()
        next(gen)
        self.active_threads.append(gen)

    def main(self):
        yield None
        while True:
            if len(self.active_threads) == 0:
                yield 1
            for thread in self.active_threads:
                try:
                    next(thread)
                    self.scheduled_threads.append(thread)
                except StopIteration:
                    pass
                yield 1

            self.active_threads = self.scheduled_threads
            self.scheduled_threads = []


class MicroThread:
    def step(self):
        pass

    def create(self):
        pass

    def main(self):
        self.create()
        yield None
        while True:
            self.step()
            yield None
