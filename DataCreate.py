import random
def hint():
    print("\033[0m\033[1;34mMake your choice:\033[0m")
    print(" 0: create elements in increasing order.")
    print(" 1: create elements in random order")
    print(" 2: exit")
    pass
def error():
    print("\033[0m\033[1;31mERROR:Invalid Type\033[0m")
    print("Please choose again!")
    pass
while(1):
    hint()
    type=int(input(">>"))
    if type == 0 or type == 1 :
        filename=input("Name your file: ")
        filename=filename+".txt"
        path="./data/"+filename
        start=int(input("The data range is :\nfrom :"))
        end=int(input("to :"))
        li=[]
        fp=open(path,mode='w')
        for i in range(start,end+1):
            li.append(i)
        if type == 1:
            random.shuffle(li)
        fp.write(str(len(li))+'\n')
        for item in li :
            fp.write(str(item)+'\n')
        fp.close()
        print("\033[0m\033[1;34m{}\033[0m is created successfully!".format(filename))
        pass
    elif type == 2 :
        break
    else :
        error()
        pass