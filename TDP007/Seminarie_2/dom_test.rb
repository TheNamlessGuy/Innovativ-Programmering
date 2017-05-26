# -*- coding: utf-8 -*-
require 'rexml/document'


class Happening
  attr_accessor :event, :date, :location
  def initialize()
    @event = "UNKNOWN"
    @date = "UNKNOWN"
    @location = "UNKNOWN"
  end
  
  def print_happening()
    print @event + " is happening on " + @date + " at "
    puts @location
  end
end

def get_info(xpath, element)
  return element.get_elements(xpath)[0].text
end

if __FILE__ == $0
  doc = REXML::Document.new(File.open "./events.html")
  array = Array.new

  doc.elements.each('//div[@class="vevent"]//span[@class="summary"]') do |i|
    array << Happening.new()
    array.last.event = i.text
  end

  counter = 0
  doc.elements.each('//div[@class="vevent"]//span[@class="dtstart"]') do |i|
    array[counter].date = i.text
    counter += 1
  end

  counter = 0
  #Lite långsam
  doc.elements.each('//div[@class="vevent"]//td[@class="location"]') do |i|
    org_fn = get_info('//strong[@class = "org fn"]', i)
    street_address = get_info('//span[@class = "street-address"]', i)
    locality = get_info('//span[@class = "locality"]', i)
    region = get_info('//span[@class = "region"]', i)

    array[counter].location = org_fn + " " + street_address + " " + locality + " " + region
    counter += 1
  end

  array.length.times do |i|
    array[i].print_happening
  end
end
