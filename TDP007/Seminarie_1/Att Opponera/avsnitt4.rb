############## Uppgift 10 ###############

def get_name(string)
  re = /:\s/
  md = re.match(string)
  md.post_match
end


############### Uppgift 11 ##############
require 'open-uri.rb'

def tag_names(string)
  re = /<(\w+)/
  string.scan(re)
end
