class Person
  def initialize(brand, postal_code, license_time, gender, age)
    @brand = brand
    @postal_code = postal_code
    @license_time = license_time
    @gender = gender
    @age = age
  end

  def evaluate_policy(filename="policy.rb")
    policy = File.open(filename) { |f| f.read }
    insurance_policy = instance_eval policy

    common_policy = insurance_policy[0]
    age_pol = insurance_policy[1]

    res =  common_policy[@brand.to_sym] + common_policy[@gender.to_sym] + get_range(@license_time, common_policy) +
        get_range(@age, age_pol)

    unless common_policy[@postal_code.to_sym].nil?
      res += common_policy[@postal_code.to_sym]
    end

    if @gender == "M" and @license_time < 3
      res *= 0.9
    end
    if @brand == "Volvo" and @postal_code[0..1] == "58"
      res *= 1.2
    end

    res
  end

  def get_range(num, policy)
    policy.each do |key, val|
        return val.to_f if key.class == Range and key.include?(num)
    end
    0
  end
end