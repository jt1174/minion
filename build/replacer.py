def reader(filename):
    begin=0
    end=0
    f = open(filename + '.minion', 'r')
    data = f.readlines()
    for i in range(len(data)):
        if ('**CONSTRAINTS**' in data(i)):
            begin=i+1
        elif ('**EOF**' in data(i)):
            end = i
    data=data[begin:end]
    return data
def parser(data):
    vars={}
    constraints = set([])
    for i in data:
        p = 0
        c = ''
        v
        for j in i:
            if j != '(' and j != '[' and p==0:
                c += j

        constraints.add(c)