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

modulecount = 0
totalsockets = 0

class WSHandler(tornado.websocket.WebSocketHandler):
    module = []
    webclient = []

	def open(self, mode, numsockets):
	def open(self, mode):
		self.mode = mode
		if self.mode == 'module':
			try:
				self.numsockets = numsockets
		    except:
		    	module.remove(self)
                pass
            # keeps a record of how many modules are connected to the server
            modulecount += 1
			self.moduleid = modulecount

			# generates a tally of all the modules
			totalsockets += self.numsockets

			module.append(self)
			print ('module has connected, there are currently %d modules connected to the server and %d sockets have been identifiable' %(modulecount,totalsockets))

		elif self.mode == 'webclient':
			webclient.append(self)

    def on_message(self, message):
    	if self.mode == 'module':
    		decode_string = json.loads(message)
    		## Setup and instruct the module on what to do next, this will allow complete control over the initialisation process ##
    		## string in format {type}
    		if decode_string['info']['type'] == 'initialise'

    		""" ## string in format 
    		## 
    		##     info: {
    		##         type, 
    		##         arraylen
    		##     },
    		##     data: [
    		##     {
    		##         timestamp,
    		##         regular: {vol, temp, freq}, 
    		##         outlet1: {cost, watt, curr, phase}, 
    		##         outlet2: {cost, watt, curr, phase}
    		##     },
    		##     {   timestamp, 
    		##         regular: {vol, temp, freq}, 
    		##         outlet1: {cost, watt, curr, phase}, 
    		##         outlet2: {cost, watt, curr, phase}
    		##     },
    		##     {   timestamp, 
    		##         regular: {vol, temp, freq}, 
    		##         outlet1: {cost, watt, curr, phase}, 
    		##         outlet2: {cost, watt, curr, phase}
    		##     },
    		##     {   timestamp, 
    		##         regular: {vol, temp, freq}, 
    		##         outlet1: {cost, watt, curr, phase}, 
    		##         outlet2: {cost, watt, curr, phase}
    		##     } ...
    		## """
    		elif decode_string['type'] == 'alldata'
    		    if module[self].pause != 1
    		        for everyclient in webclient[]
    		        module[self].lastact = datetime.datetime.now()

    		    else logging.warning("ERR Exception: pause returns true, resolving unexpected conflict. Setting module[%d].pause = false" % self.moduleid )


    		## I think the method might not be neccesary depending on the parameters the module is sending in the data string ##
    		# if decode_string['type'] == 'sendall_data'

    		## 
    		if decode_string['type'] == 'activity'

    		## for querying the sockets associated with the module to allow json specific decoding and for making database entries ##
    		# while x < self.numsockets:
    	if self.mode == 'webclient':
            ## Command: Set timer - send new refresh-rate setting to specified module
            ## Command: Set all timers - updates all modules with refresh-rate setting
            ## Command: Pause data feed until further instruction, 
            ## Command: Start streaming data feed
            ## Command: Set socket on/off 
            ## *Command: Create dynamic instruction, i.e switch off socket at x:y time, generate string stored in an array list/database
            ## *Command: Delete/Update dynamic instruction, specify query definition
            ## Can be generated on the client side and directly update or query the database ##
            ## The websocket server just needs to handle the instruction arguments and query the database ##

    def on_close(self):
        if self.mode == 'module':
        	modulecount -= 1
        	totalsockets -= module[self].numsockets
        	module.remove(self)

    	logging.warning("Client closing. Chan: %s, Mode: %s"%(self.channel, self.mode))
        if moduleself.mode == 'rw' or self.mode == 'ro':
            subscribers[self.channel].remove(self)
            if len(subscribers[self.channel]) == 0:
                del subscribers[self.channel]
        elif self.mode == 'wo':
		        try:
		            del publishers[self]
		        except:
			          pass

application = tornado.web.Application([
    (r'/ws/(.*)(.*)', WSHandler),
    (r'/ws/(.*)', WSHandler),
])


def parse_conditions():
	## parse dynamic array list, for instruction array-length times every minute.
	## store dynamic list in database and query every row in the table.

def test():
	for activity in self.modules

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
