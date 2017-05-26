class String
  def is_num?
    /\b([0-9]|.)+\b/ === self
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
  index1 = line.split(" ")[index1].gsub("*", '')
  index2 = line.split(" ")[index2].gsub("*", '')
  if (index1 == nil || index2 == nil)
    return false
  elsif (index1.is_num? && index2.is_num?)
    return (index1.to_i - index2.to_i).abs
  else
    return false
  end
end

def get_token(index, string)
  return string.split(" ")[index]
end

def get_columns(line, day, lowest, highest)
  day_col = get_column(day, line)
  mxt_col = get_column(lowest, line)
  mnt_col = get_column(highest, line)
  return day_col, mxt_col, mnt_col
end

def read_columns(file, startval, endval, col1, col2, col3)
  hash = Hash.new
  startval.upto(endval) do |i|
    key = file[i].split(" ")[col1]
    value = get_column_difference(col2, col3, file[i])
    if (value != false)
      hash[key] = value
    end
  end
  return hash
end

if __FILE__ == $0
  file = read_file("weather.txt", "pre")

  day_col, mxt_col, mnt_col = get_columns(file[2], "Dy", "MxT", "MnT")

  hash = read_columns(file, 4, file.length - 2, day_col, mxt_col, mnt_col)

  hash = hash.sort_by {|k, v| v}.reverse
  print hash
  puts
end
