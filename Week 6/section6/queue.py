class Queue(object):
      def __init__(self, q):
          self.q = q

      def enqueue(self, x):
          self.q.append(x)

      def dequeue(self):
          self.q.pop(0)

      def print_queue(self):
          print(self.q)

      def clean(self):
          self.q = []

lijst = Queue([])
lijst.enqueue(30)
lijst.enqueue(10)
lijst.enqueue(5)
lijst.print_queue()
lijst.enqueue(10)
lijst.print_queue()
lijst.enqueue(1337)
lijst.print_queue()
