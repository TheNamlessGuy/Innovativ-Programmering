class Filter_Data_By

  def initialize(value_regexp, key_regexp)
    @value_regexp = value_regexp
    @key_regexp = key_regexp
  end

  def filter(file)
    src_file = File.open(file, "r")
    sorted_hash = {}
  
    src_file.each {|line|
      values = line.match(@value_regexp)
      if values != [] and values != nil
        values = $1
        key_string = line.match(@key_regexp).captures[0]
        values = values.split(/\s*[\*-]*\s+/)
        sorted_hash[key_string] = (values[0].to_i - values[1].to_i).abs
      end
    }
    return sorted_hash.sort_by {|key, value| value}
  end

end


#goals = Filter_Data_By.new( /(\d+\s*-\s*\d+)/, /\s*([a-zA-Z_]{2,})\s*/)
#puts goals.filter("football.txt")

weather = Filter_Data_By.new( /^\s*\d+\s*(\d+[\*\s]*\d+)/, /^\s*(\d{1,})\s*/)
weather.filter("weather.txt").each {|key,val| puts "#{key}: #{val}" }
