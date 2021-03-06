import argparse
from pythonosc import dispatcher
from pythonosc import osc_server

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--ip",
                      default="127.0.0.1", help="The ip to listen on")
  parser.add_argument("--port",
                      type=int, default=7001, help="The port to listen on")
  args = parser.parse_args()

dispatcher = dispatcher.Dispatcher()
dispatcher.map("/noteon", print)
dispatcher.map("/noteoff", print)
# dispatcher.map("/velocity", print)

server = osc_server.ThreadingOSCUDPServer(
    (args.ip, args.port), dispatcher)
print("Serving on {}".format(server.server_address))
server.serve_forever()
