class Person
  def initialize(cartype, zipcode, years_with_license, gender, age)
    @cartype = cartype
    @zipcode = zipcode
    @years_with_license = years_with_license
    @gender = gender
    @age = age
    @policy_score = 0
  end
  
  def evaluate_policy(file)
    instance_eval(File.read(file))
    return @policy_score
  end
  
  def car_type(default, *lists)
    lists.each do |list|
      if (list.first == @cartype)
        @policy_score += list.last
        return
      end
    end
    @policy_score += default.last
  end
  
  def zip_code(default, *lists)
    lists.each do |list|
      if (list.first == @zipcode)
        @policy_score += list.last
        return
      end
    end
    @policy_score += default.last
  end

  def years_with_license(default, *lists)
    lists.each do |list|
      if ( @years_with_license.between?(list[0], list[1]) )
        @policy_score += list.last
        return
      end
    end
    @policy_score += default.last
  end
  
  def gender(default, *lists)
    lists.each do |list|
      if (list.first == @gender)
        @policy_score += list.last
        return
      end
    end
    @policy_score += default.last
  end
  
  def age(default, *lists)
    lists.each do |list|
      if ( @age.between?(list[0], list[1]) )
        @policy_score += list.last
        return
      end
    end
    @policy_score += default.last
  end

  def rule(rule)
    eval(rule)
  end
end
