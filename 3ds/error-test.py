def c():
    raise Exception

def b():
    c()

def a():
    b()

a()