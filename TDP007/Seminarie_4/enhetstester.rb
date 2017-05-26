require 'test/unit'
require './constraint_networks.rb'
require './constraint-parser.rb'

class TestC2F < Test::Unit::TestCase
  def test_coolio
    c, f = celsius2fahrenheit
    c.user_assign(-1)
    assert_equal(30.2, f.value)

    c.user_assign(100)
    assert_equal(212, f.value)

    c.forget_value "user"
    f.user_assign(400)
    assert_in_delta(204.40, c.value, 0.1)

    f.user_assign(-15)
    assert_in_delta(-26.1, c.value, 0.1)

    f.forget_value "user"
    c.user_assign(1)
    assert_equal(33.8, f.value)
  end
end

class TestLanguage < Test::Unit::TestCase
  def test_aaaaaa
    parser = ConstraintParser.new
    c, f = parser.parse "9*c=5*(f-32)"
    
    c.user_assign 5
    assert_equal(41, f.value)

    c.forget_value "user"
    assert_equal(false, c.value)

    f.user_assign 41
    assert_equal(5, c.value)
  end
end
