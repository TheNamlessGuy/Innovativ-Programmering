require './uppg1.rb'
require './uppg2.rb'
require 'test/unit'

class TestUppg1 < Test::Unit::TestCase
  def test_simple
    kalle = Person.new("Volvo", "58435", 2, "M", 32)
    assert_equal(15.66, kalle.evaluate_policy("./policy.rb"))
    
    kalle = Person.new("Mercedes", "58937", 17, "M", 41)
    assert_equal(25, kalle.evaluate_policy("./policy.rb"))
  end
end

class TestUppg2 < Test::Unit::TestCase
  def test_simple
    lang = Lang.new
    lang.log(false)
    
    assert_equal(true, lang.test_string("(set a true)"))
    assert_equal(true, lang.test_string("(and a true)"))
    assert_equal(false, lang.test_string("(or false false)"))
    assert_equal(false, lang.test_string("(not a)"))
  end
end
