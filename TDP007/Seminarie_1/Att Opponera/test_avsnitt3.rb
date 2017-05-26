require "./avsnitt3"
require "test/unit"

class TestAvsnitt3 < Test::Unit::TestCase
  
  def test_uppgift7
    assert_nothing_thrown { 1.fib() }
    assert_equal(0, 0.fib())
    assert_equal(1, 1.fib())
    assert_equal(1, 2.fib())
    assert_equal(377, 14.fib())
    assert_equal(39088169, 38.fib()) # Takes a long time to test!
  end

  def test_uppgift8
    assert_equal("I", "i".acronym())
    assert_equal("A", "areallylongword".acronym())
    assert_equal("B", "beCanAlsoStart".acronym())
    assert_equal("PAR", "pro at ruby".acronym())
    assert_equal("PAR", "PRO AT RUBY".acronym())
    assert_equal("LOL", "laugh          out         loud!!".acronym())
  end
end
