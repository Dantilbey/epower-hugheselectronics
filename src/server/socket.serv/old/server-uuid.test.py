# network driver and websocket server application api
import tornado.httpserver
import tornado.websocket
import tornado.ioloop

# initialise microprocessor
from time import gmtime, strftime

# api for decoding strings into objects
import json 

# database driver
import psycopg2 

# storage
modulecount = 0
totalsockets = 0
module = []
webclient = []

class WSHandler(tornado.websocket.WebSocketHandler):

    def open(self, mode, numsockets):
    def open(self, mode):   
        self.mode = mode
        if self.mode == 'module':
        try:
            self.numsockets = numsockets
            if numsockets >== 0 <== 225
            self.id = uuid.uuid4()
            module[self.id] = {'id':self.id}
        except:
            logging.warning("ERR Exception: Client module selected with unspecified number of sockets")
            pass

        elif self.mode == 'webclient':

            self.id = uuid.uuid4()
            webclient[self.id] = {'id':self.id}
        else 

        self.id = uuid.uuid4()
        module[self.id] = {'id':self.id}
        print 'new connection'

    def on_message(self, message):
        #Some message parsing here
        if message.type == 'set_group':
           external_storage[self.id]['group'] = message.group
        print 'message received %s' % message

    def on_close(self):
        if self.mode == 'module':
            ##
            self.id = uuid.uuid4()
            module.remove(self.id)
            print 'closed connection'
        elif self.mode == 'webclient':
            ##
            self.id = uuid.uuid4()
            webclient.remove(self.id)
            print 'closed connection'

application = tornado.web.Application([
    (r'/ws/(.*)(.*)', WSHandler),
    (r'/ws/(.*)', WSHandler),
])

def parse_conditions():
    ## parse dynamic array list, for instruction array-length times every minute. ##
    ## store dynamic list in database and query every row in the table. ##

    ##
def test():
    ## Check for connection state and handle data transmission errors ##

    ##
    for k,v in publishers.items():
        delta = datetime.datetime.now() - v['lastact'] 
        if delta.seconds > 1:
            logging.warning("Callback. Client %s did not send data in the last 2 secs. Closing"%k)
                del publishers[k]
            v['stream'].stream.close()

if __name__ == "__main__":
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(8888)
    loop = tornado.ioloop.IOLoop.instance()
    tornado.ioloop.PeriodicCallback(test, 1000, io_loop=loop).start() ## 1 second
    tornado.ioloop.PeriodicCallback(parse_conditions, 60000, io_loop=loop).start() ## 1 minute
    loop.start()
