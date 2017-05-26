car_type ["default", 0], ["BMW", 5], ["Citroen", 4], ["Fiat", 3], ["Ford", 4], ["Mercedes", 5], ["Nissan", 4], ["Opel", 4], ["Volvo", 5]

zip_code ["default", 0], ["58937", 9], ["58726", 5], ["58647", 3]

years_with_license ["default", 0], [0, 1, 3], [2, 3, 4], [4, 15, 4.5], [16, 99, 5]

gender ["default", 0], ["M", 1], ["F", 0.5]

age ["default", 0], [18, 20, 2.5], [21, 23, 3], [24, 26, 3.5], [27, 29, 4], [30, 39, 4.5], [40, 64, 5], [65, 70, 4], [71, 99, 3]

rule "@policy_score *= 0.9 if (@gender == 'M' && @years_with_license < 3)"

rule "@policy_score *= 1.2 if (@cartype == 'Volvo' && @zipcode.to_s.split('')[0..1] == ['5', '8'])"
