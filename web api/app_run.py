from flask import *
import sqlite3
import urllib.parse
import json
from pymongo import MongoClient


app = Flask(__name__)
CONNSTR = "mongodb://admin:admin@ds257245.mlab.com:57245/queries"
DBNAME = "queries"
OBJECT_QUERY = "Query"
OBJECT_RESPONSE = "Response"


def get_db(COLNAME = OBJECT_QUERY):
    client = MongoClient(CONNSTR)
    db = client[DBNAME]
    col = db[COLNAME]
    return col
#    db = getattr(g, '_database', None)
#    if db is None:
#        db = g._database = sqlite3.connect(DATABASE)
#        db.execute("CREATE TABLE IF NOT EXISTS queries (id INTEGER PRIMARY KEY AUTOINCREMENT, author TEXT, query TEXT)")
#    return db


@app.teardown_appcontext
def close_connection(exception):
	pass
#    db = getattr(g, '_database', None)
#    if db is not None:
#        db.close()


@app.route('/')
@app.route('/index')
def index():
    mes = 'ee'
    db = get_db()
    jsons = list(db.find())
#    cursor = db.cursor()
    #jsons = []
#    cursor.execute("SELECT * FROM queries WHERE author = 'Client_2' ORDER BY id ASC")
#    jsons = cursor.fetchall() 
    return render_template('index.html', **locals())

@app.route('/put', methods=['POST'])
def put():
    author = request.form['author']
    query = request.form['query']
    time = request.form['time']
    db = get_db()
    data = {
    	'author': author,
    	'query': query,
    	'time': time
    }
    db.insert_one(data)   
 #   cursor = db.cursor()
 #   cursor.execute("INSERT INTO queries (author, query) VALUES ('" + author + "', '" + query + "')")
 #   db.commit()
    return render_template('index.html', **locals())


@app.route('/get', methods=['GET'])
def get():
    author = request.args.get('author')
    db = get_db(OBJECT_RESPONSE)
    jsons1 = list(db.find({'to': author}).sort('time', 1))
    jsons = '['
    for e in jsons1:
        cur = dict(e)
        db.delete_one({'_id': cur['_id']}) 
        if 'signup' != e['response']:
            if len(jsons) > 1:   jsons += ','
            jsons += e['response']
    jsons += ']'
    return render_template('index.html', **locals())


'''
@app.route('/API/add_data', methods=['POST'])
def add_data():
    name = request.form['name']
    description = request.form['description']
    if name != "" and description != "":
        add_data = UserData(
            Name=name,
            Description=description,
            CreateDate=datetime.now()
        )
        db.session.add(add_data)
        db.session.commit()
    return redirect('index')'''


if __name__ == '__main__':
    app.run(debug=True)
