import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import time
import datetime
import logging
import json

class Data():
    modulecount = 0
    totaloutlets = 0
    module = {}
    outlets = []

class WSHandler(tornado.websocket.WebSocketHandler):
    def open(self, mode, arg='none'):
        self.mode = mode
        if self.mode == 'module':
            if arg == 'none':
                self.numoutlets = 0
            elif arg == '2':
                self.numoutlets = 2

            if self.numoutlets >= 0 <= 255:
                Data.module[self] = {'moduleid':Data.modulecount, 'numoutlets':self.numoutlets, 'activeoutlets':[0]*self.numoutlets, 'pause':1, 'stop':1, 'lastact':datetime.datetime.now()}
                Data.modulecount += 1
                Data.totaloutlets += self.numoutlets

    def on_message(self, message):
        print(message)
        try:  
            decode_string = json.loads(message)
            print(decode_string)
            if self.mode == 'module':
                if decode_string['info']['type'] == 'validate':
                    self.id = decode_string['info']['numoutlets']
                    unix_time = time.time()
                    unit_cost = 1.52
                    self.write_message("=!%s/1.52$2@30>1&11" % unix_time)
        except:
            print("Recieved unformatted string")

            if decode_string['info']['type'] == 'message':
                print("%s" % (decode_string['info']['print']))

    def on_close(self):
        if self.mode == 'module':
            Data.modulecount -= 1
            Data.totaloutlets -= self.numoutlets
        try:
            del Data.module[self]
        except:
            logging.warning("Warning: Unable to identify object to delete")
            pass

application = tornado.web.Application([
    (r'/ws/(.*)/(.*)', WSHandler),
])

if __name__ == "__main__":
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
