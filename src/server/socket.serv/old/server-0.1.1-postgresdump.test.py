import psycopg2
import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import tornado.web
import datetime
import json

try:
    conn = psycopg2.connect("dbname=electronics user='epower' password='mbed' host='localhost'")
    conn.autocommit = True
except:
    print "ERR:::Cannot connect to database"

cur = conn.cursor()
 
class WSHandler(tornado.websocket.WebSocketHandler):
    read_only = []
    device = []
    controller = []

    def open(self, mode, deviceName, noSockets):
        self.mode = mode
        self.deviceNo = device
        if self.mode == 'read_only':
            self.read_only.append(self)
            print 'new connection in read_only: %s' % self.read_only
        elif self.mode == 'device':
            self.device.append(self)
            print 'new connection in devices: %s' % self.device
            self.count = 0
            self.wattbuff = 0
        elif self.mode == 'controller':
            self.controller.append(self)
            print 'new connection in controllers: %s' % self.controller

        self.write_message("Hello World")

    def on_message(self, message):
        if self.mode == 'read_only':
            print message
        if self.mode == 'device':
             decode_jsonstring = json.loads(message)
             x = decode_jsonstring['watt']
             t = datetime.datetime.now()
             print json.dumps(decode_jsonstring, sort_keys=True, indent=4)
             if self.count <= 4:
                 self.wattbuff += x
                 self.count += 1
             elif (self.count == 5):
                 self.count = 0
                 kwh = self.wattbuff / 1000
                 price = 100 * kwh
                 device_name = 'mbed'
                 deviceNo = '1'
                 try:
                     cur.execute("INSERT INTO wholebuilding VALUES (%s, %s, %s, %s);",
                     (device_name, deviceNo, kwh, price,))
                 except:
                     print "ERR 102: Unable to insert into table 'wholebuilding'"
                 self.wattbuff = 0
                 
             #try:
             #    code = json.loads(message)
                 #print json.dumps(code, sort_keys=True, indent=4)
             #    t = datetime.datetime.now()
             #    print "Socket1: Wattage: %s at %s" % (json_decodestring['watt'], t)
             #except:
                 #print "ERROR 101: JSON format error"
#            log.append(message)
#            print 'ePower: %s' % message
#            for client in self.read_only:
#              client.write_message(message)
#            try:
#                cur.execute("INSERT INTO test VALUES (default, %s);", 
#                (message,))
#                print 'SUCCESS:::Insert successfully updated to database'
#            except:
#                print 'ERR:::Failed to insert'
#        elif self.mode == 'controller':
#            print 'CMD: %s' % message
#            for client in self.device:
#                client.write_message(message)
#                print 'sent to device'
              

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
    (r'/ws/(.*)/(.*)', WSHandler),
])
 
if __name__ == "__main__":
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
