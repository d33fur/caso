import cppyy
import os
import math

#linking library
currentDir = os.path.dirname(os.path.abspath(__file__))
caso_dir = os.path.dirname(currentDir)
headerFilePath = os.path.join(caso_dir, "caso.h")
cppyy.include(headerFilePath)

casoH = cppyy.gbl

dydx = cppyy.gbl.std.vector[float]
y = cppyy.gbl.std.vector[float] #y-values list declaration
xl: float #left border
xr: float #right border
xs: float #step
x: float

M_E = math.e

#types of equations
def f0(dydxInp, yInp, xInp):
    dydx = cppyy.gbl.std.vector[float](
        [(-3) * yInp[0]])
    
def f1(dydxInp, yInp, xInp):
    dydx = cppyy.gbl.std.vector[float](
        [3 * xInp * xInp * yInp[0] + xInp * xInp * math.pow(M_E, math.pow(xInp, 3))])

def f2(dydxInp, yInp, xInp):
    dydx = cppyy.gbl.std.vector[float](
        [(-2) * xInp * yInp[0]])

def f3(dydxInp, yInp, xInp):
    dydx = cppyy.gbl.std.vector[float](
        [(-2) * yInp[0] + xInp])
    
def f4(dydxInp, yInp, xInp):
    dydx = cppyy.gbl.std.vector[float](
        [(-2) * yInp[0]])

class testList:
    def zeroStep():
        xsTest = 0

    def maxStep():
        defNum = 1
        xsTest = 2

    def LEquR():
        global xr
        defNum = 2
        xlTest = xr

class valSet:
    def setValues(funcInp, yInp, xInp, xlInp, xrInp, xsInp):
        #using "global" for changing global variables values
        global dydx, y, x, xl, xr, xs, func
        func = funcInp
        y = yInp
        x = xInp
        xl = xlInp
        xr = xrInp
        xs = xsInp
       
    '''def returnDefVal(self, defNum): (дописать потом, не стирать)
        global xl, xr, xs, x
        match defNum:
            case 1:
                xs = 0.25
            case 2:
                xl = 1.0
                xr = 3.0'''

def startTests():
    print("Start test of Runge Kutta")
    #ODE class object initialization
    a = casoH.caso.ODE(func, y, xl, xr, xs)
    a.setButcherTableau(casoH.caso.RungeKutta4)
    answer = a.rungeKutta() #ошибка 301 строка хедера

#casoH.caso.ODE.setButcherTableau(casoH.RungeKutta4)

'''result = a.casoH.rungeKutta()

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
        print (result[i])'''

'''for i in range(len(expectedOutput)):
        if expectedOutput[i] == result[i]:
            continue
        else:
            

    assert result == expectedOutput

__'''