#!/usr/bin/env python
# -*- coding: utf-8 -*-
def load(file_directory):
    """Loads a .JSON file, and returns the data"""
    import json
    log("LOAD: Trying to load datafile '" + str(file_directory) + "'")
    try: #If something doesn't work
        with open(file_directory) as json_file:
            data = json.load(json_file) #Try to load the files data in JSON format
            log("LOAD: Loaded datafile '" + str(file_directory) + "'")
            return data #Return all data
    except:
        log("LOAD: Failed to get datafile '" + str(file_directory) + "'")
        return None #If something goes wrong, return None

def get_project_count(database):
    """Retrieves the number of projects in the database"""
    log("PROJECT COUNT: Trying to get length of database")
    if isinstance(database, list): #If the database is a list (as it should be), return the length of the database
        log("PROJECT COUNT: Got length of database. Length was '" + str(len(database)) + "'")
        return len(database)
    else: #If it's not a list, or the length could not be gotten
        log("PROJECT COUNT: Failed to get length of database")
        return None

def get_project(database, project_id):
    """Returns the project with the specified id in the database"""
    log("GET PROJECT: Trying to get project '" + str(project_id) + "' from database")
    if isinstance (database, list): #If the database is a list
        if isinstance (project_id, int): #If the project_id is an int
            for i in range(len(database)):
                if (database[i]['project_no'] == project_id): #If the currently looping projects ID equals the ID the module recieved
                    log("GET PROJECT: Got project with id '" + str(project_id) + "'")
                    return database[i] #Return project
    log("GET PROJECT: Failed to get project with id '" + str(project_id) + "'")
    return None #If something went wrong, return None

def get_techniques(database):
    """Returns a list of techniques used in all projects"""
    try: #If there's some sort of fault in the database
        returnValue = []
        log("GET TECHNIQUES: Trying to get techniques from database")
        if isinstance (database, list):
            for i in range(len(database)): #Goes through all the projects in the database
                techniques_used = database[i]['techniques_used'] #Gets the techniques in the current project
                for j in range(len(techniques_used)):
                    #Checks if each technique gotten is in returnValue already 
                    if not techniques_used[j].lower() in returnValue:
                        returnValue.append(techniques_used[j].lower())
            returnValue.sort()
            log("GET TECHNIQUES: Got techniques used")
            return returnValue
    except:
        return None #If something goes wrong, return None
    log("GET TECHNIQUES: Failed to get techniques used")
    return None

def get_technique_stats(database):
    """Returns a dict with all the stats for all the techniques, organized after technique"""
    log("TECHNIQUE STATS: Trying to get technique stats")
    if (isinstance(database, list)):
        techniques_used = get_techniques(database) #Tries to get all the techniques through the module above
        if not techniques_used: #If you couldn't get the techniques used, return None
            return None
        returnDict = {}
        for i in range (len(techniques_used)): #Loop through all the techniques
            returnDict[techniques_used[i]] = get_projects_with_technique(database, techniques_used[i])
            #Adds all the stats for the projects used under the technique currently being checked in the loop into a dict with the technique as the key.
        returnDict = sort_technique_stats(returnDict, 'name')
        log("TECHNIQUE STATS: Got the technique stats")
        return returnDict
    log("TECHNIQUE STATS: Failed to get technique stats")
    return None

def sort_technique_stats(list_to_sort, sort_key):
    """Sorts the dict inputted"""
    log("TECHNIQUE STATS: Attempting to sort technique stats")
    from operator import itemgetter
    for key in list_to_sort.keys(): #For every key in the dict you put in
        list_to_sort[key] = sorted(list_to_sort[key], key = itemgetter(sort_key))
        #Get the value of the currently looping key, and sort it with the help of itemgetter
    log("TECHNIQUE STATS: Successfully sorted technique stats")
    return list_to_sort

def get_projects_with_technique(database, technique):
    """Returns a list which contains all the stats for the technique inputted"""
    log("TECHNIQUE STATS: Trying to get projects where '" + technique + "' was used")
    returnList = []
    for i in range(len(database)): #Loop through database
        for technique_used in database[i]['techniques_used']: #Get all the techniques in the currently looping element from the database
            if technique.lower() == technique_used.lower(): #If the techniques match (in lowercase, so all different spellings go through)
                returnList.append({'id': database[i]['project_no'], 'name': database[i]['project_name']})
                #Add the id and the name of the project which matched
    log("TECHNIQUE STATS: Got projects where '" + technique + "' was used")
    return returnList

def search(database, sort_by='start_date', sort_order='desc', techniques=None, search=None, search_fields=None):
    """Returns a sorted list matching the inputted criteria"""
    try: #If there is some sort of fault in the database, go to except
        returnValue = []
        search_is_none = False
        log("SEARCH: Trying to search for '" + str(search) + "'")
        if techniques is None or len(techniques) == 0:
            log("SEARCH: No techniques were provided")
        if search_fields is None or len(search_fields) == 0:
            log("SEARCH: No fields were provided")
        if search == None or search == '':
            search_is_none = True #If search is none, this becomes true
        else: #If search is not none, remove all the whitespaces before and after the searchword
            search = search.strip()
        for project in database: #Loop through all the projects
            for key in project.items(): #Get all the items of that project
                valueStr = str(key[1]) #Get the value as a string
                if search_is_none: #If search is none, it should match all cases, so set search equal to the value it is searching for
                    search = valueStr
                if search.lower() in valueStr.lower(): #If search is in valueStr
                    if not project in returnValue: #If the project is not already being returned
                        if technique_check(project, techniques): #If the technique criteria matches
                            if fields_check(key[0], search_fields): #If the fields criteria matches
                                returnValue.append(project)
        returnValue = sort_search_list(returnValue, sort_by, sort_order) #Sorts the searchlist
        if search_is_none:
            search = None
            #If search is none, change it back to none so the log prints the correct thing
        log("SEARCH: Search for '" + str(search) + "' successful")
        return returnValue
    except: #If something went wrong, return None
        return None

def technique_check(project, techniques):
    if techniques == None or len(techniques) == 0: #If techniques is empty, return True
        return True
    else:
        for technique in techniques:
            for used_technique in project['techniques_used']:
                if technique.lower() == used_technique.lower():
                    log("SEARCH: Technique check was successful with technique '" + technique + "'")
                    return True #If one of the techniques used is found within the project, return True
        log("SEARCH: Technique check failed")
        return False #If none of the techniques were found

def fields_check(key, search_fields):
    if search_fields == None or len(search_fields) == 0: #If search_fields is empty, return True
        return True
    else:
        if key in search_fields: #If the key you are checking is in search_fields, return True
            log("SEARCH: Fields check was successful with field '" + key + "'")
            return True
        log("SEARCH: Fields check failed")
        return False #If they key is not in search_fields, return False

def sort_search_list(searchList, sort_by, sort_order):
    """Sorts search list"""
    sortList = []
    returnList = []
    log("SEARCH: Trying to sort search list")
    for i in range(len(searchList)):
        #For every item in the searchList, add the value of the key 'sort_by' to sortList and extend the length of returnList
        sortList.append(searchList[i][sort_by])
        returnList.append("")
    if (sort_order == "asc"): #If ascending sort order
        sortList.sort()
    else: #If descending sort order
        sortList.sort(reverse=True)
    for i in range(len(searchList)):
        #For every item in the searchList, get the index that the value 'sort_by' has in the current loop, from the sortList list 
        index = sortList.index(searchList[i][sort_by])
        while not returnList[index] == "": #In case some values are the same, increase index by one until that index is empty.
            index += 1
        returnList[index] = searchList[i] #Add the currently looping searchList item to returnList at the specified index
    log("SEARCH: Sorted search list")
    return returnList

def log(string_to_log):
    from time import strftime
    date = strftime("%Y-%m-%d %H:%M:%S") #Write the date with the format: YEAR-MONTH-DAY HOUR:MINUTE:SECOND
    txt_file = open("log.txt", "a") #Open the textfile log.txt as txt_file
    txt_file.write(date + " | " + string_to_log + "\n")
    txt_file.close() #Close the txt_file
