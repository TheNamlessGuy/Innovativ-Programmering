require './avsnitt1'
require 'test/unit'

class TestUppgift1 < Test::Unit::TestCase
  def test_simple

    assert_equal(1, n_times(5) {puts "Tjaba"})
    assert_equal(1, n_times(50) {puts "135563477572424"})
    assert_equal(1, n_times(123) {puts "Nemen tjenaaa!"})
    assert_equal(1, n_times(0) {puts "Tjo"})

    assert_equal(1, Repeat.new(5).each {puts "Tjaba"})
    assert_equal(1, Repeat.new(50).each {puts "135563477572424"})
    assert_equal(1, Repeat.new(123).each {puts "Nemen tjenaaa!"})
    assert_equal(1, Repeat.new(0).each {puts "Tjo"})

  end
end


class TestUppgift2 < Test::Unit::TestCase
  def test_simple

    assert_equal(120, faculty(5) )
    assert_equal(2432902008176640000, faculty(20) )
    assert_equal(479001600, faculty(12) )
    assert_equal(1, faculty(0) )

  end
end
