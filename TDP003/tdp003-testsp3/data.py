#!/usr/bin/env python
# -*- coding: utf-8 -*-
def load(file_directory):
    """Loads a .JSON file, and returns the data"""
    import json
    log("Trying to load datafile '" + str(file_directory) + "'")
    try:
        with open(file_directory) as json_file:
            data = json.load(json_file)
            log("Loaded datafile '" + str(file_directory) + "'")
            return data
    except:
        log("Failed to get datafile '" + str(file_directory) + "'")
        return None

def get_project_count(database):
    """Retrieves the number of projects in the database"""
    log("Trying to get length of database")
    if isinstance(database, list):
        log("Got length of database. Length was '" + str(len(database)) + "'")
        return len(database)
    else:
        log("Failed to get length of database")
        return None

def get_project(database, project_id):
    """Returns the project with the specified id in the database"""
    log("Trying to get project '" + str(project_id) + "' from database")
    if isinstance (database, list):
        if isinstance (project_id, int):
            for i in range(len(database)):
                if (database[i]['project_no'] == project_id):
                    log("Got project with id '" + str(project_id) + "'")
                    return database[i]
    log("Failed to get project with id '" + str(project_id) + "'")
    return None

def get_techniques(database):
    """Returns a list of techniques used in all projects"""
    returnValue = []
    log("Trying to get techniques from database")
    if isinstance (database, list):
        for i in range(len(database)): #Goes through all the projects in the database
            techniques_used = database[i]['techniques_used'] #Gets the techniques in the current project
            for j in range(len(techniques_used)):
                #Checks if each technique gotten is in returnValue already 
                if not techniques_used[j] in returnValue:
                    returnValue.append(techniques_used[j])
        returnValue.sort()
        log("Got techniques used")
        return returnValue
    log("Failed to get techniques used")
    return None

def get_technique_stats(database):
    """Returns a dict with all the stats for all the techniques, organized after technique"""
    log("Trying to get technique stats")
    import collections
    if (isinstance(database, list)):
        techniques_used = get_techniques(database)
        returnDict = {}
        for i in range (len(techniques_used)):
            returnDict[techniques_used[i]] = get_projects_with_technique(database, techniques_used[i])
            #Adds all the stats for the projects used under the technique currently being checked in the loop into a dict with the technique as the key.
        log("Got the technique stats")
        sortedReturnDict = collections.OrderedDict(sorted(returnDict.items(), reverse=True))
        return sortedReturnDict
        #return returnDict
    log("Failed to get technique stats")
    return None

def get_projects_with_technique(database, technique):
    """Returns a list which contains all the stats for the technique inputted"""
    import collections
    log("Trying to get projects where '" + technique + "' was used")
    returnList = []
    dict_to_sort = []
    dict_to_add = {}
    list_of_names = []
    index = 0
    for i in range(len(database)):
        if technique in database[i]['techniques_used']:
            dict_to_sort.append({'id': database[i]['project_no'], 'name': database[i]['project_name']})
    for i in range(len(dict_to_sort)):
        list_of_names.append(dict_to_sort[i]['name'])
    list_of_names.sort()
    for i in range(len(list_of_names)):
        for j in range(len(dict_to_sort)):
            if dict_to_sort[j]['name'] == list_of_names[i]:
                index = dict_to_sort.index(dict_to_sort[j])
                break;
        dict_to_add = {'id': dict_to_sort[index]['id'], 'name': dict_to_sort[index]['name']}
        returnList.append(dict_to_add)
    log("Got projects where '" + technique + "' was used")
    return returnList

def search(database, sort_by='start_date', sort_order='desc', techniques=None, search=None, search_fields=None):
    """Returns a sorted list matching the inputted criteria"""
    returnValue = []
    search_is_none = False
    log("Trying to search for '" + str(search) + "'")
    if search == None:
        search_is_none = True
    else:
        search = search.upper()
    if not techniques==None or not search_fields==None:
        #Search in techniques, search_field or both
        returnValue = search_techniques_and_search_field(database, search, techniques, search_fields, search_is_none)
    else:
        #Search in everything
        returnValue = search_all(database, search, search_is_none)
    returnValue = sort_search_list(returnValue, sort_by, sort_order)
    log("Search for '" + str(search) + "' successful")
    print(len(returnValue))
    return returnValue

def search_all(database, search, search_is_none):
    """Search without restrictions"""
    returnList = []
    log("Searched in no particular technique or field for '" + str(search) + "'")
    for i in range(len(database)):
        for key in database[i]:
            valueStr = str(database[i][key])
            if search_is_none:
                search = valueStr
            if search in valueStr:
                if not database[i] in returnList:
                    returnList.append(database[i])
    if search_is_none:
        search = None
    log("Search for '" + str(search) + "' with no particular restriction on technique or field successful")
    return returnList

def search_techniques(database, search, techniques, search_is_none):
    """Search with restrictions to the techniques used"""
    returnList = []
    log("Searched with technique restriction '" + ', '.join(techniques) + "' for '" + str(search) + "'")
    for i in range(len(database)):
        for key in database[i]:
            valueStr = str(database[i][key])
            if search_is_none:
                search = valueStr
            if search in valueStr:
                for j in range(len(techniques)):
                    if techniques[j] in database[i]['techniques_used']:
                        if not database[i] in returnList:
                            returnList.append(database[i])
    log("Search for " + str(search) + " with restriction for the techniques '" + ', '.join(techniques) + "' successful")
    return returnList

def search_search_fields(database, search, search_fields, search_is_none):
    """search in the specified fields"""
    log("Searched with fields restriction '" + ', '.join(search_fields) + "' for '" + str(search) + "'")
    returnList = []
    for i in range(len(database)):
        for key in database[i].items():
            valueStr = str(key[1])
            if key[0] in search_fields:
                if search_is_none:
                    search = valueStr
                if search in valueStr:
                    if not database[i] in returnList:
                        returnList.append(database[i])
    log("Search for '" + str(search) + "' with restriction for the fields '" + ', '.join(search_fields) + "' successful")
    return returnList

def search_techniques_and_search_field(database, search, techniques, search_fields, search_is_none):
    """Searches in both techniques and search_fields, and checks if you only want to search in one of them"""
    if techniques == None or len(techniques) == 0:
        #Search in fields only
        return search_search_fields(database, search, search_fields, search_is_none)
    elif search_fields == None or len(search_fields) == 0:
        #Search in techniques only
        return search_techniques(database, search, techniques, search_is_none)
    else:
        #Search in both search fields and techniques
        log("Searched with technique restrictions '" + ', '.join(techniques) + "' and field restrictions '" + ', '.join(search_fields) + "' for '" + search + "'")
        returnList = []
        for i in range(len(database)):
            for key in database[i].items():
                valueStr = str(key[1])
                if key[0] in search_fields:
                    if search_is_none:
                        search = valueStr
                    if search in valueStr:
                        for j in range(len(techniques)):
                            if techniques[j] in database[i]['techniques_used']:
                                if not database[i] in returnList:
                                    returnList.append(database[i])
        log("Search for " + str(search) + " with technique restrictions '" + ', '.join(techniques) + "' and field restrictions '" + ', '.join(search_fields) +  "' successful")
        return returnList

def sort_search_list(searchList, sort_by, sort_order):
    """Sorts search list"""
    sortList = []
    returnList = []
    log("Trying to sort search list")
    for i in range(len(searchList)):
        sortList.append(searchList[i][sort_by])
        returnList.append("")
    if (sort_order == "asc"):
        sortList.sort()
    else:
        sortList.sort(reverse=True)
    for i in range(len(searchList)):
        index = sortList.index(searchList[i][sort_by])
        while not returnList[index] == "": #In case some values are the same, increase index by one until that index is empty.
            index += 1
        returnList[index] = searchList[i]
    log("Sorted search list")
    return returnList

def log(string_to_log):
    from time import strftime
    date = strftime("%Y-%m-%d %H:%M:%S")
    txt_file = open("log.txt", "a")
    txt_file.write(date + " | " + string_to_log + "\n")
    txt_file.close()
