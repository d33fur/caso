import cppyy
import os
import math

headerFilePath = os.path.join(f"{os.path.dirname(__file__)}", "caso.h")

cppyy.include("/Users/jungdongwook/vscode/caso/caso.h")
#cppyy.include(headerFilePath)

casoH = cppyy.gbl

dydx = cppyy.gbl.std.vector[float]
y = cppyy.gbl.std.vector[float] #y-values list declaration
xl: float #left border
xr: float #right border
xs: float #step
x: float

M_E = math.e

def f0(yInp, xInp):
    global dydx, y, x
    y = yInp
    x = xInp
    dydx = cppyy.gbl.std.vector[float](
        [(-3) * yInp[0]])
    
def f1(yInp, xInp):
    global dydx, y, x
    y = yInp
    x = xInp
    dydx = cppyy.gbl.std.vector[float](
        [3 * xInp * xInp * yInp[0] + xInp * xInp * math.pow(M_E, math.pow(xInp, 3))])

def f2(yInp, xInp):
    global dydx, y, x
    y = yInp
    x = xInp
    dydx = cppyy.gbl.std.vector[float](
        [(-2) * xInp * yInp[0]])

def f3(yInp, xInp):
    global dydx, y, x
    y = yInp
    x = xInp
    dydx = cppyy.gbl.std.vector[float](
        [(-2) * yInp[0] + xInp])
    
def f4(yInp, xInp):
    global dydx, y, x
    y = yInp
    x = xInp
    dydx = cppyy.gbl.std.vector[float](
        [(-2) * yInp[0]])

class testList:

    def zeroStep():
        global xs
        defNum = 1
        xs = 0
        valSet.returnDefVal(defNum, xs) #todo: придумать куда деть

    def maxStep():
        global xs
        defNum = 1
        xs = 2
        valSet.returnDefVal(defNum, xs)

    def LEquR():
        global xl, xr
        defNum = 2
        xl = xr
        valSet.returnDefVal(defNum, xl)

class valSet:
    def setValues(func, yInp, xInp, xlInp, xrInp, xsInp):
        #using "global" for changing global variables values
        global dydx, y, x, xl, xr, xs
        y = yInp
        x = xInp
        xl = xlInp
        xr = xrInp
        xs = xsInp
        print(dydx, y, x, xl, xr, xs)
    
    def returnDefVal(self, defNum):
        global xl, xr, xs, x
        match defNum:
            case 1:
                xs = 0.25
            case 2:
                xl = 1.0
                xr = 3.0

#ODE class object initialization
'''a = casoH.caso.ODE(dydx, y, xl, xr, xs)

casoH.caso.ODE.setButcherTableau(casoH.RungeKutta4)

result = a.casoH.rungeKutta()

expectedOutput = [0, 2]

for i in range(len(result)):
    print ("asdasdasdasd")
    print (result[i])

def testRungeKutta(capsys):
    a = casoH.ODE(dydx, y, xl, xr, xs)
    casoH.setButcherTableau(casoH.RungeKutta4)

    result = a.rungeKutta()

    expectedOutput = [0, 2]

    for i in range(len(result)):
        print ("asdasdasdasd")
        print (result[i])

    ''''''for i in range(len(expectedOutput)):
        if expectedOutput[i] == result[i]:
            continue
        else:
            

    assert result == expectedOutput

__'''