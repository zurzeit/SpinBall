a_list = [1,2,3,4,5]
print(a_list)
r_list = [1,2]
for i in r_list[::-1]:
    del a_list[i]
print(a_list)