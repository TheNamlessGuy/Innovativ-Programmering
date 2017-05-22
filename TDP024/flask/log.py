import os
from flask import Flask, request
from flask_cors import CORS
app = Flask(__name__)
CORS(app)

@app.route('/log/')
def log():
    message = request.args.get('msg');
    with open('log/log.txt', 'a+') as log_file:
        log_file.write(message + '\n')
    return 'OK'

if __name__ == "__main__":
    app.run(port=8050)
