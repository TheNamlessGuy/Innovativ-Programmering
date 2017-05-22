# -*- coding: utf-8 -*-
require './parser.rb'
require './nonlanguagethings.rb'

class ETR

  def initialize
    @@classes = {}
    @@vars = {}
    @@funcs = {}
    @@maincontent = []
    #@@includes = []
    @@includesdone = []
    @@ifstmts = []
    @@whilestmts = []
    @@defaultclasses = {}
    @@defaultfuncs = {}
    @@returntype = "Integer"

    @@returnstmt = false

    # Match everything between [ and ] that is not ],
    # with a possibility of something like "a,"
    # and "a"
    @@arrayRegex = /\[[[^\]],]*[^\]]*\]/


###############################################################################
#
# READER STARTS HERE
#
###############################################################################
    @reader = Parser.new("EasyToRead") do

      everythingTokenRegex = /[^\s({}),;"]+/
      
      # Comments, throw. Match everything between // and // that is not //
      token(/\/\/[^\/\/]+\/\//)

      # Special symbols
      token(/(\(|\))/) {|a| a } # Match ( and )
      token(/({|})/) {|a| a } # Match { and }
      token(/,/) {|a| a }
      token(/;/) {|a| a }
      token(/"[^"]*"/) {|a| a } # Match everything between " and " that is not "
      token(/\[[[^\]],]*[^\]]*\]/) {|a| a }# Match all arrays
      token(/\s+/)

      token(everythingTokenRegex) {|a| a } # Everything else

      start :program do
        match(:contents)
      end
      
      rule :contents do
        match(:contents, :content) {|a, b| [a, b]}
        match(:content)
      end

      def check_if_var_exists(var)
        # Checks if the parameter is not a constant (therefore a variable),
        # and in that case checks if it has been initiated, thus preventing
        # variable use before definition.
        if (!@@vars.has_key?(var) && !var.is_boolean? && !var.is_digit? && !var.is_string?)
          if (var.is_array?)
            var = var[1..-2]
            var.split(",").each do |elem|
              check_if_var_exists(elem.strip)
            end
          else
            raise "Variable \"#{var}\" used before initiation"
          end
        end
      end

      rule :content do
        # Content is everything the program can contain
=begin
        match(:includes) {|a|
          # Saves all the names of the files the user has included to an array
          @@includes = [@@includes, a].flatten!
          a }
=end
        match(:classdefs)
        match(:funcdefs) {|a|
          # Saves all function definitions to the @@funcs variable.
          # If the match matches more than one function definition,
          # it loops through the resulting array
          if (a.class == Array)
            a.flatten!
            a.each do |entry|
              @@funcs[entry.general_name] = entry
            end
          else
            @@funcs[a.general_name] = a
          end
          a }
        match(:stmts) {|a|
          # Statements are either variable definitions,
          # of "main content" (function calls, mathematical expressions,
          # print statements, etc). Also prevents that a variable is declared
          # more than once (in a scope).
          # Variables are saved in the Hash "@@vars", and
          # "main content" in an Array by the same name
          if (a.class == Array)
            a.flatten!
            a.each do |entry|
              if (entry.class == VarHolder)
                if (@@vars.has_key?(entry.name))
                  raise "The variable '#{entry.name}' is declared more than once"
                else
                  @@vars[entry.name] = entry
                end
              else
                get_vars_from_func_call(entry).each do |var|
                  check_if_var_exists(var.strip)
                end
                @@maincontent << entry
              end
            end
          elsif (a.class == VarHolder)
            raise "The variable '#{a.name}' is declared more than once" if (@@vars.has_key?(a.name))
            @@vars[a.name] = a
          else
            get_vars_from_func_call(a).each do |var|
              check_if_var_exists(var.strip)
            end
            
            @@maincontent << a
          end
          a }
      end
      
      rule :classdefs do
        # Catches all class definitions
        match(:classdefs, :classdef) {|a, b| [a, b]}
        match(:classdef)
      end

      def classdeffunc(name, inherit, cont)
        # Since we needed so many different class definitions,
        # we decided to make the class definition into a function.
        ch = ClassHolder.new
        # Function checks if the class has been defined already,
        # and updates that class in that case.
        if (@@classes.has_key?(name))
          ch = @@classes[name]
        elsif (@@defaultclasses.has_key?(name))
          ch = @@defaultclasses[name]
        end
        ch.name = name
        ch.inherits << inherit
        ch.inherits.flatten!
        
        # Goes through all the inherits and adds all the functions to the
        # current class' member function Hash, with the key being the functions
        # general name, and the value being the name of the class it can be
        # found in. This is to prevent a lot of looping when a call is made.
        ch.inherits.each do |inherit_name|
          if (!@@classes.has_key?(inherit_name))
            raise "The inherit #{inherit_name} is being inherited before the class is defined!"
          end
          
          @@classes[inherit_name].memfuncs.each do |key, val|
            ch.memfuncs[key] = inherit_name
          end
        end
        
        # Loops through all the content in the class, and adds all the variables
        # to the variable "memvars", and all the
        # functions to the variable "memfuncs".
        cont.flatten!
        cont.each do |entry|
          if (entry.class == VarHolder)
            if (ch.memvars.has_key?(entry.name))
              raise "The variable '#{entry.name}' is declared more than once in the class '#{name}'"
            else
              ch.memvars[entry.name] = entry
            end
          elsif (entry.class == FuncHolder)
            ch.memfuncs[entry.general_name] = entry
          end
        end

        # Adds the variable "__class__" to every class,
        # containing the class' name, and the function
        # to get the variable.
        ch.memvars["__class__"] = VarHolder.new("__class__", "String", name)
        func = FuncHolder.new("$n.class", [], ["return '__class__'"])
        ch.memfuncs[func.general_name] = func

        # Sets the return type for all the member functions
        ch.memfuncs.each do |name, func|
          # Skip if the function is in another class
          next if (func.class == String)
          
          # Gets the name (or the constant) of the first return value,
          # removes any "trash" symbols and then skips if the name is empty.
          return_var = func.get_first_return_var(@@ifstmts, @@whilestmts)
          return_var = return_var[1..-2] if (return_var[0] =~ /(')/ && return_var[-1] =~ /(')/)
          next if (return_var.empty?)

          # Sets "returntype" if the variable is one
          # of the member variables, or a constant
          func.returntype = return_var.get_return_type(ch.memvars)

          #if (ch.memvars.has_key?(return_var))
          #  func.returntype = ch.memvars[return_var].type
          #else
          # If "returntype" still is empty, check if the
          # return value is defined in the function.
          if (func.returntype == nil || func.returntype.empty?)
            func.parameters.each do |param|
              func.returntype = param.type if (param.name == return_var)
            end
            if (func.returntype == "")
              func.contents.each do |elem|
                func.returntype = elem.type if (elem.class == VarHolder && elem.name == return_var)
              end
            end
          end
        end

        @@classes[ch.name] = ch
      end

      rule :classdef do
        # Matches classes with inheritance but no content,
        # no inheritance or content,
        # inheritance and content,
        # or no inheritance and content.
        match('define', :NAME, 'as', 'class', :inheritance, '{', '}') {|_, name, _, _, inherit, _, cont, _|
          classdeffunc(name, inherit, [])
          [name, inherit, cont] }
        
        match('define', :NAME, 'as', 'class', '{', '}') {|_, name, _, _, _, cont, _|
          classdeffunc(name, [], [])
          [name, cont] }
        match('define', :NAME, 'as', 'class', :inheritance, '{', :classcont, '}') {|_, name, _, _, inherit, _, cont, _|
          classdeffunc(name, inherit, cont)
          [name, inherit, cont] }
        
        match('define', :NAME, 'as', 'class', '{', :classcont, '}') {|_, name, _, _, _, cont, _|
          classdeffunc(name, [], cont)
          [name, cont] }
      end

      rule :classcont do
        # A class can contain member function definitions
        # and member variable definitions
        match(:memfuncdefs, :classcont) {|a, b| [a, b]}
        match(:memvardefs, :classcont) {|a, b| [a, b]}
        match(:memfuncdefs)
        match(:memvardefs)
      end

      rule :memfuncdefs do
        # Captures all the member functions
        match(:memfuncdefs, :memfuncdef) {|a, b|
          if (a.class == Array)
            a = (a << b).flatten!
            a
          else
            [a, b]
          end }
        match(:memfuncdef)
      end

      rule :memfuncdef do
        # A member function name has to include
        # the symbol "$n", so if it doesn't, it
        # gets added.
        match(:funcdef) {|a|
          if (!a.name.include?("$n"))
            a.name += "_from_$n"
          end

          [a] }
      end

      rule :memvardefs do
        # Captures all member variables
        match(:memvardefs, :memvardef) {|a, b| [a, b].flatten! }
        match(:memvardef)
      end

      rule :memvardef do
        # Captures all member variable defintions one by one
        match(:vardef, :SEP) {|a, _| [a] }
      end

      rule :funcdefs do
        # Captures all functions (outside of classes)
        match(:funcdefs, :funcdef) {|a, b| [a, b]}
        match(:funcdef)
      end

      rule :funcdef do
        # Matches function definitions with and without parameters.
        match('define', :NAME, 'as', 'function', 'with', 'parameters', '(', :parameters, ')', :codeblock) {|_, name, _, _, _, _, _, param, _, cont|
          fh = FuncHolder.new
          fh.parameters = param
          fh.contents = cont
          
          # Since all parameters have to be in the function name
          # if they aren't there, it adds them.
          (1..param.length).each do |i|
            if (!name.include?("$#{i}"))
              name += "_$#{i}"
            end
          end
          fh.name = name
          
          # Tries to set the return type
          return_var = fh.get_first_return_var(@@ifstmts, @@whilestmts)
          fh.returntype = return_var.get_return_type(@@vars)
          
          # If still not set, checks parameters and content
          # for the returned variable definition.
          if (fh.returntype == nil)
            param.each do |parameter|
              fh.returntype = parameter.type if (parameter.name == return_var[1..-2])
            end
            
            if (fh.returntype == nil)
              cont.each do |elem|
                fh.returntype = elem.type if (elem.class == VarHolder && elem.name == return_var[1..-2])
              end
            end
          end
          
          fh }
        match('define', :NAME, 'as', 'function', :codeblock) {|_, name, _, _, cont|
          fh = FuncHolder.new
          fh.name = name
          fh.contents = cont

          # Tries to set the return type
          return_var = fh.get_first_return_var(@@ifstmts, @@whilestmts)
          fh.returntype = return_var.get_return_type(@@vars)

          # If still not set, checks the content
          # for the returned variable definition
          if (fh.returntype == nil)
            cont.each do |elem|
              fh.returntype = elem.type if (elem.class == VarHolder && elem.name == return_var[1..-2])
            end
          end

          fh }
      end

      rule :parameters do
        # For example: define a as Integer, define b as Boolean
        match(:vardef, ',', :parameters) {|a, _, b| [a, b].flatten! }
        match(:vardef) {|a| [a] }
      end

      rule :codeblock do
        # Matches codeblocks with and without contents
        match('{', :stmts, '}') {|_, a, _|
          if (a.class == Array)
            a
          else
            [a]
          end }

        match('{', '}') {|_, _| []}
      end

      rule :stmts do
        # Captures all statements
        match(:stmts, :stmt) {|a, b|
          if (a.class == Array)
            a << b
            a
          else
            [a, b]
          end }
        match(:stmt)
      end

      rule :stmt do
        match(:vardef, :SEP) {|a, _| a } # Variable definitions
        match(:ifstmt) # If statements
        match(:whilestmt) # While statements
        match(:setstmt) # Set statements (a = 5)
        match(:equalitystmt, :SEP) {|a, _| a } # Equality statements (a == b, a < b)
        match(:mathstmt, :SEP) {|a, _| a } # Math statements
        match(:logicstmt, :SEP) {|a, _| a } # Logic statements
        match(:printstmt) {|a, _| a } # Print statments
        match(:returnstmt) {|a, _| a } # Return statements
        match(:func_call) # Function calls, matched last since it matches everything.
      end

      rule :mathstmt do
        # Matches all math statements, digits and variables
        match(:mathstmt, '+', :mathstmt) {|a, _, b| "#{a} + #{b}" }
        match(:mathstmt, '-', :mathstmt) {|a, _, b| "#{a} - #{b}" }
        match(:mathstmt, '*', :mathstmt) {|a, _, b| "#{a} * #{b}" }
        match(:mathstmt, '/', :mathstmt) {|a, _, b| "#{a} / #{b}" }
        match(:mathstmt, '%', :mathstmt) {|a, _, b| "#{a} % #{b}" }
        match(:mathstmt, '**', :mathstmt) {|a, _, b| "#{a} ** #{b}" }
        match('(', :mathstmt, ')') {|_, a, _| "( #{a} )" }
        match(/\d+/)
        match(/\d+\.\d+/)
        match(/'\w+'/)
      end

      rule :logicstmt do
        # Matches all logic statements, true, false and variables 
        match(:logicstmt, '&&', :logicstmt) {|a, _, b| "#{a} && #{b}" }
        match(:logicstmt, '||', :logicstmt) {|a, _, b| "#{a} || #{b}" }
        match('!', :logicstmt) {|_, a| "! #{a}" }
        match('(', :logicstmt, ')') {|_, a, _| "( #{a} )" }
        match('true')
        match('false')
        match(/'\w+'/)
      end

      rule :equalitystmt do
        # Matches all equality statements, variables,
        # default classes, math expressions and logic expressions
        match(:equalitystmt, '<', :equalitystmt) {|a, _, b| "#{a} < #{b}" }
        match(:equalitystmt, '>', :equalitystmt) {|a, _, b| "#{a} > #{b}" }
        match(:equalitystmt, '==', :equalitystmt) {|a, _, b| "#{a} == #{b}" }
        match('(', :equalitystmt, ')') {|_, a, _| "( #{a} )" }
        match(:DEFAULT_CLASS)
        match(/'\w+'/)
        match(:mathstmt)
        match(:logicstmt)
      end

      rule :DEFAULT_CLASS do
        # Matches all default class constants
        match(@@arrayRegex) {|a|
          if (a.is_array?)
            a
          else
            nil
          end }
        match(/"[^"]*"/) {|a|
          if (a.is_string?)
            a
          else
            nil
          end }
        match(:mathstmt)
        match(:logicstmt)
      end

      rule :setstmt do
        # Matches all set statements for constants,
        # variables (through DEFAULT_CLASS) and function calls
        match(/:\w+:/, '=', :DEFAULT_CLASS, :SEP) {|a, _, b, _|
          if (b.is_array?)
            "#{a} = #{b.gsub(" ", "")}"
          else
            "#{a} = #{b}"
          end }
        match(/:\w+:/, '=', :func_call) {|a, _, b| "#{a} = #{b}" }
      end
      
      rule :printstmt do
        # Matches all print statements for default class constants,
        # variables, equality statements and function calls
        match('print', :DEFAULT_CLASS, :SEP) {|_, a|
          if (a.is_array?)
            "print #{a.gsub(" ", "")}"
          else
            "print #{a}"
          end }
        match('print', :equalitystmt, :SEP) {|_, a, _| "print #{a}" }
        match('print', /'\w+'/, :SEP) {|_, a| "print #{a}" }
        match('print', :func_call) {|_, a| "print #{a}" }
      end

      rule :returnstmt do
        # Matches all return statements for default class constants,
        # variables, equality statements and function calls
        match('return', :DEFAULT_CLASS, :SEP) {|_, a|
          if (a.is_array?)
            "return #{a.gsub(" ", "")}"
          else
            "return #{a}"
          end }
        match('return', :equalitystmt, :SEP) {|_, a, _| "return #{a}" }
        match('return', /'\w+'/, :SEP) {|_, a| "return #{a}" }
        match('return', :func_call) {|_, a| "return #{a}" }
      end

      rule :ifstmt do
        # Matches simple if statements and if-otherwise statements
        match('if', '(', :expr, ')', :codeblock, 'otherwise', :codeblock) {|_, _, expr, _, cont, _, othercont|
          ih = IfHolder.new(cont, expr, othercont)

          # Saves the index the if statement will get
          index = @@ifstmts.length
          
          # Loops through all the previously defined if statements,
          # and checks if one equals another, and saves it as that
          # if statement instead of a new entry.
          @@ifstmts.length.times do |i|
            if (ih.equals?(@@ifstmts[i]))
              index = i
              break
            end
          end

          # Adds the if statement to the array, and returns the if identifier
          @@ifstmts << ih if (index == @@ifstmts.length)
          "if #{index}" }
        
        match('if', '(', :expr, ')', :codeblock) {|_, _, expr, _, cont|
          ih = IfHolder.new(cont, expr)

          # Saves the index the if statement will get
          index = @@ifstmts.length

          # Loops through all the previously defined if statements,
          # and checks if one equals another, and saves it as that
          # if statement instead of a new entry.
          @@ifstmts.length.times do |i|
            if (ih.equals?(@@ifstmts[i]))
              index = i
              break
            end
          end

          # Adds the if statement to the array, and returns the if identifier
          @@ifstmts << ih if (index == @@ifstmts.length)
          "if #{index}" }
      end

      rule :whilestmt do
        match('while', '(', :expr, ')', :codeblock) {|_, _, expr, _, cont|
          wh = WhileHolder.new(cont, expr)

          # Saves the index the while statement will get
          index = @@whilestmts.length

          # Loops through all the previously defined while statements,
          # and checks if one equals another, and saves it as that
          # while statement instead of a new entry.
          @@whilestmts.length.times do |i|
            if (wh.equals?(@@whilestmts[i]))
              index = i
              break
            end
          end
          
          # Adds the while statement to the array,
          # and returns the while identifier
          @@whilestmts << wh if (index == @@whilestmts.length)
          "while #{index}" }
      end

      rule :expr do
        # An expression is handled like a function call
        match(:equalitystmt)
        match(/[^)]+/, :expr) {|a, b|
          a += "_" + b
          a }
        match(/[^)]+/)
      end

      rule :vardef do
        # For example: define a as Integer
        match('define', :NAME, 'as', :NAME) {|_, name, _, cname|
          vh = VarHolder.new
          vh.name = name
          vh.type = cname

          # Check if the class you are defining your variable as
          # is defined, and then sets the variables value
          # to a Hash of the member variables of that class
          if (!@@defaultclasses.empty? && cname != "function" && cname != "class")
            if (@@classes.has_key?(cname))
              vh.value = {}
              @@classes[cname].memvars.each do |key, val|
                vh.value[key] = val.dup unless (key == "__class__")
              end
            elsif (@@defaultclasses.has_key?(cname))
              vh.value = {}
              @@defaultclasses[cname].memvars.each do |key, val|
                vh.value[key] = val.dup unless (key == "__class__")
              end
            else
              raise "The classname #{cname} is used before definition or not defined at all!"
            end
          end
          vh }
      end

      rule :inheritance do
        # Matches all inheritances
        match('<', :cnamelist) {|_, a|
          if (a.class == Array)
            a
          else
            [a]
          end}
      end

      rule :cnamelist do
        # Matches all NAMEs in a list (separated by ",")
        match(:cnamelist, ',', :NAME) {|a, _, b|
          if (a.class == Array)
            a << b
            a
          else
            [a, b]
          end }
        match(:NAME)
      end

      rule :func_call do
        # A function call matches anything up until a SEP.
        # Combines words with "_" and
        # removes all whitespaces.
        match(/[^;{}]+/, :func_call) {|a, b|
          a += "_" + b
          a.gsub(" ", "\\s") }
        match(/[^;{}]+/, :SEP) {|a, _| a }
      end

      rule :NAME do
        # Matches everything you would imagine you
        # can call a variable/function/class
        match(everythingTokenRegex)
      end

      rule :PATH do
        # For example: "myfile.etr"
        match(/\".+\.etr\"/)
      end

      rule :SEP do
        # This is its own rule for easy changing of the separator.
        match(';')
      end
      
    end

###############################################################################
#
# PARSER STARTS HERE
#
###############################################################################
    @@parser = Parser.new("EasyToRead") do
      
      token(/;/) # Statement separator. Throw
      token(/\s+/) # Spaces. Throw
      token(/_/) # Extra underlines. Throw
      token(@@arrayRegex) {|m| m } # Array
      token(/"[^"]*"/) {|m| m } # Strings
      token(/\d+\.\d+/) {|m| m } # Floats
      token(/\d+/) {|m| m } # Integers
      token(/[()]/) {|m| m } # Parenthesis
      token(/(\*\*|\+|-|\/|\*|%)/) {|m| m } # Math signs
      token(/(true|false)/) {|m| m } # true / false
      token(/(==|<|>|=|!|&&|\|\|)/) {|m| m } # logic statements
      token(/(print|return|if|otherwise|while)/) {|m| m } # Builtin words
      token(/:\w+:<</) {|m| m } # Array append symbol

      token(/[^;\s]+/) {|m| m } # Everything else

      def set_array_content(array)
        # Sets the content of an array to the correct class
        # Numbers are set to Integers,
        # True/False are set to Booleans, etc.
        #
        # When you have an array in an array,
        # the function turns recursive.
        array.length.times do |i|
          next if (array[i].class != String)
          array[i] = array[i].strip
          if (array[i][0] == "'" && array[i][-1] == "'")
            array[i] = @@vars[array[i][1..-2]].get_lowest_self.value
          elsif (array[i].is_int?)
            array[i] = array[i].to_i
          elsif (array[i].is_float?)
            array[i] = array[i].to_f
          elsif (array[i].is_boolean?)
            array[i] = array[i].to_b
          elsif (array[i].is_string?)
            array[i] = array[i][1..-2]
          elsif (array[i].is_array?)
            array[i] = set_array_content(array[i])
          end
        end
        
        return array
      end

      start :program do
        match(:stmts)
      end
      
      rule :stmts do
        match(:stmts, :stmt) {|a, b| [a, b] }
        match(:stmt)
      end

      rule :stmt do
        match(:ifstmt)
        match(:whilestmt)
        match(:setstmt)
        match(:equalitystmt)
        match(:logicstmt)
        match(:mathstmt)
        match(:printstmt)
        match(:returnstmt)
        match(:arrayaddstmt)
        match(:func_call)
      end

      rule :arraygetstmt do
        # Get a value out of an array
        # For example: :array:[0] or :array:['a']
        
        match(/:\w+:\[\d+\]/) {|call|
          # Sets the "call" variable to the variable name
          # and the index the user requested
          call = call.split("[")
          call[0] = call[0][1..-2]
          call[1] = call[1][0..-2]

          # Gets the variable by name
          if (@@vars.has_key?(call[0]))
            var = @@vars[call[0]]
          else
            raise "The variable #{call[0]} does not exist"
          end

          # If the variable is not of the type array,
          # you can't get a value from it 
          if (!var.type == "Array")
            raise "The variable #{var.name} is not of type array, but is trying to be accessed as one"
          end

          # Checks so that you aren't trying to get a
          # value outside of the variables scope
          index = call[1].to_i
          if (index >= var.get_lowest_self.value.length)
            raise "The index #{index} is out of range from the variable #{var.name}" 
          end
          
          # Returns the value
          var.get_lowest_self.value[index] }
        match(/:\w+:\['\w+'\]/) {|call|
          # Sets the "call" variable to the variable name
          # and the variable name containing the index the user requested
          call = call.split("[")
          call[0] = call[0][1..-2]
          call[1] = call[1][0..-2]

          # Gets the variable by name
          if (@@vars.has_key?(call[0]))
            var = @@vars[call[0]]
          else
            raise "The variable #{call[0]} does not exist"
          end
          
          # If the variable is not of the type array,
          # you can't get a value from it. 
          if (!var.type == "Array")
            raise "The variable #{var.name} is not of type array, but is trying to be accessed as one"
          end

          # Checks so that you aren't trying to get a
          # value outside of the variables scope
          index = set_array_content([call[1]])[0]
          if (index >= var.get_lowest_self.value.length)
            raise "The index #{index} is out of range from the variable #{var.name}" 
          end
          
          # Returns the value
          var.get_lowest_self.value[index] }
      end

      rule :arrayaddstmt do
        # Add a constant, a variable or a value
        # from another array to this array
        match(/:\w+:<</, :DEFAULT_CLASS) {|call, toadd|
          # Set the "call" variable to the name of the array variable
          call = call.split(":")[1]
          
          # Gets the VarHolder object for the array variable.
          if (@@vars.has_key?(call))
            call = @@vars[call]
          else
            raise "The variable #{call} does not exist"
          end
          
          # Checks if the constant you are adding is an array,
          # and adds it specially. Otherwise adds it normally.
          if (toadd.class == String && toadd.is_array?)
            toadd = toadd[1..-2].gsub("_", "").split(",")
            call.get_lowest_self.value << set_array_content(toadd)
          else
            call.get_lowest_self.value << toadd
          end }
        match(/:\w+:<</, :GETVAR) {|call, toadd|
          # If the gotten variable is empty, the value can not be appended
          if (toadd == nil)
            raise "You are trying to get the value of a variable that has not yet been given a value!"
          end

          # Set the "call" variable to the name of the array variable
          call = call.split(":")[1]
          
          # Gets the VarHolder object for the array variable.
          if (@@vars.has_key?(call))
            call = @@vars[call]
          else
            raise "The variable #{call} does not exist"
          end
          
          # Adds the value to the array
          call.get_lowest_self.value << toadd }
        match(/:\w+:<</, :arraygetstmt) {|call, toadd|
          # Set the "call" variable to the name of the array variable
          call = call.split(":")[1]
          
          # Gets the VarHolder object for the array variable.
          if (@@vars.has_key?(call))
            call = @@vars[call]
          else
            raise "The variable #{call} does not exist"
          end
          
          # Adds the value to the array
          call.get_lowest_self.value << toadd }
      end

      rule :ifstmt do
        # Matches and executes if-statment
        match('if', :DIGIT) {|_, index|

          # Creates a scope
          var_backup = @@vars.dup
          
          return_value = index
          
          # Checks if statement's expression is true
          if (@@parser.parse(@@ifstmts[index].logicstmt))
            
            # Loops through all the content in the if statement.
            # If the content is a VarHolder, it adds that
            # variable to the @@vars Hash, otherwise it parses the line
            # If the line contains a return statement, the loop breaks
            @@ifstmts[index].content.each do |line|
              if @@returnstmt
                @@returnstmt = false
                break
              elsif (line.class == VarHolder)
                @@vars[line.name] = line
              else
                return_value = @@parser.parse(line)
              end
            end
            
          # Checks if the statements has any otherwise content
          elsif (!@@ifstmts[index].othercont.empty?)
            
            # Loops through all the content in the otherwise statement.
            # If the content is a VarHolder, it adds that
            # variable to the @@vars Hash, otherwise it parses the line
            # If the line contains a return statement, the loop breaks
            @@ifstmts[index].othercont.each do |line|
              if @@returnstmt
                @@returnstmt = false
                break
              elsif (line.class == VarHolder)
                @@vars[line.name] = line
              else
                return_value = @@parser.parse(line)
              end
            end
          end
          
          # Resets scope
          @@vars = var_backup
          return_value }
      end
      
      rule :whilestmt do
        # Mathes while statements
        match('while', :DIGIT) {|_, index|
          var_backup = @@vars

          return_value = index

          # Checks if the statement's expression is true
          while(@@parser.parse(@@whilestmts[index].logicstmt))

            # Loops through all the content in the while statement.
            # If the content is a VarHolder, it adds that
            # variable to the @@vars Hash, otherwise it parses the line
            # If the line contains a return statement, the loop breaks
            @@whilestmts[index].content.each do |line|
              if @@returnstmt
                @@returnstmt = false
                break
              elsif (line.class == VarHolder)
                @@vars[entry.name] = entry
              else
                return_value = @@parser.parse(line)
              end
            end
          end

          # Resets scope
          @@vars = var_backup
          return_value }
      end
      
      rule :setstmt do
        # Matches all instances where a variable gets a value
        match(:USEVAR, '=', :arraygetstmt) {|var, _, val|
          # Matches setting a variable to a value from an array
          var = @@vars[var[1..-2]]
          # Checks if the value is of the same class as the variable
          if (var.type == val.class.name ||
              (var.type == "Boolean" && !!val == val) ||
              (var.type == "Integer" && val.is_a?(Integer)))
            var.get_lowest_self.value = val
          else
            raise "The variable #{var.name} is being set to wrong type!\nError: :#{var.name}:=#{val}"
          end }

        match(:USEVAR, '=', @@arrayRegex) {|var, _, val|
          # Matches setting a variable to an array constant
          var = var[1..-2]
          val = val[1..-2]
          # Checks if the variable is an Array
          if (@@vars[var].type == "Array")
            # Sets the array's content to the correct type.
            # Sets the value to the new array and sets length
            # to the length of the new array
            array = set_array_content(val.split(","))
            @@vars[var].get_lowest_self.value = array
            if (@@vars[var] == @@vars[var].get_lowest_self)
              @@vars["length"].value = array.length
            else
              @@vars[var].value["length"].value = array.length
            end
          else
            raise "The variable #{var} is being set to wrong type!\nError: :#{var}:=[#{val}]"
          end }

        match(:USEVAR, '=', /"[^"]*"/) {|var, _, val|
          # Matches setting a variable to a string constant
          var = var[1..-2]
          val = val[1..-2]
          # Checks if the variable is a String
          if (@@vars[var].type == "String")
            @@vars[var].get_lowest_self.value = val
          else
            raise "The variable #{var} is being set to wrong type!\nError: :#{var}:=\"#{val}\""
          end }

        match(:USEVAR, '=', :mathstmt) {|var, _, val|
          # Matches setting a variable to the value of a math statement
          var = var[1..-2]
          
          # Checks if both variables are either Float or Integer,
          # the only possible types math statements can be
          if (val.class == Float && @@vars[var].type == "Float")
            @@vars[var].get_lowest_self.value = val
          elsif (val.is_a?(Integer) && @@vars[var].type == "Integer")
            @@vars[var].get_lowest_self.value = val
          elsif (val.class.name == @@vars[var].type || @@vars[var].type == "Boolean" && !!val == val)
            @@vars[var].get_lowest_self.value = val
          else
            raise "The variable #{var} is being set to wrong type!\nError: :#{var}:=#{val}"
          end }

        match(:USEVAR, '=', :logicstmt) {|var, _, val|
          # Matches setting a variable to the value of a logic statements
          var = var[1..-2]

          # Checks if the variable is a Boolean,
          # the only possible type logic statements can be
          if (@@vars[var].type == "Boolean")
            @@vars[var].get_lowest_self.value = val
          else
            raise "The variable #{var} is being set to wrong type!\nError: :#{var}:=#{val}"
          end }

        match(:USEVAR, '=', :GETVAR) {|var, _, val|
          # If the gotten variable is empty, the value can not be appended
          if (val.value == nil)
            raise "You are trying to get the value of a variable that has not yet been given a value!"
          end
          # Matches setting a variable to the value of another variable
          var = var[1..-2]
          # Checks if both variables are arrays and sets the length variable,
          # if that is the case
          if (val.type == "Array" && @@vars[var].type == "Array")
            if (@@vars[var].value.class != Hash)
              @@vars["length"].value = val.value.length
            else
              if (!@@vars[var].value.has_key?("length"))
                @@vars[var].value["length"] = VarHolder.new("length", "Integer")
              end
              @@vars[var].value["length"].value = val.value.length
            end
          end
          
          # Checks if both variables are of the same type
          if (val.type == @@vars[var].type)
            @@vars[var].get_lowest_self.value = val.value
          else
            nil
          end }

        match(:USEVAR, '=', :func_call) {|var, _, val|
          # Matches setting a variable to the return value of a function call
          var = var[1..-2]
          
          # Checks if the value is of the same class as the variable
          if (@@vars[var].type == val.class.name ||
              (@@vars[var].type == "Boolean" && !!val == val) ||
              (@@vars[var].type == "Integer" && val.is_a?(Integer)))
            @@vars[var].get_lowest_self.value = val 
          else
            raise "You are trying to set a variable (#{var}) to the wrong type!"
          end }
      end

      rule :mathstmt do
        # Math statement matches down through the different mathematical
        # expressions down to "mathexpr". This is to ensure the correct
        # priorities of the expressions
        match(:plusstmt)
      end

      rule :plusstmt do
        match(:plusstmt, '+', :minusstmt) {|e, _, t| e + t }
        match(:minusstmt)
      end

      rule :minusstmt do
        match(:minusstmt, '-', :multstmt) {|m, _, t| m - t } 
        match(:multstmt)
      end

      rule :multstmt do
        match(:multstmt, '*', :divstmt) {|t, _, q| t * q }
        match(:divstmt)
      end

      rule :divstmt do
        match(:divstmt, '/', :modstmt) {|a, _, b| a / b }
        match(:modstmt)
      end

      rule :modstmt do
        match(:modstmt, '%', :powstmt) {|a, _, b| a % b }
        match(:powstmt)
      end
      
      rule :powstmt do
        match(:powstmt, '**', :mathexpr) {|f, _, q| f ** q }
        match(:mathexpr)
      end

      rule :mathexpr do
        match('(', :mathstmt, ')') {|_, e, _| e }
        match(:GETVAR) {|a| 
          # If the gotten variable is empty, the value can not be appended
          if (a.value == nil)
            raise "You are trying to get the value of a variable that has not yet been given a value!"
          end
          a.value }
        match(:DIGIT)
      end

      rule :logicstmt do
        # Logic statement matches down through the different logical
        # expressions down to "logicexpr". This is to ensure the correct
        # priorities of the expressions
        match(:orstmt)
      end

      rule :orstmt do
        match(:orstmt, '||', :andstmt) {|a, _, b| a || b }
        match(:andstmt)
      end

      rule :andstmt do
        match(:andstmt, '&&', :notstmt) {|a, _, b| a && b }
        match(:notstmt)
      end
      
      rule :notstmt do
        match('!', :notstmt) {|_, a| !a }
        match(:logicexpr)
      end

      rule :logicexpr do
        match('(', :logicstmt, ')') {|_, l, _| l }
        match('true') { true }
        match('false') { false }
        match(:GETVAR) {|a| 
          # If the gotten variable is empty, the value can not be appended
          if (a.value == nil)
            raise "You are trying to get the value of a variable that has not yet been given a value!"
          end
          a.value }
      end
      
      rule :equalitystmt do
        # Equality statement is used for testing equalities,
        # such as ==, < and >
        match(:equalitystmt, '<', :equalitystmt) {|a, _, b| a < b }
        match(:equalitystmt, '>', :equalitystmt) {|a, _, b| a > b }
        match(:equalitystmt, '==', :equalitystmt) {|a, _, b| a == b }
        match('(', :equalitystmt, ')') {|_, a, _| a }
        match(:DEFAULT_CLASS) {|a|
          if (a.class == String && a.is_array?)
            set_array_content(a[1..-2].split(","))
          else
            a
          end }
        match(:GETVAR) {|a| 
          # If the gotten variable is empty, the value can not be appended
          if (a.value == nil)
            raise "You are trying to get the value of a variable that has not yet been given a value!"
          end
          a.value }
        match(:mathexpr)
        match(:logicexpr)
      end

      def print_array(array)
        # Prints the contents of an array. If the array contains another array,
        # the function prints it recursively
        print "["
        array.length.times do |i|
          entry = array[i]
          if (entry.class == Array)
            print_array(entry)
          elsif (entry.class == String)
            entry = '"' + entry unless (entry[0] == '"')
            entry = entry + '"' unless (entry[-1] == '"')
            print entry.gsub("\\s", " ")
          else
            print entry
          end
          print ", " unless (i == array.length - 1)
        end
        print "]"
      end

      rule :printstmt do
        # Matches printing a value from an array
        match('print', :arraygetstmt) {|_, a|
          # Checks if the value is a String or an Array
          # and prints it specially, otherwise normally
          if (a.class == String)
            print a.gsub("\\n", "\n").gsub("\\s", "\s");
          elsif (a.class == Array)
            print_array(a)
          else
            print a
          end
          a }

        match('print', :DEFAULT_CLASS) {|_, a|
          # Matches printing a default class constant
          if (a.class == String)
            # If a is a String, it can be either an Array or a String
            if (a.is_array?)
              a = a[1..-2].split(",")
              a = set_array_content(a)
              print_array(a)
            else
              print a.gsub("\\n", "\n").gsub("\\s", " ")
            end
          elsif (a.class == Array)
            print_array(a)
          else
            print a
          end
          a }

        match('print', :GETVAR) {|_, a|
          # If the gotten variable is empty, the value can not be appended
          if (a.value == nil)
            raise "You are trying to get the value of a variable that has not yet been given a value!"
          end
          # Matches printing the value of a variable
          a = a.get_lowest_self.value
          # Checks if the value is a String or an Array
          # and prints it specially, otherwise normally
          if (a.class == String)
            print a.gsub("\\n", "\n").gsub("\\s", " ")
          elsif (a.class == Array)
            print_array(a)
          else
            print a
          end
          a }

        # Matches printing the return value of a function call
        match('print', :func_call) {|_, a| print a; a }
      end

      rule :returnstmt do
        # Matches all return statements
        match('return', :DEFAULT_CLASS) {|_, a|
          # Return default class constant
          # Check if "a" is an array, and set content if that is the case
          # Check if the returntype is the correct type.
          @@returnstmt = true
          if (a.class == String && a.is_array?)
            a = set_array_content(a[1..-2].split(","))
          end

          if (@@returntype == a.class.name ||
              (@@returntype == "Boolean" && !!a == a) ||
              (@@returntype == "Integer" && a.is_a?(Integer)) ||
              (@@returntype == "ARRAYGETSTMT"))
            a
          else
            raise "Trying to return a variable (#{a}) that is not the correct class"
          end }

        match('return', :equalitystmt) {|_, a|
          # Return an equality statement
          # Check if "a" is the correct type (Boolean),
          # and that the returntype is set to Boolean
          @@returnstmt = true
          if (@@returntype == "Boolean" && !!a == a)
            a
          else
            raise "Trying to return a variable (#{a}) that is not the correct class"
          end }

        match('return', :arraygetstmt) {|_, a|
          # Return a value from an array
          # Check if the returntype is of the same type as "a"
          # If the returntype is "ARRAYGETSTMT", you can return anything
          @@returnstmt = true
          if (@@returntype == a.class.name ||
              (@@returntype == "Boolean" && !!a == a) ||
              (@@returntype == "Integer" && a.is_a?(Integer)) ||
              (@@returntype == "ARRAYGETSTMT"))
            a
          else
            raise "Trying to return a variable (#{a}) that is not the correct class"
          end }
        
        match('return', :func_call) {|_, a|
          # Return the return value from a function
          # Check if the returntype is of the same type as "a"
          # If the returntype is "ARRAYGETSTMT", you can return anything
          @@returnstmt = true
          if (@@returntype == a.class.name ||
              (@@returntype == "Boolean" && !!a == a) ||
              (@@returntype == "Integer" && a.is_a?(Integer)) ||
              (@@returntype == "ARRAYGETSTMT"))
            a
          else
            raise "Trying to return a variable (#{a}) that is not the correct class"
          end }
      end

      def memfunc_call(func_name)
        # This function is called if you make a function call containing two ":"
        var_name = get_chars_between_char(':', func_name)[0]
        if (var_name == nil)
          # If "var_name" is empty, it means we are trying
          # to access a member function from within another member function
          # For example: set :: to 5
          vars_backup = @@vars.dup
          
          return_val = func_call(func_name, true)
          
          @@vars = vars_backup
          return return_val
        end
        var = @@vars[var_name]

        # If the gotten variable is empty, the value can not be appended
        if (var.value == nil)
          raise "You are trying to get the value of a variable that has not yet been given a value!"
        end

        # Get the ClassHolder for the type of the variable
        class_holder = nil
        
        if (@@classes.has_key?(var.type))
          class_holder = @@classes[var.type].dup
        elsif (@@defaultclasses.has_key?(var.type))
          class_holder = @@defaultclasses[var.type].dup
        end

        # Create a new scope
        func_backup = @@funcs.dup
        var_backup = @@vars.dup
        @@funcs = @@funcs.merge(class_holder.memfuncs)
        @@vars = class_holder.memvars.dup

        if (var.value.class == Hash)
          var.value.each do |key, val|
            @@vars[key].value = val.value
          end
        end

        # Set all the parameters to the value they had before.
        get_chars_between_char("'", func_name).each do |var_name|
          @@vars[var_name] = var_backup[var_name].dup
        end

        # Run the function
        return_val = func_call(func_name, true)

        # Save the changed variables
        vars_changes = @@vars

        # Revert to the previous scope
        @@funcs = func_backup
        @@vars = var_backup

        # Set the value of the called upon variable to the correct value(s)
        if (@@vars[var.name].value == nil)
          @@vars[var.name].value = {}
          class_holder.memvars.each do |key, val|
            @@vars[var.name].value[key] = val.dup
          end
        elsif (@@vars[var.name].value.class == Hash)
          @@vars[var.name].value.each do |key, val|
            @@vars[var.name].value[key] = vars_changes[key].dup
          end
        end

        return return_val
      end
      
      def func_call(func_name, memfunc_call=false)
        # This function is called whenever a function call is made.
        # Start by getting all the possible function names
        func = func_name.get_possible_function_name
        real_func_name = nil
        # Loop through the names, and try to find the correct one
        func.each do |name|
          if @@funcs.has_key?(name)
            func = @@funcs[name]
            real_func_name = name
            break
          elsif @@defaultfuncs.has_key?(name)
            func = @@defaultfuncs[name]
            real_func_name = name
            break
          end
        end
        
        # If no function was found, the variable func will be of type Array
        if (func.class == Array)
          raise "The function call for #{func_name} was not found!"
        end

        # If the function does not exist in the current class,
        # "func" will be of type string, containing the name
        # of the class which will have the function
        class_name = func
        while (func.class == String)
          if (@@classes.has_key?(func))
            class_name = func
            func = @@classes[func].memfuncs[real_func_name].dup
          elsif (@@defaultclasses.has_key?(func))
            class_name = func
            func = @@defaultclasses[func].memfuncs[real_func_name].dup
          end
        end
        
        # Now that the function is found, get all the variables
        # from the function call, and loop through them
        # to set the parameters to the correct values

        var_array = func_name.get_vars_in_order(func.general_name)
        var_array.length.times do |i|
          # If the value of the parameter hasn't been set,
          # set it to the value it should have.
          if (func.parameters[i].value == nil)
            func.parameters[i].value = {}
            if (@@classes.has_key?(func.parameters[i].type))
              @@classes[func.parameters[i].type].memvars.each do |key, val|
                func.parameters[i].value[key] = val.dup
              end
            elsif (@@defaultclasses.has_key?(func.parameters[i].type))
              @@defaultclasses[func.parameters[i].type].memvars.each do |key, val|
                func.parameters[i].value[key] = val.dup
              end
            end
          end
          var = var_array[i]
          # If "var" is a variable
          if ((var[0] == "'" && var[-1] == "'") || (var[0] == ":" && var[-1] == ":"))
            var = var[1..-2]
            # If "var" is the user trying to get a value from an array
            # For example: function ':array:[0]';
            if (var.include?("[") && var.include?("]"))
              # Get the array's name and the index
              split = var.split("[")
              split[0] = split[0][1..-2]
              split[1] = split[1][0..-2]
              # Get the value of that index in the array variable
              val = @@vars[split[0]].get_lowest_self.value[split[1].to_i]
              # Check if the parameter type and the variable
              # type are of the same type, and add the variable in that case
              if (func.parameters[i].type == "String" && val.class == String)
                func.parameters[i].get_lowest_self.value = val
              elsif (func.parameters[i].type == "Integer" && val.is_a?(Integer))
                func.parameters[i].get_lowest_self.value = val
              elsif (func.parameters[i].type == "Boolean" && !!val == val)
                func.parameters[i].get_lowest_self.value = val
              elsif (func.parameters[i].type == "Float" && val.class == Float)
                func.parameters[i].get_lowest_self.value = val
              elsif (func.parameters[i].type == "Array" && val.class == Array)
                func.parameters[i].get_lowest_self.value = val
                if (func.parameters[i].value.class == Hash)
                  func.parameters[i].value["length"] = VarHolder.new("length", "Integer")
                  func.parameters[i].value["length"].value = val.length
                end
              else
                raise "The parameter '#{var}' is not of the correct type!"
              end
            # Check that the variable is of the same type as the parameter
            elsif (func.parameters[i].type == @@vars[var].get_lowest_self.type)
              func.parameters[i].get_lowest_self.value = @@vars[var].get_lowest_self.value
            else
              raise "The parameter '#{var}' is not of the correct type!"
            end
          # "var" is an array
          elsif (var.is_array?)
            if (func.parameters[i].type == "Array")
              var = var[1..-2].gsub("_", "").gsub("\\s", " ")
              func.parameters[i].get_lowest_self.value = set_array_content(var.split(","))
              if (func.parameters[i].value.class == Hash)
                func.parameters[i].value["length"] = VarHolder.new("length", "Integer")
                func.parameters[i].value["length"].value = func.parameters[i].get_lowest_self.value.length
              end
            else
              raise "The parameter '#{var}' is not of the correct type!"
            end
          # "var" is an int
          elsif (var.is_int?)
            if (func.parameters[i].type == "Integer")
              func.parameters[i].get_lowest_self.value = var.to_i
            else
              raise "The parameter '#{var}' is not of the correct type!"
            end
          # "var" is a float
          elsif (var.is_float?)
            if (func.parameters[i].type == "Float")
              func.parameters[i].get_lowest_self.value = var.to_f
            else
              raise "The parameter '#{var}' is not of the correct type!"
            end
          # "var" is a string
          elsif (var.is_string?)
            if (func.parameters[i].type == "String")
              func.parameters[i].get_lowest_self.value = var[1..-2].gsub("\\s", " ")
            else
              raise "The parameter '#{var}' is not of the correct type!"
            end
          # "var" is a boolean
          elsif (var.is_boolean?)
            if (func.parameters[i].type == "Boolean")
              func.parameters[i].get_lowest_self.value = var.to_b
            else
              raise "The parameter '#{var}' is not of the correct type!"
            end
          end
        end

        if (@@vars["self"].class == VarHolder &&
            @@vars["self"].type == "Array" &&
            @@vars["self"].value != nil &&
            @@vars["length"].class == VarHolder &&
            @@vars["length"].value == nil)
          @@vars["length"].value = @@vars["self"].value.length
        end
        
        # Create a new scope
        vars_backup = @@vars.dup
        # Member functions should be able to use
        # member variables, but functions should not
        # be able to use variables outside its scope
        @@vars = {} unless memfunc_call

        # Add all the parameters to the scope
        func.parameters.each do |param|
          @@vars[param.name] = param
        end
        
        # If the function was inherited, get all the
        # variables from that class and put it in the scope
        if (class_name.class == String)
          if (@@classes.has_key?(class_name))
            @@classes[class_name].memvars.each do |key, val|
              @@vars[key] = val
            end
          else
            @@defaultclasses[class_name].memvars.each do |key, val|
              @@vars[key] = val
            end
          end
        end
        
        # Backup the previous return type
        returntype_backup = @@returntype
        @@returntype = func.returntype

        return_val = nil
        # Parse the contents of the function
        func.contents.each do |entry|
          if @@returnstmt
            break
          elsif (entry.class == VarHolder)
            @@vars[entry.name] = entry
          else
            return_val = @@parser.parse(entry)
          end
        end
        
        # Restore the previous scope
        @@returntype = returntype_backup

        @@returnstmt = false
        
        @@vars = vars_backup
        
        return return_val
      end

      rule :func_call do
        # Matches all function calls.
        # If it contains two ":", it is a member function call
        # If it does not, it is a normal function call
        match(/[^;{}]+/) {|a|
          if (a =~ /:\w+:/ || a =~ /::/) # Member function call
            memfunc_call(a)
          else # Normal function call
            func_call(a)
          end }
      end
      
      rule :DEFAULT_CLASS do
        # Matches all default classes:
        #   Array
        #   String
        #   Integer/Float
        #   Boolean
        match(@@arrayRegex) {|a| #Array
          if (a.is_array?)
            a
          else
            nil
          end }
        match(/"[^"]*"/) {|a|
          if (a.is_string?)
              a[1..-2]
          else
            nil
          end } # String
        match(:mathstmt) # All mathematicals
        match(:logicstmt) # All boolean logic
      end
      
      rule :GETVAR do
        # Matches the getting of the value of a variable.
        # Checks if the variable exists, and returns the VarHolder.
        match(/'\w+'/) {|a|
          a = a[1..-2]
          if @@vars.has_key?(a)
            var = @@vars[a].get_lowest_self
            if (var.value == nil)
              raise "You are trying to get the value of a variable that has not yet been given a value!"
            else
              var
            end
          else
            nil
          end}
      end

      rule :USEVAR do
        # Matches the using of a variable.
        match(/:\w+:/)
      end

      rule :DIGIT do
        # Matches integers and floats
        match(/\d+\.\d+/) {|float|
          if (float.is_float?)
            float.to_f
          else
            nil
          end }
        match(/\d+/) {|integer| 
          if (integer.is_int?)
            integer.to_i
          else
            nil
          end }
      end
      
    end
  end

  def read_file(fileloc)
    # Make sure that no file is read more than once
    @@includesdone << fileloc
    # Read the file
    file_to_superstring(fileloc).each do |line|
      if (line =~ /use \".+\.etr\";/)
        line = line[/\".+\.etr\"/][1..-2]
        read_file(line) unless (@@includesdone.include?(line))
      else
        @reader.parse(line)
      end
    end

    # All default classes and functions should be accessible from
    # any scope, so they get their own variables.
    if (fileloc == "__DEFAULTTHINGS__.etr")
      @@defaultfuncs = @@funcs
      @@funcs = {}
      
      @@defaultclasses = @@classes
      @@classes = {}
    end
  end

  def file_to_superstring(fileloc)
    # Takes a file location, adds all the
    # lines to one long string besides
    # include statements, which are set
    # separately
    return_val = [""]
    File.open(fileloc).each_line do |line|
      if (line =~ /use \".+\.etr\";/)
        return_val << line
        return_val << ""
      else
        line.delete!("\n")
        return_val[return_val.length-1] += line
      end
    end
    return return_val
  end

  def parse(fileloc)
    # Parses the file by first reading it
    # and then parsing it
    read_file(fileloc)

    # If you are in debug-mode, print out all the saved data
    if (ARGV.length > 1)
      puts "******** CLASSES ***********"
      @@defaultclasses.each do |key, val|
        print_class(val)
        puts
      end
      @@classes.each do |key, val|
        print_class(val)
        puts
      end
      puts "******** MAIN FUNCTIONS ***********"
      @@defaultfuncs.each do |key, val|
        print_func(val)
        puts
      end
      @@funcs.each do |key, val|
        print_func(val)
        puts
      end

      puts "******** MAIN VARIABLES ***********"
      @@vars.each do |key, val|
        print_var(val)
        puts
      end
      
      puts "******** READ FILES ***********"
      @@includesdone.each do |include|
        puts include
      end

      puts "******** MAIN CONTENT ***********"
      @@maincontent.each do |entry|
        puts entry
      end
      
      puts "******** IFS ***********"
      @@ifstmts.length.times do |i|
        print "#{i} => "
        print [@@ifstmts[i]]
        puts
      end

      puts "******** WHILES ***********"
      @@whilestmts.length.times do |i|
        print "#{i} => "
        print [@@whilestmts[i]]
        puts
      end

      puts "\n\n\n"
      puts "******** INTERPRET ***********"
    end
    returnval = ""
    @@maincontent.each do |line|
      if @@returnstmt
        return returnval
      else
        returnval = @@parser.parse(line)
      end
    end
  end

  def log(state = true)
    # Sets the state of the loggers
    if (state)
      @reader.logger.level = Logger::DEBUG
      @@parser.logger.level = Logger::DEBUG
    else
      @reader.logger.level = Logger::WARN
      @@parser.logger.level = Logger::WARN
    end
  end

  def print_class(ch)
    # Prints a ClassHolder in a coherent way
    puts "NAME: "
    puts ch.name
    puts "MEMFUNCS: "
    ch.memfuncs.each do |key, val|
      print key
      puts
      if (val.class == String)
        puts "  can be found in #{val}"
        next
      end
      print "  PARAM: "
      print val.parameters
      puts
      print "  CONT: "
      print val.contents
      puts
      print "  RET: "
      print val.returntype
      puts
    end
    puts
    puts "MEMVARS: "
    print ch.memvars
    puts
    puts "INHERITS: "
    print ch.inherits
    puts
  end

  def print_func(fh)
    # Prints a FuncHolder in a coherent way
    print fh.name
    print ":\n"
    print "  PARAMETERS: "
    print fh.parameters
    puts
    print "  CONTENT: "
    print fh.contents
    puts
    print "  RETURN TYPE: "
    print fh.returntype
    puts
  end

  def print_var(vh)
    # Prints a VarHolder in a coherent way
    print vh.name
    print ":\n"
    print "  CLASS: "
    puts vh.type
    print "  VALUE: "
    puts vh.value
  end
end

# Initialize the parser
etr = ETR.new
# If more than just the file name is inserted, enter debug mode
if (ARGV.length > 1)
  etr.log(true)
  $stdout.reopen(File.new("./DEBUG.txt", 'w'))
else
  etr.log(false)
end
# Check that the user inserted a filename, and that it is of the correct type
if (ARGV.length > 0)
  if (ARGV[0][-4..-1] != ".etr")
    raise "The file given is not of the correct type!"
  end
else
  raise "No file given"
end
returnval = nil

# Parse the file containing all the default content
etr.read_file("__DEFAULTTHINGS__.etr")
# Parse the file the user requested.
begin
  returnval = etr.parse("#{ARGV[0]}")
rescue Exception => ex
  puts ex.message
end

returnval
