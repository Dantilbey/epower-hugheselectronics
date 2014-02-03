import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import datetime
import logging
import json

## STORAGE ##
module = {}
outlets = []
modulecount = 0
totaloutlets = 0

class WSHandler(tornado.websocket.WebSocketHandler):

    def open(self, mode, arg='none'):
        self.mode = mode
        if self.mode == 'module':
        	self.numoutlets = arg
        	if self.numoutlets >== 0 <== 255
        	    module[self] = {'moduleid':modulecount, 'numoutlets':self.numoutlets, 'activeoutlets':[0]*self.numoutlets, 'pause':1, 'stop':1, 'lastact':datetime.datetime.now()}
        	    modulecount++
        	    totaloutlets += self.numoutlets
                logging.warning("New module connected at %s with %s identifiable outlets" % (module['self']['lastact'],module['self']['numoutlets']))
            else
                logging.debug("ERR Exception: An invalid datatype or value has been passed. TERMINATING SOCKET...")

    def on_message(self, message):
        decode_string = json.loads(message)
        if self.mode == 'module':
            if decode_string['info']['type'] = 'initialise':
            	self.id = decode_string['info']['key']
        	    unix_time = time.time()
        	    unit_cost = 1.5
                self.write_message("=!%s£%d$2%30^1&00" % (unix_time,unit_cost))
            if decode_string['info']['type'] = 'message':
            	print("%s" % decode_string['info']['print'])

    def on_close(self):
    	if self.mode == 'module':
    		modulecount--
    		totaloutlets -= self.numsockets
    		module.remove(self)
    		try:
    			del module[self]
    	    except:
    	    	logging.warning("Warning: Unable to identify object to delete")
    	    	pass

application = tornado.web.Application([
    (r'/ws/(.*)/(.*)', WSHandler),
], **settings)

if __name__ == "__main__":
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(8888)
    tornado.ioloop.IOLoop.instance().start()