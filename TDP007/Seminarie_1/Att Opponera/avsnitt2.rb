# -*- coding: utf-8 -*-
############## Uppgift 5 ###############


class PersonName

  #Instansieras med ett för och efternamn
  def initialize(name, surname)
    @name = name
    @surname = surname
  end

  #Skapar ett virtuellt 'fullname' av de givna parametrarna
  def fullname
    @name + " " + @surname
  end

  #Ändrar 'name' och 'surname'
  def fullname=(new_name)
    @name = new_name.split(" ").first
    @surname = new_name.split(" ").last
  end
  
end


################ Uppgift 6 ##############

#require 'date'


class Person
  
  #Instansieras valfritt med 'name', 'surname' och 'age'
  def initialize(name = "", surname = "", age = 0)
    @age = age
    @birthyear = Time.new.year - age

    #Namnet använder sig av PersonName klassen
    @name = PersonName.new(name, surname)
  end

  #Tar in en ny ålder och ändrar den och födelseåret
  def age=(new_age)
    @age = new_age
    @birthyear = Time.new.year - age
  end

  #Tar in ett nytt födelseår och ändrar det och åldern
  def birthyear=(new_birthyear)
    @birthyear = new_birthyear
    @age = Time.new.year - birthyear
  end
  
  attr_reader :age, :birthyear, :name
end
