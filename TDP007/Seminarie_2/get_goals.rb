class String
  def is_i?
    /\b[0-9]+\b/ === self
  end
end

def read_file(location, tag)
  file = File.new("./#{location}", "r")
  recording = false;
  lines = Array.new
  while (line = file.gets)
    if (line.include? "<#{tag}>")
      recording = true;
    elsif (line.include? "</#{tag}>")
      recording = false;
    elsif (recording)
      lines << line
    end
  end
  return lines
end

def get_column(tag, line)
  return line.split(" ").index(tag)
end

def get_column_difference(index1, index2, line)
  index1 = line.split(" ")[index1]
  index2 = line.split(" ")[index2]
  if (index1 == nil || index2 == nil)
    return false
  elsif (index1.is_i? && index2.is_i?)
    return index1.to_i - index2.to_i
  else
    return false
  end
end

def get_token(index, string)
  return string.split(" ")[index]
end

def get_most_average(hash)
  most_average = hash.first
  keys = hash.keys
  1.upto(hash.length - 1) do |i|
    if (hash[most_average.first].abs > hash[keys[i]].abs)
      most_average = [keys[i], hash[keys[i]]]
    end
  end
  return most_average
end

if __FILE__ == $0
  file_contents = read_file("football.txt", "pre")
  f_column = get_column("F", file_contents[0]) + 1
  a_column = get_column("A", file_contents[0]) + 2
  hash = Hash.new

  1.upto(file_contents.length - 1) do |i|
    key = get_token(1, file_contents[i])
    value = get_column_difference(f_column, a_column, file_contents[i])
    if (key != nil && value != false)
      hash[key] = value
    end
  end

  print "Least difference in goals: "
  most_average_team = get_most_average(hash)
  puts "#{most_average_team.first} with a difference of #{most_average_team.last}"

  hash = hash.sort_by {|key, value| value}
  hash = hash.reverse

  puts
  puts "Teams sorted from largest to smallest difference: "
  puts

  0.upto(hash.length - 1) do |i|
    puts "#{hash[i].first} with a difference of #{hash[i].last}"
  end

end
