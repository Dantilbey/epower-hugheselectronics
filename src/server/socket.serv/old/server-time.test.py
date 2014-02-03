import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import tornado.web
import datetime
from time import gmtime, strftime
import json
 
class WSHandler(tornado.websocket.WebSocketHandler):
    device = []

    def open(self, mode):
        self.mode = mode
        if self.mode == 'device':
            self.device.append(self)
            print 'new connection in devices: %s' % self.device
            self.count = 0
            self.wattbuff = 0

        self.write_message("Hello World")

    def on_message(self, message):
        if self.mode == 'device':
            if message[0] == 'x':
                #t = datetime.datetime.now()
                #time = strftime('%Y%m%d%H%M%S', gmtime())
                #print (time)
                t = time.time()
                self.write_message("%s" % t)
            if message[0] == 'a':
                message.replace(@, '')
                #n = 1
                #while n < len(message):
                #    string_recv[n-1] = message[n]
                #    n = n+1 
                print (message)
               
    def on_close(self):
        if self.mode == 'read_only':
            self.read_only.remove(self)
            print 'closed connection'
        if self.mode == 'device':
            self.device.remove(self)
            print 'closed connection'
        if self.mode == 'controller':
            self.controller.remove(self)
            print 'closed connection'
 
 
application = tornado.web.Application([
    (r'/ws/(.*)', WSHandler),
])
 
if __name__ == "__main__":
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
