# Uppgift 7

class Integer
  def fib()
    n = 1
    a = 1
    s = 1
    if (self < 3)
      return 1
    end
    3.upto(self) {|b|
      s = n + a
      n = a
      a = s }
    return a
  end
end

# Uppgift 8

class String
  def acronym
    acro = String.new
    rest = self
    loop do
      first, *rest = rest.split(" ")
      acro += first[0].upcase
      rest = rest.join(" ")
      break if (rest.empty?)
    end
    return acro
  end
end

# Uppgift 9

class Array
  def rotate_left(n = 1)
    n.times {
      self.insert(self.length, self[0])
      self.delete_at(0)
    }
    return self
  end
end

