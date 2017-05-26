require './rdparse.rb'

class Langer
  def initialize
    @langParser = Parser.new ('langer') do
      @vars = Hash.new
      token(/\s+/)
      token(/\w+/) {|i| i}
      token(/./) {|i| i}

      start :valid do
        match(:assign)
        match(:expr)
      end

      rule :assign do
        match('(', 'set', :var, :expr, ')') {|_, _, a, b, _| @vars[a] = b}
      end

      rule :expr do
        match('(', 'or', :expr, :expr, ')') {|_, _, a, b, _| a or b}
        match('(', 'and', :expr, :expr, ')') {|_,_,a,b,_| a and b}
        match('(', 'not', :expr, ')') {|_,_,a,_| not a}
        match(:term)
      end

      rule :term do
        match('true') {true}
        match('false') {false}
        match(:getvar)
      end

      rule :var do
        match(/\w+/)
      end

      rule :getvar do
        match(/\w+/) {|a| @vars.has_key?(a) ? @vars[a] : false }
      end
    end

  end

  def done(str)
    ["quit","exit","bye",""].include?(str.chomp)
  end

  def run (interactive)
    unless interactive
      return @langParser.parse yield
    end

    print "LangParser => "
    str = gets
    if done(str)
      puts "Bye."

    else
      puts "=> #{@langParser.parse str}"
      run (true)
    end
  end

  def log(state = true)
    if state
      @langParser.logger.level = Logger::DEBUG
    else
      @langParser.logger.level = Logger::WARN
    end
  end
end
