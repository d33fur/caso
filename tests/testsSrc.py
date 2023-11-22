import cppyy
import os

headerFilePath = os.path.join(f"{os.path.dirname(__file__)}", "caso.h")

cppyy.include("/Users/jungdongwook/vscode/caso/caso.h")
#cppyy.include(headerFilePath)

casoH = cppyy.gbl

xl: float #left border
xr: float #right border
xs: float #step
x: float

y = cppyy.gbl.std.vector[float] ([2]) #y-values initialization

class testList:

    def zeroStep():
        global xs
        defNum = 1
        xs = 0
        valSet.returnDefVal(defNum, xs)

    def maxStep():
        g
        defNum = 1
        xs = 2
        valSet.returnDefVal(defNum, xs)

    def LEquR():
        defNum = 2
        xl = xr
        valSet.returnDefVal(defNum, xs)

class valSet:
    def setValues(self, dydxInp, yInp, xlInp, xrInp, xsInp):
        #using "global" for changing global variables values
        global dydx, y, xl, xr, xs
        dydx = dydxInp
        y = yInp
        xl = xlInp
        xr = xrInp
        xs = xsInp
    
    def returnDefVal(self, defNum):
        global xl, xr, xs, x
        match defNum:
            case 1:
                xs = 0.25
            case 2:
                xl = 1.0
                xr = 3.0

a = casoH.caso.ODE(dydx, y, xl, xr, xs)
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

    '''for i in range(len(expectedOutput)):
        if expectedOutput[i] == result[i]:
            continue
        else:
            

    assert result == expectedOutput'''

__