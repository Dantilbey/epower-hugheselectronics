#!/usr/bin/python
# -*- coding: utf-8 -*-

import psycopg2
import sys

con = None

try:
     
    con = psycopg2.connect(dbname='jelly_db', user='jelly_user', host='localhost', password='jelly') 
    con.autocommit = True
    cur = con.cursor()

    #cur.execute("INSERT INTO room(description, efficiencyrating) VALUES ('main office',100)")
    #cur.execute("INSERT INTO appliance(applianceID,description) VALUES ('NONE','An appliance has not been connected.')")
    #cur.execute("INSERT INTO appliance(applianceID,description) VALUES ('UNSPECIFIED','Appliance has been connected but has not yet been specified.')")
    cur.execute("INSERT INTO members(username,password) VALUES ('admin','password')")

    #cur.execute("INSERT INTO module(modulekey, RoomNo, ApplianceNo, description, connected, active) VALUES ('zPcFUVZn4auapcsTA8PgYTbm',1,1,'Module has 1 outlet and is located by the Sales B Computer.','false','false')")

except psycopg2.DatabaseError, e:
    print 'Error %s' % e    
    sys.exit(1)
    
    
finally:
    
    if con:
        con.close()