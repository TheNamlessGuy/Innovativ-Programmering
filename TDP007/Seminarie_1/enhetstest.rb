# -*- coding: utf-8 -*-
require './iterators.rb'
require './atkomst.rb'
require './class_extension.rb'
require './regexp.rb'
require 'test/unit'

class TestUpg1a < Test::Unit::TestCase
  def test_simple
    assert_equal(3, n_times(3) { puts "Hello!" })

    number = 0
    n_times(3) { number += 1 }
    assert_equal(3, number)

    number = 0
    n_times(10) { number *= 2 }
    assert_equal(0, number)
  end
end

class TestUpg1b < Test::Unit::TestCase
  def test_simple
    repeat_thrice = Repeat.new(3)
    assert_equal(3, repeat_thrice.each { puts "Hooray!" })

    test = 5
    repeat_thrice.each { test += 2 }
    assert_equal(test, 11)

    test = "foo"
    repeat_thrice.each { test += "foo" }
    assert_equal("foofoofoofoo", test)
  end
end

class TestUpg2 < Test::Unit::TestCase
  def test_simple
    assert_equal(2432902008176640000, faculty(20))
    assert_equal(1, faculty(0))
    assert_equal(2, faculty(2))
    assert_equal(120, faculty(5))
  end
end

class TestUpg3 < Test::Unit::TestCase
  def test_simple
    assert_equal("apelsin", longest_string(["gris", "apelsin", "banan", "citron"]))
    assert_equal("abc", longest_string(["a", "ab", "abc"]))
  end
end


class TestUpg4 < Test::Unit::TestCase
  def test_simple
    assert_equal("apelsin", find_it(["gris", "apelsin", "banan", "citron"]) { |a, b| a.length > b.length })
    assert_equal("gris", find_it(["gris", "apelsin", "banan", "citron"]) { |a, b| a.length < b.length })
    assert_equal(4653, find_it([1,4653,2,875,2]) { |a, b| a > b })
  end
end

class TestUpg5 < Test::Unit::TestCase
  def test_simple
    name = PersonName.new("Henrik", "Björs")

    assert_equal("Björs Henrik", name.fullname)

    name.fullname = "HG Mannen"

    assert_equal("HG Mannen", name.fullname)
    assert_equal("Mannen", name.name)
    assert_equal("HG", name.surname)
  end
end

class TestUpg6 < Test::Unit::TestCase
  def test_simple
    henrik = Person.new(8, "henke", "man")
    
    assert_equal(8, henrik.age)
    assert_equal(2007, henrik.birthyear)
    assert_equal("man henke", henrik.name.fullname)

    henrik.birthyear = 1989
    
    assert_equal(26, henrik.age)
    assert_equal(1989, henrik.birthyear)
    assert_equal("man henke", henrik.name.fullname)

    henrik.age = 79

    assert_equal(79, henrik.age)
    assert_equal(1936, henrik.birthyear)
    assert_equal("man henke", henrik.name.fullname)
  end
end

class TestUpg7 < Test::Unit::TestCase
  def test_simple
    assert_equal(3, 4.fib)
    assert_equal(5, 5.fib)
    assert_equal(8, 6.fib)
  end
end

class TestUpg8 < Test::Unit::TestCase
  def test_simple
    assert_equal("LOL", "Laugh out Loud".acronym)
    assert_equal("DWIM", "Do what I mean!!".acronym)
  end
end

class TestUpg9 < Test::Unit::TestCase
  def test_simple
    assert_equal([2,3,1], [1, 2, 3].rotate_left)
    assert_equal([1, 2, 3], [1, 2, 3].rotate_left(3))
  end
end

class TestUpg10 < Test::Unit::TestCase
  def test_simple
    assert_equal("Brian", get_username("USERNAME: Brian"))
    assert_not_equal("Fiaeifgeag", get_username("USERNAE:Brian"))
  end
end

class TestUpg11 < Test::Unit::TestCase
  def test_simple
    test = ["html", "head", "title", "meta charset=\"UTF-8\"", "body", "header", "br", "section id=\"articleTitleContainer\"", "section id=\"articleTextContainer\"", "h3", "nav"]
    html = open("http://www-und.ida.liu.se/~maxbr431/TDP001/") { |f| f.read }
    assert_equal(test, tag_names(html))
  end
end


class TestUpg12 < Test::Unit::TestCase
  def test_simple
    assert_equal("FMA297", regnr("Min bil heter FMA297"))
    assert_equal(false, regnr("XQT784"))
  end
end
