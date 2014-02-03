# Jelly Socket Server - 230114 by Daniel Tilbey 2014
# A light-weight web socket server for carrying out JSON string requests
import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import tornado.web
import datetime
import psycopg2
import json

try:
    conn = psycopg2.connect("dbname=jelly_db user='jelly_socket' password='mbed' host='localhost'")
    conn.autocommit = True
    cur = conn.cursor()
except:
    print "ERR:::Cannot connect to database"

LISTENERS = []

"""
set properties:
unix_time = time.time()
unit_cost = 1.5
"""

class WSHandler(tornado.websocket.WebSocketHandler):

    def open(self, mode, key):
        self.mode = mode
            if self.mode == 'jellyutil'
                LISTENERS['jellyutil'] = self

            elif self.mode == 'module'
                LISTENERS['module'] = self

                
    def on_message(self, message):


    def on_close(self):


application = tornado.web.Application([
    (r'/ws/(.*)/(.*)', WSHandler),
], **settings)

if __name__ == "__main__":
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(8890)
    tornado.ioloop.IOLoop.instance().start()