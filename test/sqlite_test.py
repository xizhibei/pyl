#!usr/bin/python

import sqlite3 as sql

conn = sql.connect('user.db')
cur = conn.cursor()

cur.execute('select * from software')

print cur.fetchall()
