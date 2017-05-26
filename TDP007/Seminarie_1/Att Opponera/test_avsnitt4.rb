require "./avsnitt4"
require "test/unit"
require "open-uri"

class TestAvsnitt4 < Test::Unit::TestCase
  
  def test_uppgift10
    assert_equal("Brian", get_name(": Brian") )
    assert_equal("Brian", get_name("hej: Brian") )
    assert_equal("K", get_name("hej igen   : K") )
    assert_equal("Carl", get_name("CARL is THEname: Carl") )
  end

  def test_uppgift11
    # Test a string with some tags in it
    tags_string = " asdf df <tag1><tag2> dfdf tag <tag3_space here are more text> \n <tag4> "
    tags_string_result = [["tag1"], ["tag2"], ["tag3_space"], ["tag4"]]
    assert_equal( tags_string_result, tag_names(tags_string) )

    # Test a real html-document
    html = open("http://www-und.ida.liu.se/~tomli962/TDP001/index.html") { |f| f.read }
    html_tags = [["html"], ["head"], ["title"], ["link"], ["meta"], ["body"], ["div"], ["header"], ["div"], ["div"], ["h1"], ["p"], ["div"], ["div"], ["div"], ["nav"], ["ul"], ["li"], ["a"], ["li"], ["a"], ["li"], ["a"], ["span"], ["a"], ["a"], ["main"], ["p"], ["p"], ["footer"]]
    assert_equal(html_tags, tag_names(html) )

    # Test a string with no tags
    assert_equal( [] , tag_names("This string has no tags.") )
  end
end
