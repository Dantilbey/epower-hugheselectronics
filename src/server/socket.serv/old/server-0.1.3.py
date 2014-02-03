import tornado.httpserver
import tornado.websocket
import tornado.ioloop
from time import gmtime, strftime
import datetime
import logging
import json
import psycopg2

## STORAGE ##
module = {}
webclient = {}
outlets = [totaloutlets] ## outlet dependant parameters
#data = [{[{},{},{}]},{[{},{},{}]},{[{},{},{}]},{[{},{},{}]},{[{},{},{}]}]
#stream = []
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

        if self.mode == 'webclient': ##
        	self.channel = arg
        	if not webclient.get(self.channel):
        		webclient[self.channel] = []
        	webclient[self.channel].append(self)
        	if self.channel = 'read':
        	    webclient[self.channel][self] = {'stream':[0]*modulecount}
        	elif self.channel = 'none':
        		pass
        	else 
        	    logging.debug("ERR Exception: An anonymous connection has opened a connection. TERMINATING SOCKET...")
        	    close()

    def on_message(self, message):
    	decode_string = json.loads(message)
    	if self.mode == 'module':
    		if decode_string['info']['type'] = 'initialize':
    			pass """ Control initial setup procedure
    					 parameters: set socket[x] on/off
    					             set start on/off
    					             set timestamp current_time
    					             set timer 1~60
                                     set cost
    			     ## Module will need to initialize and wait for parameters in order to process data
                     """

    		if decode_string['info']['type'] = 'data':
    			pass """ Update and insert data into database table: modules
                """

    		if decode_string['info']['type'] = 'stream':
    			if module[self]['pause'] = 0
    			    for client in webclient.get('read', []):
	                    if webclient[client]['stream'][(module[self].moduleid)] = 1
                		    ## send message string to webclients
                	        client.write_message(message)
                	    else
                	        pass
	            else
	                logging.debug("ERR Exception: pause is set to true, unexpected message type. Changing module settings stream to false.")
	                self.write_message("=S0")

    		if decode_string['info']['type'] = 'status':
    			pass """ is used for setting the status of the sockets in the database, the web
    			         socket server handles the communication to the database table: modulestatus
    			"""

    		if decode_string['info']['type'] = 'error':
    			pass """ is used for sending error messages to the database in the table: messages
    			         the messages can be accessed via a secured web session.
    			"""

        if self.mode == 'webclient':
        	if decode_string['info']['type'] = 'settimer':
        		pass """ set timer refresh-rate for updating data array for all modules
        		"""

        	if decode_string['info']['type'] = 'setarray_size':
        		pass """ set timer refresh-rate for updating data array for all modules
        		"""

        	if decode_string['info']['type'] = 'pause':
        		pass """ turn off streaming and set pause on - identify module
        		         if streaming is active, data refresh-rate will be every second
        		"""

            if decode_string['info']['type'] = 'unpause':
            	pass """ turn on streaming and set pause off - identify module
            	"""

        	if decode_string['info']['type'] = 'start':
        		pass """ send start signal - identify module
        		"""

        	if decode_string['info']['type'] = 'stop':
        		pass """ send stop signal - identify module
        		"""

        	if decode_string['info']['type'] = 'setoutlet':
        		pass """ set socket on/off - the web client utilises a database connection for aquiring the following parameters:
        		         moduleid, moduledesc, outletnum, appliancename
        		"""

            if decode_string['info']['type'] = 'reset':
        		pass """ set reset and close connection - identify module
        		"""

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
    	elif self.mode == 'webclient':
    		logging.warning("Closed Webclient socket in channel:read")
    		if self.channel == 'read':
    			if len(webclient['read']) == 0:
    				del webclient['read']
    			pass """ 
    			"""
    		elif self.channel == 'none':
    			if len(webclient['none']) == 0:
    				del webclient['none']
    			pass """
    			"""
    	else
    	    logging.debug("ERR Unhandled exception: Socket opened undetected! Anonymous socket has closed.")
            """!! Should send server messages to database !!"""

	def close(self):
	    """Closes this Web Socket.

	    Once the close handshake is successful the socket will be closed.
	    """
	    self.ws_connection.close()
	    self.ws_connection = None

def test_():
	if len(webclient['read']) == 0:
		for module_ in module
		    if module[module_].pause != 1:
		    	logging.debug("ERR Caught exception: No available webclients in channel:\'read\', pausing module:%s " % module[module_].moduleid)
		        module_.write_message("=S0")
		    else
		        pass

application = tornado.web.Application([
    (r'/ws/(.*)/(.*)', WSHandler),
], **settings)

if __name__ == "__main__":
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(8888)
    loop = tornado.ioloop.IOLoop.instance()
    tornado.ioloop.PeriodicCallback(test_, 1000, io_loop=loop).start()
    #tornado.ioloop.PeriodicCallback(test, 1000, io_loop=loop).start()
    loop.start()