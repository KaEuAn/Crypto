import hashlib
import itertools
#test to check if we do hash right
m = hashlib.sha256()
m.update(str.encode("8987 KFC"))
print(m.hexdigest())

find_path = "7bb497d33b383a1f85206a3692e585641439e0f292a281772e11f5f5f0f8360b"
firstpart = itertools.product('0123456789', repeat=4)
flag = False
for i in firstpart:
    secondpart = itertools.product("BCDFGHJKLMNPRSTVWXYZ", repeat=3)
    for j in secondpart:
        st = ''.join(i) + ' ' + ''.join(j)  
        m = hashlib.sha256()
        m.update(str.encode(st))
        if m.hexdigest() == find_path:
            print("number:", st)
            flag = True
            break
    if flag:
        break
#6816 JBF