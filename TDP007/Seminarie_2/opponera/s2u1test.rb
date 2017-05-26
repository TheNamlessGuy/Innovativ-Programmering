require "./s2u1"
require "test/unit"

class TestS2U1 < Test::Unit::TestCase
  def test
    goals = Filter_Data_By.new( /(\d+\s*-\s*\d+)/, /\s*([a-zA-Z_]{2,})\s*/)
    assert_equal(["Tottenham", 4], goals.filter("football.txt")[2])
    assert_equal(["Aston_Villa", 1], goals.filter("football.txt")[0])

    weather = Filter_Data_By.new( /^\s*\d+\s*(\d+[\*\s]*\d+)/, /^\s*(\d{1,})\s*/)
    
    assert_equal(["27",19], weather.filter("weather.txt")[10])
    assert_equal(["14",2], weather.filter("weather.txt")[0])
    assert_not_equal(["9",54], weather.filter("weather.txt")[0])
  end
end
