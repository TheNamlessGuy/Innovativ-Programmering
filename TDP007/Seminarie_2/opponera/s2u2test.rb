# -*- coding: utf-8 -*-
require "./s2u2"
require "test/unit"

class TestS2U2 < Test::Unit::TestCase
  def test
    assert_equal(["Barnaby Walters", "Rohit Khare", "Kevin Marks", "Ted O’Connor"], find_microformats(open("http://microformats.org/2014/06"), "h-card"))

    assert_equal(["The Dark Carnival - 101.9FM", "Sinister Sundays", "The Dark Carnival - 101.9FM", "Sinister Sundays", "The Dark Carnival - 101.9FM", "Sinister Sundays", "The Dark Carnival - 101.9FM", "Sinister Sundays"], find_microformats("events.html", "vevent"))
  end
end
