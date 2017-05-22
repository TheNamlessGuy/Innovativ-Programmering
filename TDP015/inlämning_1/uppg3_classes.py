class Var:
    def __init__(self, name):
        self.name = name

    def value(self, name_dict):
        return name_dict[self.name]

    def get_vars(self):
        return [self.name]

class Neg:
    def __init__(self, a):
        self.a = a

    def value(self, name_dict):
        return not self.a.value(name_dict)

    def get_vars(self):
        return self.a.get_vars()


class Conj:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def value(self, name_dict):
        return self.a.value(name_dict) and self.b.value(name_dict)

    def get_vars(self):
        array = self.a.get_vars()
        array.extend(self.b.get_vars())
        return array

class Disc:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def value(self, name_dict):
        return self.a.value(name_dict) or self.b.value(name_dict)

    def get_vars(self):
        array = self.a.get_vars()
        array.extend(self.b.get_vars())
        return array

class Impl:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def value(self, name_dict):
        return not (self.a.value(name_dict) and not self.b.value(name_dict))

    def get_vars(self):
        array = self.a.get_vars()
        array.extend(self.b.get_vars())
        return array

class Equi:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def value(self, name_dict):
        return (self.a.value(name_dict) == self.b.value(name_dict))

    def get_vars(self):
        array = self.a.get_vars()
        array.extend(self.b.get_vars())
        return array
