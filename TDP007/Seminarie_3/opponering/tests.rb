require 'test/unit'
require './uppgift1.rb'
require './uppgift2.rb'

class Tests < Test::Unit::TestCase
  def test_upg1
    p1 = Person.new("Volvo", "58435", 2, "M", 32)
    p2 = Person.new("Opel", "13667", 2, "M", 31)

    assert_equal(15.66, p1.evaluate_policy("policy.rb"))
    assert_equal(12.15, p2.evaluate_policy("policy.rb"))
  end

  def test_upg2
    langp = Langer.new
    langp.log(false)

    assert(langp.run(false) {"(set t true)"})
    assert(langp.run(false) {"t"})
    assert(langp.run(false) {"(not false)"})
    assert(langp.run(false) {"(and true true)"})
    assert(langp.run(false) {"(or false (and true true))"})

    assert_equal(false, langp.run(false) {"(and false false)"})
    assert_equal(false, langp.run(false) {"(not true)"})
    assert_equal(false, langp.run(false) {"k"})

  end
end
