# Uppgift 1
def n_times(num, &block)
  num.times { block.call }
end

class Repeat
  def initialize(num)
    @num = num
  end

  def each(&block)
    @num.times { block.call }
  end
end

# Uppgift 2
# puts (1..20).inject(1) { | fac, mod | fac * mod }

def faculty(num)
  (1..num).inject(1) { | fac, mod | fac * mod }
end

# Uppgift 3

def longest_string(array)
  array.max_by(&:length)
end

# Uppgift 4

def find_it(array, &block)
  longest = 0
  (array.length - 1).times { |i| longest = i if (block.call(array[i], array[longest])) }
  return array[longest]
end
