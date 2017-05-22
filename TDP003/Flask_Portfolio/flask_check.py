#!/usr/bin/env python
# -*- coding: utf-8 -*-
from flask import Flask
app = Flask(__name__)
@app.route('/')
def index():
    return "Flask fungerar! Den kan även ta svenska bokstäver: å ä ö"

app.run()
