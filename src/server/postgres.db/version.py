#!/usr/bin/python
# -*- coding: utf-8 -*-

import psycopg2
import sys


con = None

try:
     
    con = psycopg2.connect(dbname='jelly_db', user='jelly_user', host='localhost', password='jelly') 
    cur = con.cursor()
    cur.execute('SELECT version()')          
    ver = cur.fetchone()
    print ver    
    

except psycopg2.DatabaseError, e:
    print 'Error %s' % e    
    sys.exit(1)
    
    
finally:
    
    if con:
        con.close()