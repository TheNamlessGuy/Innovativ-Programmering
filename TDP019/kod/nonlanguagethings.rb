# -*- coding: utf-8 -*-
def get_vars_from_func_call(call)
  # Returns list of all variables used in a function call
  array = []

  if (call.count_chars_in_row("'").max > 1 || call.count_chars_in_row(":").max > 1)
    raise "You can only include one variable in a function call.\nError: #{call}"
  end

  array = get_chars_between_char("'", call, array)
  array = get_chars_between_char(":", call, array)

  return array
end

def get_chars_between_char(char, str, array=[])
  # Returns list of characters between "char"
  while (str.count(char) >= 2)
    str = str[str.index(char)+1..-1]
    var = str[0..str.index(char)-1]
    # Checks if there are more characters to extract
    if (var.include?("'") || var.include?(":"))
      array << get_vars_from_func_call(var)
      array.flatten!
    else
      array << var if (!array.include?(var))
    end
    str = str[str.index(char)+1..-1]
  end
  return array
end

class Array
  def delete_first_of(char)
    # Removes the first instance of "char" from the array
    self.delete_at(self.index(char) || self.length)
  end
end

class String
  def to_b
    # Converts "self" to Boolean
    self == "true"
  end

  def is_digit?
    # Returns true if "self" is int or float
    (self.is_int? || self.is_float?)
  end

  def is_int?
    # Returns true if "self" is an Integer.
    # The function tries to convert "self" to an Integer,
    # if it's possible the function returns true.
    true if (Integer(self)) rescue false
  end

  def is_float?
    # Returns true if "self" is a Float.
    # The function tries to convert "self" to a Float,
    # if it's possible the function returns true.
    true if (Float(self)) rescue false
  end

  def is_boolean?
    # Returns true if "self" is "true" or "false"
    return true if (self == "true" || self == "false")
    return false
  end

  def is_string?
    # Returns true if both the first and the last character in "self" is '"'
    return true if (self[0] == '"' && self[-1] == '"')
    return false
  end

  def is_array?
    # Returns true if the first character in "self" is "[" and the last "]"
    return true if (self[0] == "[" && self[-1] == "]")
    return false
  end
  
  def is_math_expr?
    # Returns true if "self" contains any mathematical operators
    return true if (self =~ /(\*\*|\+|-|\/|\*|%)/)
    return false
  end

  def is_logic_expr?
    # Returns true if "self" contains any logical operators
    return true if (self =~ /(==|<|>|=|!|&&|\|\|)/)
    return false
  end

  def get_return_type(vars={})
    # Returns correct type of the string
    return "Array" if self.is_array?
    return "String" if self.is_string?
    return "Integer" if (self.is_int? || self.is_math_expr?)
    return "Float" if self.is_float?
    return "Boolean" if (self.is_boolean? || self.is_logic_expr?)
    return "ARRAYGETSTMT" if (self =~ /(:\w+:\['\w+'\]|:\w+:\[\d+\])/)
    return vars[self].type if (vars.has_key?(self))
    return nil
  end

  def is_default_class?
    # Returns true if the string is a default class
    return true if (self.is_boolean? || self.is_digit? || self.is_string? || self.is_array?)
    return false
  end

  def count_chars_in_row(char)
    # Returns list of the number of times "char" is found in a row in "self"
    return [0] if (!self.include?(char))
    
    array = []
    copy = self
    # Checks if the string has more instances of "char"
    while (copy.include?(char))
      count = 1
      index = copy.index(char)
      
      # Checks if the index is inside the length and
      # if the next character in the string matches "char"
      while (copy.length > index && copy[index+1] == char)
        count += 1
        index += 1
      end
      
      copy = copy[index+1..-1]
      array << count
    end
    
    return array
  end

  def calc_values(mod, length, name)
    # Returns an array containing whether or not
    # a number should be in the function name
    # for all possible values (depending on "length" and "mod")
    length = 2 ** length
    counter = 0
    parameter = false
    return_array = []

    while (counter < length)
      # For every mod, switch whether or not a number should be
      # itself or "$"
      mod.times do |i|
        if (parameter)
          return_array << name
        else
          return_array << "$"
        end
        counter += 1
      end
      parameter = !parameter
    end
    
    return return_array
  end

  def calc_all_values(array, str)
    # Makes use of the fact that the numbers being in the name follow a pattern
    # of doubling the window of being in the name or not.
    # For example:
    #     If we have a function name (func_$1_$2), a possible call could be:
    #     func 1 2. However, there could be a function named func_1_$1.
    #     So the fact that the number is in the name or not follows a pattern:
    #     1 : in, not in, in, not in
    #     2 : in, in, not in, not in
    # This function uses the above logic to determine all possibilities of
    # a number being in the function name or not.
    mod = 1
    vals = {}
    
    # 
    array.each do |i|
      vals[i] = calc_values(mod, array.length, str[i])
      mod *= 2
    end

    return_array = []
    (2 ** array.length).times do |i|
      temp = {}
      array.each do |j|
        temp[j] = vals[j][i]
      end
      return_array << temp
    end
    
    return return_array
  end

  def get_possible_function_name
    # Returns list of all possible function names
    func = self.gsub(" ", "_")
    
    # In the function name, replaces the name of the object the
    # function is being called from (specified between ":") with "$n"
    # For example: :a: is array?; => $n_is_array?
    array = get_chars_between_char(":", func)
    array.each do |elem|
      elem = ":" + elem + ":"
      func = func.gsub(elem, "$n")
    end
    func = func.gsub("::", "$n") if (array.empty?)

    # In the function name, replaces parameter names
    # (specified between "'") with "$"
    # For example: multiply 'x' by 'y'; => multiply_$_by_$
    array = get_chars_between_char("'", func)
    (1..array.length).each do |i|
      elem = "'" + array[i-1] + "'"
      func = func.sub(elem, "$")
    end

    # Replaces strings in the function call with "$"
    while (func.count('"') > 1 && func.count_chars_in_row('"') != 2)
      str = func[func.index('"')+1..-1]
      str = str[0..str.index('"')-1]
      str = '"' + str + '"'
      str = '""' if (str == '"""')
      func = func.sub(str, "$")
    end

    # Replaces floats, arrays and boolean values with "$"
    func = func.gsub(/\d+\.\d+/, "$") # Float
    func = func.gsub(/\[[[^\]],]*[^\]]*\]/, "$") # Array
    func = func.gsub(/(true|false)/, "$")

    # Adds all numbers in "func" to ---------------- 
    array = []
    all_digits = []
    func.split("_").each do |entry|
      if (entry.is_digit?)
        all_digits << entry
      end
    end
    
    all_values = calc_all_values(all_digits, func)
    
    # Calculates all the possible function names depending on the
    # values from "calc_all_values" and adds them to an array
    all_values.each do |entry|
      temp_func = func.dup
      entry.each do |key, val|
        temp_func[key] = val
      end
      array << temp_func
    end

    return array
  end

  def special_split(splitchar)
    # Splits "self" on all "splitchar", if "splitchar" isn't between
    # ":", "'", "[" or "]". Returns array containing all substrings
    return_array = []
    copy = self.dup
    start = 0
    finish = 0

    getvar = false
    usevar = false
    string = false
    array = false

    while (finish < copy.length)
      case copy[finish]
      when ":"
        usevar = !usevar
      when "'"
        getvar = !getvar
      when '"'
        string = !string
      when "["
        array = true
      when "]"
        array = false
      when splitchar
        if (!getvar && !usevar && !string && !array)
          return_array << copy[start..finish-1]
          start = finish + 1
        end
      end
      finish += 1
    end

    return_array << copy[start..-1]
    return return_array
  end

  def get_vars_in_order(function_name)
    # Gets all the variables inserted to a function in order,
    # to be inserted in the correct order as parameters
    array = []

    # Split both the function name and the function call ("self") by "_"
    funcsplit = function_name.special_split("_")
    selfsplit = self.special_split("_")
    
    # If the function name has "$" at an index,
    # add the same index to the returning array
    funcsplit.length.times do |i|
      if (funcsplit[i] == "$")
        array << selfsplit[i]
      end
    end
    
    return array
  end
end

class ClassHolder
  # Holder for all classes.
  # "name" is the class name, "memfuncs" contains the member functions,
  # "memvars" contains the member variables and
  # "inherits" contains the class names of all the classes
  # the class inherits from
  attr_accessor :name, :inherits, :memfuncs, :memvars
  def initialize()
    @name = nil
    @memfuncs = {}
    @memvars = {}
    @inherits = []
  end
end

class VarHolder
  # Holder for all variables.
  # "name" is the variable name, "type" is the variable type and
  # "value" stores the current value of the variable
  attr_accessor :name, :type, :value
  def initialize(name=nil, type=nil, value=nil)
    @name = name
    @type = type
    @value = value
  end

  def dup
    # Returns a copy of "self"
    return VarHolder.new(@name, @type, @value)
  end

  def get_lowest_self
    # Returns the deepest self 
    val = self
    # While the value of val is of the type Hash,
    # set "val" to the next "self"
    while (val.value.class == Hash)
      if (val.value.has_key?("self"))
        val = val.value["self"]
      else
        raise "Can't find the deepest self for variable #{@name}"
      end
    end
    return val
  end
end

class FuncHolder
  # Holder for all functions.
  # "name" is the name of the function,
  # "parameters" contains all parameters of the function, 
  # "contents" contains the content of the function and
  # "returntype" contains the type of the value the function returns
  attr_reader :name
  attr_accessor :parameters, :contents, :returntype
  def initialize(name=nil, parameters=[], contents=[], returntype=nil)
    @name = name
    @parameters = parameters
    @contents = contents
    @returntype = returntype
    check_name unless @name == nil
  end

  def content
    @contents
  end

  def name=(name)
    @name = name
    check_name
  end

  def check_name
    # Checks if the parameters in the function name is written in the
    # correct order.
    return nil if @parameters.length == 0
    array = @name.split("$")
    expected = 1
    (1..array.length-1).each do |i|
      if (array[i][0] == expected.to_s)
        expected += 1
      elsif (array[i][0] == "n")
        expected += 0
      else
        raise "A function name has to have it's parameters in order!\nError: #{@name}"
      end
    end
  end

  def find_return_in_holder(holder, ifs, whiles)
    # Returns the return statement of the function
    return_stmt = nil
    
    # Goes through each line in the functions content and sets "return_stmt"
    # to the line if the line contains "return ". Also checks for
    # return statements in if and while statements recursively
    holder.content.each do |content|
      if (content.class == String && content =~ /return\s/)
        return_stmt = content
        break
      elsif (content.class == String && content =~ /if \d+/)
        return_stmt = find_return_in_holder(ifs[content[/\d+/].to_i], ifs, whiles)
        break if (return_stmt != nil)
      elsif (content.class == String && content =~ /while \d+/)
        return_stmt = find_return_in_holder(whiles[content[/\d+/].to_i], ifs, whiles)
        break if (return_stmt != nil)
      end
    end
    
    return return_stmt
  end

  def get_first_return_var(ifs, whiles)
    # Returns the variable name of the first return statement.
    # If the return statement doesn't contain a variable the
    # function returns "nil".
    return_stmt = find_return_in_holder(self, ifs, whiles)
    
    return "" if (return_stmt == nil)
    return_stmt = return_stmt.sub("return ", "")
    return return_stmt.strip
  end
  
  def general_name
    # Returns the general name of the function.
    # Replaces all "$<number>" with just "$"
    # Example: multiply_$1_$2 => multiply_$_$
    name = @name
    (1..@parameters.length).each do |i|
      name = name.sub("$#{i}", "$")
    end
    return name
  end
end

class IfHolder
  # Holder for all if statements.
  # "content" contains the content of the statement,
  # "logicstmt" contains the logical expression and
  # "othercont" contains the "otherwise" content
  attr_accessor :content, :logicstmt, :othercont
  def initialize(cont=[], logicstmt=nil, othercont=[])
    @content = cont
    @logicstmt = logicstmt
    @othercont = othercont
  end

  def equals?(ifholder)
    # Returns true if "ifholder" contains the same data as "self"
    return (ifholder.content.eql?(@content) && ifholder.logicstmt == @logicstmt && ifholder.othercont.eql?(@othercont))
  end
end

class WhileHolder
  # Holder for all while statements
  # "content" contains the content of the statement and
  # "logicstmt" contains the logical expression
  attr_accessor :content, :logicstmt
  def initialize(cont=[], logicstmt=nil)
    @content = cont
    @logicstmt = logicstmt
  end

  def equals?(whileholder)
    # Returns true if "ifholder" contains the same data as "self"
    return (whileholder.content.eql?(@content) && whileholder.logicstmt == @logicstmt)
  end
end
