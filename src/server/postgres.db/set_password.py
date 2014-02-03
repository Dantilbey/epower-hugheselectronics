#!/usr/bin/python
# -*- coding: utf-8 -*-

import psycopg2
import sys

#from optparse import OptionParser

#parser = optionParser()
#usage = "usage: %prog [options] arg"
#parser = OptionParser(usage)
#parser.add_option("--password", "-p", attr=arg, dest="password")


con = None

try:
     
    con = psycopg2.connect(dbname='jelly_db', user='jelly_user', host='localhost', password='jelly') 
    con.autocommit = True
    cur = con.cursor()

    cur.execute("UPDATE settings SET value='5zcR3i2mz4JjiNjjHcpyPKFNURIaLRaWQ2eGDaXCvcqTebTWmFiW4Gm8CQDdH1N' WHERE option='systemKEY'")   

except psycopg2.DatabaseError, e:
    print 'Error %s' % e    
    sys.exit(1)
    
    
finally:
    
    if con:
        con.close()