#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
@author: Debojyoti Roy
"""

proc = int(input("Enter num of processes "))
 
msgs = []
proclist = []
 
#Example test data
gstate={}
initdict = {}
prcs=[]
 
for i in range(proc):
  pname = input("Process Name ")
  pvalue = input("Process Value ")
  prcs.append(pname)
  initdict[pname] = int(pvalue)
 
print(prcs)
 
for t in range(1,10):
  gstate[t]=initdict
print(gstate)
 
msgs = [['p1',1,'p2', 2, 20, 'white'],
        ['p1',2,'p2', 6, 30, 'white'],
        ['p1',3,'p2', 5, 10, 'white'],
        ['p2',4,'p1', 6, 30, 'white'],
        ['p2',7,'p1', 8, 20, 'white']]
newgstate=[]
 
#Initializing the list
k = 1
for key in initdict:
  newlist=[]
  newlist.append(k)
  newlist.append(initdict[key])
  newlist.append(key)
  newgstate.append(newlist)
  k=k+1
 
#Updating values
for timer in range(2,10):
  for p in prcs: 
    for m in msgs:
      if(m[0]==p and timer-1==m[1]):
        gstate[timer][p]=gstate[timer-1][p]-m[4]       
      elif(m[2]==p and timer-1==m[3]):
        gstate[timer][p]=gstate[timer-1][p]+m[4]        
      else: 
        gstate[timer][p]=gstate[timer-1][p]
    print(gstate[timer][p], timer, p)
    newlist=[]
    newlist.append(timer)
    newlist.append(gstate[timer][p])
    newlist.append(p)
    newgstate.append(newlist)
print(newgstate)
 
#Marking messages as red
redmsg = 'p1'
redmsgtime = 3
restred= 999
otherredprocess = {}
gsnapshot={}
gsnapshot[redmsg]=redmsgtime
for m in msgs:
  if(m[0]==redmsg and m[1]>=redmsgtime):
    m[5]='red'
    if m[3]<restred:
      restred=m[3]
      otherredprocess[m[2]]=restred
 
#for other processes find the timestamp when each of them turned red
 
print(">>>>>",msgs, otherredprocess)
 
for i in otherredprocess:
  for m in msgs:
    if(m[0]==i and m[1]>=otherredprocess[i]):
      m[5]='red'
print("======",msgs)
 
#redtimes
redtimes=[]
redtimes.append(redmsgtime)
#Snapshot for starter process
for n in newgstate:
  if(n[0]==redmsgtime and n[2]==redmsg):
    gsnapshot[n[2]]=n[1]
 
 
#For other processes
temp =0
newredproc = ''
for m in msgs:
  if(m[0]==redmsg and m[1]==redmsgtime):
    temp = m[3]
    newredproc = m[2]
    redtimes.append(temp)
    break
for n in newgstate:
  if(n[0]==temp and n[2]==newredproc):
    gsnapshot[n[2]]=n[1]
 
print("-=-=-=-=",redtimes)
 
 
 
print("Global", gsnapshot)
C12Sent = 0
C12Recv = 0
C21Recv = 0
C21Sent = 0
for m in msgs:
  if(m[5]=='white' and m[0]=='p1' and m[2] == 'p2' and m[1]<redtimes[0]):
    C12Sent+=m[4]
  if(m[5]=='white' and m[0]=='p1' and m[2] == 'p2' and m[3]<redtimes[1]):
    C12Recv+=m[4]
  if(m[5]=='white' and m[0]=='p2' and m[2] == 'p1' and m[1]<redtimes[1]):
    C21Sent+=m[4]
  if(m[5]=='white' and m[0]=='p2' and m[2] == 'p1' and m[3]<redtimes[0]):
    C21Recv+=m[4]  
 
print("Channels", C12Sent, C12Recv, C21Sent, C21Recv)
 
print("Final Snapshot:")
finalvalue = 0
#TO find integrity of the system value
finalvalue+=gsnapshot['p1']
finalvalue+=gsnapshot['p2']
finalvalue+=(C12Sent - C12Recv)
finalvalue+=(C21Sent - C21Recv)
print("Value of P1 + Value of P2 + Value of messages in transit = ", finalvalue)