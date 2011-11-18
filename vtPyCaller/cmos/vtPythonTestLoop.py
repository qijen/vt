import vt

globalvar = 1.5

def init():
  print("initiated")
  vt.ActivateOut(bid,0,1)

def loop():
  print("loop in activated")
  vt.ActivateOut(bid,1,1)

#--------------------------- PyFuncEx enters here : -------------------------------#

def vTest():
  
  #-------call init when the first bb input is triggered :
  
  if vt.IsInputActive(bid,0) : 
    init()
  
  #-------call loop when the second bb input is triggered :
  if vt.IsInputActive(bid,1) : 
    loop()
    


    
  
    