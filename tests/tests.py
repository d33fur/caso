import cppyy
import pytest
import os

headerFilePath = os.path.join(os.path.dirname(__file__), "caso.h")

cppyy.include(headerFilePath)

caso = cppyy.gbl

RungeKuttaTable = cppyy.gbl.std.vector[cppyy.gbl.std.vector[float]](
    [
        [0.],
        [1. / 2., 1. / 2.],
        [1. / 2., 0., 1. / 2.],
        [1., 0., 0., 1.],
        [1. / 6., 1. / 3., 1. / 3., 1. / 6.]
    ]
)

DormanPrinceTable = cppyy.gbl.std.vector[cppyy.gbl.std.vector[float]](
    [
        [0.],
        [1. / 5., 1. / 5.],
        [3. / 10., 3. / 40., 9. / 40.],
        [4. / 5., 44. / 45., -56. / 15., 32. / 9.],
        [8. / 9., 19372. / 6561., -31760. / 2187., 10448. / 6561., 0., 25360. / 2187.],
        [1., 9017. / 3168., -355. / 33., 46732. / 5247., 49. / 176., -5103. / 18656., 0., 5. / 143.],
        [1. / 2., 35. / 384., 0., 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 0.],
        [35. / 384., 0., 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 0.],
        [71. / 57600., 0., -71. / 16695., 71. / 1920., -17253. / 339200., 22. / 525., -1. / 40]
    ]
)

HeunTable = cppyy.gbl.std.vector[cppyy.gbl.std.vector[float]](
    [
        [0.],
        [1., 1.],
        [1. / 2., 1. / 2.],
        [1., 0.],
    ]
)

FehilbergTable = cppyy.gbl.std.vector[cppyy.gbl.std.vector[float]](
    [
        [0.],
        [1. / 4., 1. / 4.],
        [3. / 8., 3. / 32., 9. / 32.],
        [12. / 13., 1932. / 2197., -7200. / 2197., 7296. / 2197.],
        [1., 439. / 216., -8., 3680. / 513., -845. / 4104.],
        [1. / 2., -8. / 27., 2., -3544. / 2565., 1859. / 4104., -11. / 40.],
        [25. / 216., 0., 1408. / 2565., 2197. / 4104., -1. / 5., 0.],
        [-1. / 360., 0., 128. / 4275., 2197. / 75240., -1. / 50., -2. / 55.]
    ]
)

BogackiShampineTable = cppyy.gbl.std.vector[cppyy.gbl.std.vector[float]](
    [
        [0.],
        [1. / 2., 1. / 2.],
        [3. / 4., 0., 3. / 4.],
        [1., 2. / 9., 1. / 3., 4. / 9.],
        [2. / 9., 1. / 3., 4. / 9., 0.],
        [-7. / 72., 1. / 12., 1. / 9., -1. / 8.]
    ]
)

def testRungeKutta(capsys):
    result = caso.rungeKutta

    expectedOutput = 

    assert result == expectedOutput