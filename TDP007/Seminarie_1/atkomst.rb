# -*- coding: utf-8 -*-
require 'date'

# Uppgift 5
class PersonName
  def initialize(name, surname)
    @name = name
    @surname = surname
  end
  attr_accessor :name, :surname

  def fullname()
    return "#{@surname} #{@name}"
  end
  def fullname=(fullname)
    @surname, @name = fullname.split(" ")
    return nil
  end
end

# Uppgift 6
class Person
  attr_reader :name, :age, :birthyear
  attr_writer :name
  def initialize(age, name, surname)
    @age = age
    @name = PersonName.new(name, surname)
    @birthyear = Date.today.strftime("%Y").to_i - age
  end
  def birthyear=(newyear)
    @age += (@birthyear - newyear)
    @birthyear = newyear
  end
  def age=(newage)
    @birthyear += (@age - newage)
    @age = newage
  end
end
