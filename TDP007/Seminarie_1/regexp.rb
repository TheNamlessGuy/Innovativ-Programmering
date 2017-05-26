# -*- coding: utf-8 -*-
require 'open-uri.rb'

# Uppgift 10

def get_username(name)
  sub = name[/[a-zA-Z]+:\s/]
  if (sub != nil)
    return name.sub(sub, '')
  else
    return nil
  end
end

# Uppgift 11

def tag_names(html)
  array = Array.new
  string = html[/<([a-zA-Z0-9]|\s|=|"|-)+>/]
  while (string != nil)
    array << string.sub('<', '').sub('>', '')
    html = html.sub(string, '')
    string = html[/<([a-zA-Z0-9]|\s|=|"|-)+>/]
  end
  return array.uniq
end

# Uppgift 12

def regnr(string)
  regnr = string[/[A-HJ-PR-UW-Z]{3}\d{3}/]
  (regnr == nil)? false : regnr
end

