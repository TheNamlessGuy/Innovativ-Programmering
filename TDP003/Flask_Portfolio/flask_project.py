#!/usr/bin/env python
# -*- coding: utf-8 -*-
from flask import Flask
from flask import url_for
from flask import render_template
from flask import request
from flask import redirect
import data
app = Flask(__name__)
@app.route('/')
def index():
    data.log("FLASK SERVER: Trying to access index page ('/')")
    db = data.load("data.json")
    if not db:
        data.log("FLASK SERVER: Database could not be read")
        return redirect("/error/db")
    db_len = data.get_project_count(db)
    if not db_len:
        data.log("FLASK SERVER: Length of database could not be read")
        return redirect("/error/len")
    data.log("FLASK SERVER: Access to index page ('/') successful")
    return render_template('index.html', projects=db, db_len=db_len)

@app.route('/list')
def list():
    data.log("FLASK SERVER: Trying to access list/archive page ('/list')")
    db = data.load("data.json")
    if not db:
        data.log("FLASK SERVER: Database could not be read")
        return redirect("/error/db")
    techniques_used = data.get_techniques(db)
    if not techniques_used:
        data.log("FLASK SERVER: Techniques used could not be read")
        return redirect("/error/tech_u")
    sort_by = request.args.get('sort_by', 'start_date')
    sort_order = request.args.get('sort_order', 'desc')
    techniques = request.args.getlist('techniques', None)
    search = request.args.get('searchBox', None)
    search_fields = request.args.getlist('search_fields', None)
    search_return = data.search(database=db, sort_by=sort_by, sort_order=sort_order, techniques=techniques, search=search, search_fields=search_fields)
    if search_return is None:
        data.log("FLASK SERVER: Search failed to search in data")
        return redirect('/error/search')
    data.log("FLASK SERVER: Access to list/archive page ('/list') successful")
    return render_template('list.html', techniques_used=techniques_used, search_return=search_return)

@app.route('/techniques')
def techniques():
    data.log("FLASK SERVER: Trying to access techniques page ('/technique')")
    db = data.load("data.json")
    if not db:
        data.log("FLASK SERVER: Database could not be read")
        return redirect("/error/db")
    technique_stats = data.get_technique_stats(db)
    if not technique_stats:
        data.log("FLASK SERVER: Technique Stats could not be read")
        return redirect("/error/tech_s")
    data.log("FLASK SERVER: Access to techniques page ('/technique') successful")
    return render_template('techniques.html', technique_stats = technique_stats)

@app.route('/project/<project_id>')
def project(project_id):
    try:
        project_id = int(project_id)
    except:
        data.log("FLASK SERVER: The project id '" + str(project_id) + "' is not a valid id")
        return redirect('/error/project')
    data.log("FLASK SERVER: Trying to access project page ('/project/ID') with id '" + str(project_id) + "'")
    db = data.load("data.json")
    if not db:
        data.log("FLASK SERVER: Database could not be read")
        return redirect('/error/db')
    project = data.get_project(db, project_id)
    if not project:
        data.log("FLASK SERVER: Project with id '" + str(project_id) + "' could not be read")
        return redirect('/error/project')
    data.log("FLASK SERVER: Access to project page ('/project/ID') with id '" + str(project_id) + "' successful")
    return render_template('projectID.html', project=project)

@app.route('/error/<error_type>')
def error_page(error_type):
    error_db = {'db': "The database couldn't be read",
                'len': "The length of the database couldn't be gotten",
                'project': "A project with that ID does not exist",
                'tech_u': "Could not get the techniques used",
                'tech_s': "Couldn't read the technique stats",
                'search': "Couldn't fullfill the search",
                'page': "The page could not be found"}
    error_message = error_db.get(error_type, "An unexpected error occured")
    return render_template('error.html', error_message = error_message)

@app.route('/<path>')
def catch_all(path):
    data.log("FLASK SERVER: Tried to access page: /" + str(path))
    return redirect('/error/page')

@app.errorhandler(404)
def error_handler(error):
    return redirect('/error/page')
app.run(debug=True)
