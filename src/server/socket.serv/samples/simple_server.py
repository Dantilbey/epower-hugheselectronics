import threading
import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import tornado.web
import redis 
 
LISTENERS = []
 
 
def redis_listener():
r = redis.Redis(host='YOURSERVER', db=2)
r.subscribe('test_realtime')
for message in r.listen():
for element in LISTENERS:
element.write_message(unicode(message['data']))
 
 
class NewMsgHandler(tornado.web.RequestHandler):
def get(self):
self.write(TEMPLATE)
 
def post(self):
data = self.request.arguments['data'][0]
r = redis.Redis(host='YOURSERVER', db=2)
r.publish('test_realtime', data)
 
class RealtimeHandler(tornado.websocket.WebSocketHandler):
def open(self):
LISTENERS.append(self)
 
def on_message(self, message):
pass
 
def on_close(self):
LISTENERS.remove(self)
 
 
settings = {
'auto_reload': True,
}
 
application = tornado.web.Application([
(r'/', NewMsgHandler),
(r'/realtime/', RealtimeHandler),
], **settings)
 
 
if __name__ == "__main__":
threading.Thread(target=redis_listener).start()
http_server = tornado.httpserver.HTTPServer(application)
http_server.listen(8888)
tornado.ioloop.IOLoop.instance().start()