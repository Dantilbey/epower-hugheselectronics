#!/usr/bin/python
# -*- coding: utf-8 -*-

import psycopg2
import sys


con = None

try:
     
    con = psycopg2.connect(dbname='jelly_db', user='jelly_user', host='localhost', password='jelly') 
    con.autocommit = True
    cur = con.cursor()

    cur.execute("INSERT INTO settings(option, value) VALUES ('systemSTATE','true')")   
    cur.execute("INSERT INTO settings(option, value) VALUES ('systemID', 'admin')")
    cur.execute("INSERT INTO settings(option, value) VALUES ('systemKEY', 'password')")
    cur.execute("INSERT INTO settings(option, value) VALUES ('costWattage', '0.5')")
    cur.execute("INSERT INTO settings(option, value) VALUES ('systemBackup', '2014-01-01 00:00:00')")
except psycopg2.DatabaseError, e:
    print 'Error %s' % e    
    sys.exit(1)
    
    
finally:
    
    if con:
        con.close()