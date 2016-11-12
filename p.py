from random import *
nums = list(range(16))
shuffle(nums)

for i in range(16):
    
    out = ""
    for j in range(4):
        mask = 1 << 4 - j - 1
        if (mask & nums[i]) == mask:
            out += "1, "
        else:
            out += "0, "
    
    for j in range(5):
        mask = 1 << 5 - j - 1
        if (mask & (2*nums[i])) == mask:
            out += "1, "
        else:
            out += "0, "
    print(out)
    


