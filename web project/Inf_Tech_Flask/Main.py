#!/usr/bin/env python
# -*- coding: utf-8 -*-
from flask import *
import sqlite3
import json
from json import JSONEncoder, JSONDecoder
from DataBaseConnection import *
from DataTable import DataTable
from DataRow import *
import codecs
import random
import string
import urllib.parse


def getDB():
    conn = sqlite3.connect('database.db')
    conn.execute('CREATE TABLE IF NOT EXISTS `used_tables`(`table` TEXT)')
    cursor = conn.cursor()
    return (conn, cursor)


def randStr():
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=16))


class CustomJSONEncoder(JSONEncoder):
    def default(self, obj):
        if isinstance(obj, DataBaseConnection):
            return obj.toJson()
        elif isinstance(obj, DataBase):
            return obj.toJson()
        elif isinstance(obj, DataTable):
            obj.toJson()
        elif isinstance(obj, DataRow):
            obj.toJson()
        elif isinstance(obj, set):
            return list(obj)
        else:
            JSONEncoder.default(self, obj)

class CustomJSONDecoder(JSONDecoder):
    def defaulf(self, obj):
        if isinstance(obj, DataBaseConnection):
            e = DataBaseConnection()
            e.fromJson(obj)
            return e
        elif isinstance(obj, DataBase):
            return DataBase(obj)
        elif isinstance(obj, DataTable):
            return DataTable(obj)
        elif isinstance(obj, DataRow):
            return DataRow(obj)
        elif isinstance(obj, set):
            return set(obj)
        else:
            JSONEncoder.default(self, obj)


app = Flask(__name__)
app.secret_key = os.urandom(32)
app.json_encoder = CustomJSONEncoder
app.json_decoder = CustomJSONDecoder


@app.route('/')
@app.route('/index')
def index():
    #if 'connection' not in session:
    session['connection'] = DataBaseConnection()
    return render_template('index.html', **locals())


@app.route('/get_data_bases', methods=['GET'])
def get_data_bases():
    conn = DataBaseConnection(session['connection'])
    return conn.getDataBases()


@app.route('/db', methods=['GET'])
def db():
    try:
        name = request.args.get('name')
        tables = session['Tables']
        return render_template('db.html', **locals())
    except:
        return redirect(url_for('index'))


@app.route('/get_table', methods=['GET'])
def get_table():
    conn = DataBaseConnection(session['connection'])
    conn.db = session['db']
    name = request.args.get('name')
    obj = conn.showTable(name)
    return obj


@app.route('/auth', methods=['POST'])
def auth():
    name = request.form['name']
    login = request.form['login']
    password = request.form['password']
    conn = DataBaseConnection(session['connection'])
    obj = dict(json.loads(conn.openDB(name, login, password))['openDataBase'])
    if not obj['Result']:
        return redirect(url_for('/'))
    tables = obj['Tables']
    session['Tables'] = tables
    session['connection'] = conn
    session['db'] = conn.db
    print('Session db obj', session['db'].toJson())
    return redirect('../db?name=' + tables[0])


@app.route('/save_txt_file', methods=['GET'])
def save_txt_file():
    name = 'temp\\' + randStr() + '_' + request.args.get('name')
    content = request.args.get('content')
    content = urllib.parse.unquote(content).encode('cp1251').decode('utf-8')
    # !/usr/bin/env python
    # -*- coding: utf-8 -*-
    # !/usr/bin/env python
    # -*- coding: utf-8 -*-
    with codecs.open(name, "w", "utf-8-sig") as temp:
        temp.write(urllib.parse.unquote(content))
    return name


@app.route('/temp/<path:filename>', methods=['GET', 'POST'])
def download(filename):
    uploads = os.path.join(current_app.root_path, 'temp')
    return send_from_directory(directory=uploads, filename=filename)


@app.route('/dd')
def fdsfdsfgds():
    (conn, cursor) = getDB()
    cursor.execute('INSERT INTO `used_tables` VALUES(\'test_db12\')')
    conn.commit()
    return ''