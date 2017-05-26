############## Uppgift 7 #################

# Extends the class Integer with a function fib that calculates the
# value of the nth fibonacci number
class Integer
  def fib
    if self == 0
      0
    elsif self == 1 or self == 2
      1
    else
      (self - 1).fib() + (self - 2).fib()
    end
  end
end

############## Uppgift 8 #################

# Extends the class String with a function acronym that returns
# the acronym for a object
class String
  def acronym
    words = self.split(/[\s]+/)
    words.inject("") { |result, word | result += word[0].upcase }
  end
end
