require './constraint_networks.rb'
require 'test/unit'

class Testadder < Test::Unit::TestCase
  def test_adder
    a = Connector.new("con")
    b = Connector.new("henke")
    c = Connector.new("britney")
    Adder.new(a, b, c)
    
    puts "TEST 1:"
    a.user_assign(10)
    b.user_assign(5)
    assert_equal(15, c.value )

    puts "TEST 2:"
    a.forget_value "user"
    c.user_assign(20)
    assert_equal(15, a.value)

    puts "TEST 3:"
    b.forget_value "user"
    b.user_assign(19)
    assert_equal(19, b.value)
    assert_equal(1, a.value)
    
    puts "TEST 4:"
    b.forget_value "user"
    b.user_assign(-20)
    assert_equal(40, a.value)
  end

  def test_mult
    a = Connector.new("con")
    b = Connector.new("henke")
    c = Connector.new("britney")
    Multiplier.new( a, b, c )

    puts "TEST 1: "
    a.user_assign(500)
    b.user_assign(1)
    assert_equal(500, c.value)

    puts "TEST 2:"
    a.forget_value "user"
    c.user_assign(20)
    assert_equal(20, a.value)

    puts "TEST 3:"
    b.forget_value "user"
    b.user_assign(10)
    assert_not_equal(14, b.value)
    assert_equal(2, a.value)

    puts "TEST 4:"
    b.forget_value "user"
    b.user_assign(-2)
    assert_equal(-10, a.value)

    a = Connector.new("con")
    b = Connector.new("henke")
    c = Connector.new("britney")
    Multiplier.new( a, b, c )
    
    puts "TEST_MULT 5:"
    a.user_assign(120)
    c.user_assign(360)
    assert_equal(3, b.value)
  end
end
