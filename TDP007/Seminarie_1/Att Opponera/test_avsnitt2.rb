require './avsnitt2'
require 'test/unit'

class TestUppgift5 < Test::Unit::TestCase
  def test_simple

    assert_equal("Rasmus Johansson", PersonName.new("Rasmus", "Johansson").fullname)
    assert_equal("RRRRRasmus JooHohansson", PersonName.new("RRRRRasmus", "JooHohansson").fullname)

    assert_equal("Tjo Tji", PersonName.new("Rasmus", "Johansson").fullname = "Tjo Tji")
    assert_not_equal("Barack Obama", PersonName.new("Barack", "YoMama").fullname = "Britney Spears")

  end
end


class TestUppgift6 < Test::Unit::TestCase
  def test_simple

    testPerson = Person.new("Rasmus", "Johansson")

    assert_not_equal("Rasmus Johansson", Person.new("Rasmus", "Johansson"))
    assert_equal("Rasmus Johansson", testPerson.name.fullname)
    assert_not_equal("Rasmus JoHansson", testPerson.name.fullname)
    
    testPerson.name.fullname = "Tommy Lindman"

    assert_equal("Tommy Lindman", testPerson.name.fullname)


  end
end
