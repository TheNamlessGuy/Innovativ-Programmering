import os
from flask import Flask, json, request
from flask_cors import CORS
import json

app = Flask(__name__)
CORS(app)
data_json = None

@app.route('/person/list')
def list():
    return json.dumps(data_json)

@app.route('/person/find.name/')
def find_name():
    name = request.args.get("name")
    for person in data_json["persons"]:
        if person["name"] == name:
            return json.dumps(person)
    return "null"

@app.route('/person/find.key/')
def find_key():
    key = request.args.get("key")
    for person in data_json["persons"]:
        if person["key"] == key:
            return json.dumps(person)
    return "null"

def loadjson():
    ROOT = os.path.realpath(os.path.dirname(__file__))
    json_url = os.path.join(ROOT, "static", "persons.json")
    return json.load(open(json_url, "r"))

if __name__ == "__main__":
    data_json = loadjson()
    app.run(port=8061)
