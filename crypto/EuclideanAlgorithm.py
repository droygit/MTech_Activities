num1 = int(input("Enter number 1: "))
num2 = int(input("Enter number 2: "))
small = 0
big = 0
r0 = 0
r1 = 0
if num1 > num2: 
    small = num2
    big = num1
    r0 = num1
    r1 = num2
else:
    small = num1
    big = num2
    r0 = num2
    r1 = num1

reminder = 1
print('Calculating GCD of {0} and {1} using Euclidean Algorithm\n'.format(num1,num2))
iteration = 0
while (small != 0):
    reminder = big % small
    result = (big-reminder) / small
    iteration = iteration + 1
    print('\t{0} = {1} x {2} + {3} | gcd({0},{2}) = gcd({2},{3})'.format(
        big,result,small,reminder))
    big = small
    small = reminder

print('\n\nFinal result = {0}'.format(big))
print('\n\n---- Complete ----\n\n')


print('Calculating GCD of r[0] = {0} and r[1] = {1} using Extended Euclidean Algorithm\n'.format(num1,num2))
ri_2 = r0
ri_1 = r1
qi = 0
si_2 = 1 # s0 = 1
ti_2 = 0 # t0 = 0
si_1 = 0 # s1 = 0
ti_1 = 1 # t1 = 1
i = 1

while True:
    i = i+1
    ri = ri_2 % ri_1
    qi_1 = (ri_2 - ri)/ri_1
    si = si_2 - qi_1*si_1
    ti = ti_2 - qi_1*ti_1
    
    if (ri == 0):
        break
    
    print('\ti={8} --> r[i-2] {0} = q[i-1] {1} x r[i-1] {2} + r[i] {3} | r[i] {3} = s[i] {4} x r[0] {5} + t[i] {6} x r[1] {7}'.format(ri_2,qi_1,ri_1,ri,si,r0,ti,r1,i))
    
    #if (i == 2):
    #    print('\ti\tq[i-1]\tr[i]\ts[i]\tt[i]')
    #print('\t{0}\t{1}\t{2}\t{3}\t{4}'.format(i,qi_1,ri,si,ti))

    ri_2 = ri_1
    ri_1 = ri
    si_2 = si_1
    ti_2 = ti_1
    si_1 = si
    ti_1 = ti

print('\n Inverse ==> {0} x {1} % {2} = {3}'.format(ti_1,r1,r0,(ti_1*r1%r0)))

print('\n\n ---- Complete ----\n\n')
