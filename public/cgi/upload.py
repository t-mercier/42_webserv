#!/usr/bin/env python3

import cgi
import cgitb
import os

form = cgi.FieldStorage()

print("Content-Type: text/html\r") 
print("\r")

file_item = form['file']

print("<h1>upload result</h1>")

DIR = "public/uploads"

if file_item.filename:
    filename = os.path.basename(file_item.filename)
    os.makedirs(DIR, exist_ok=True)
    open(DIR + '/' + filename, 'wb').write(file_item.file.read())
    print(f"<h1>File '{filename}' uploaded successfully!</h1>")
else:
    print("<h1>No file was uploaded!</h1>")
