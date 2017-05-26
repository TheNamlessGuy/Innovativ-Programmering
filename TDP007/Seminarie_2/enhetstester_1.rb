# -*- coding: utf-8 -*-
require './get_goals.rb'
require './get_weather.rb'
require './dom_test.rb'
require 'test/unit'

class TestUppg1 < Test::Unit::TestCase
  def test_simple
    assert_equal(true, "123".is_i?)
    assert_equal(false, "123a".is_i?)
    
    assert_equal(["hej\n"], read_file("./hej.txt", "hej"))
    
    assert_equal(3, get_column("A", "B C D A"))
    assert_not_equal(11, get_column("A", "B H S A RF E D"))
    
    assert_equal(1, get_column_difference(0, 1, "2 1"))
    assert_not_equal(12, get_column_difference(0, 1 ,"2 3"))

    assert_equal("hej", get_token(0, "hej din galning!"))
    assert_not_equal("YO BIATCH", get_token(3, "Hej hej hej hej"))

    test = {"Banan" => 5, "Klitor is" => 10, "Pen 15" => 60}
    assert_equal(["Banan", 5], get_most_average(test))
  end
end

class TestUppg2 < Test::Unit::TestCase
  def test_simple
    doc = REXML::Document.new(File.open "./test.html")
    doc = doc.get_elements('/div[@class="Hej"]')[0]
    assert_equal("Hallo", get_info('//div[@class="Hallo"]', doc))
  end
end
