import os, json
from flask import Flask, json, request
from flask_cors import CORS
app = Flask(__name__)
CORS(app)

def loadData():
    ROOT = os.path.realpath(os.path.dirname(__file__))
    json_url = os.path.join(ROOT, 'static/banks.json')
    data = json.load(open(json_url))
    return data


@app.route('/bank/list')
def list():
    return json.dumps(data)

@app.route('/bank/find.name/')
def name():
    name = request.args.get("name")
    for bank in data['banks']:
        if (name == bank['name']):
            return json.dumps(bank)
    return 'null'
    
@app.route('/bank/find.key/')
def key():
    key = request.args.get("key")
    for bank in data['banks']:
        if (key == bank['key']):
            return json.dumps(bank)
    return 'null'


global data
data = loadData()

if __name__ == '__main__':
    app.run(port=8071)
