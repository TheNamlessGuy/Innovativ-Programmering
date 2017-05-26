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

def get_name(line)
  return line[/\b([A-Za-z]|_)+\b/]
end

def get_score_difference(line)
  scores = line[/[0-9]+\s+-\s+[0-9]+/]
  if scores == nil
    return ""
  end
  scores = scores.split(" ")
  return scores.first.to_i - scores.last.to_i
end

def add_team_scores(scores)
  file = read_file(scores, "pre")
  hash = Hash.new
  1.upto(file.length - 1) do |i|
    key = get_name(file[i])
    value = get_score_difference(file[i])
    if (key != nil && value != nil)
      hash[key] = value
    end
  end
  return hash
end

def sort_score_hash(hash)
  hash.sort_by {|k, v| v}.reverse
end

hash = add_team_scores("football.txt")
hash = sort_score_hash(hash)
print hash
puts
