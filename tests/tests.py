import cppyy
import pytest
import os

headerFilePath = os.path.join(f"{os.path.dirname(__file__)}", "caso.h")

cppyy.include("/Users/jungdongwook/vscode/caso/caso.h")
#cppyy.include(headerFilePath)

casoH = cppyy.gbl

#y-values initialization
y = cppyy.gbl.std.vector[float] (
    [2])

xl: float = 1.0 #left border
xr: float = 3.0 #right border
xs: float = 0.25 #step
x: float = 0

#equation initialization
dydx = cppyy.gbl.std.vector[float] (
    [(-2) * y[0] + x])

def zeroStep(xs):
    defNum = 1
    xs = 0
    returnDefVal(defNum, xs)

def maxStep(xs):
    defNum = 1
    xs = 2
    returnDefVal(defNum, xs)

def LEquR(xr):
    defNum = 2
    xl = xr
    returnDefVal(defNum, xs)

def returnDefVal(defNum, value):
    match defNum:
        case 1:
            xs = 0.25
        case 2:
            xl = 1.0
            xr = 3.0

a = casoH.caso.ODE(dydx, y, x, xr, xs)
casoH.caso.ODE.setButcherTableau(casoH.RungeKutta4)

result = a.casoH.rungeKutta()

expectedOutput = [0, 2]

for i in range(len(result)):
    print ("asdasdasdasd")
    print (result[i])

def testRungeKutta(capsys):
    a = casoH.ODE(dydx, y, x, xr, xs)
    casoH.setButcherTableau(casoH.RungeKutta4)

    result = a.rungeKutta()

    expectedOutput = [0, 2]

    for i in range(len(result)):
        print ("asdasdasdasd")
        print (result[i])

    '''for i in range(len(expectedOutput)):
        if expectedOutput[i] == result[i]:
            continue
        else:
            

    assert result == expectedOutput'''

__