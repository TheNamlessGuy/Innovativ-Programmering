require 'rexml/streamlistener'
require 'rexml/document'
require 'open-uri'

class AwesomeListner
  include REXML::StreamListener

  def initialize(word_to_search_for)
    @found_event = false
    @word = word_to_search_for
    @findings = Array.new
  end

  attr_reader :findings

  def tag_start(name, attrs)
    if attrs["class"] == @word
      @found_event = true
    end
  end

  def tag_end(name)
    
  end

  def text(text)
    if @found_event and text[/^\s*$/] == nil
      @findings.push text
      @found_event = false
    end
  end
end

def find_microformats(filename, identifier)
  lst = AwesomeListner.new(identifier)
  src = File.new(filename)
  REXML::Document.parse_stream(src,lst)
  return lst.findings
end

print find_microformats("events.html", "vevent")
puts
