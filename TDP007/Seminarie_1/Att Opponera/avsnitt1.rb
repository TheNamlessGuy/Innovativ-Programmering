# -*- coding: utf-8 -*-
############# Uppgift 1 #################

#Funktionen anropar det givna blocket '&block', 'count' gånger
def n_times(count, &block)
  1.upto(count) do
    block.call()
  end
end


=begin
Klassens skapade objekt instansieras med ett tal,
det talet anger hur många gånger som metoden 'each'
ska anropa ett givet block som ges i metoden.
=end
class Repeat
  def initialize(count)
    @count = count
  end

  def each(&block)
    1.upto(@count) do
      block.call()
    end
  end

end


############# Uppgift 2 #################

#Beräknar fakulteten av 20
(1..20).inject(1) {| result, entry | result * entry }


#Tar in ett positivt heltal och beräknar fakulteten av det
def faculty(count)
  (1..count).inject(1) {| result, entry| result * entry }
end

