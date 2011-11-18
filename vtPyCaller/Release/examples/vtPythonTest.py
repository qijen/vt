import string


import vt

currentIndex = 0;

def loop():
  global currentIndex
  print (currentIndex)
  maxcount = vt.GetInVal(bid,3)
  startindex = vt.GetInVal(bid,4)
  step = vt.GetInVal(bid,5)
  currentIndex +=step
  vt.SetOutVal(bid,0,currentIndex)
  count = (currentIndex-startindex)/step
  if count >= maxcount:
    currentIndex = 0
    vt.ActivateOut(bid,0,1)
  else:
    vt.ActivateOut(bid,2,1)
  print(count)


def counter():
  if vt.IsInputActive(bid,0) :
    global currentIndex
    currentIndex = 0
    loop()
  if vt.IsInputActive(bid,1) :
    loop()
  
   
  
  
  