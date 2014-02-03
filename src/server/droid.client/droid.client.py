
import websocket

import psycopg2
import sys

if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.create_connection("ws://vnvl.co/ws/droid/daniel")
    print("Sending 'Hello, World'...")
    ws.send("Hello, World")
    print("Sent")
    print("Receiving...")
    result = ws.recv()
    print("Received '%s'" % result)
    ws.close()