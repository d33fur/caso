import testsSrc
import pytest
import cppyy

y = cppyy.gbl.std.vector[float]([2])
x: float = 0
xl: float = 1.0 #left border
xr: float = 3.0 #right border
xs: float = 0.25 #step

#equation initialization

testsSrc.valSet.setValues(testsSrc.f4, y, x, xl, xr, xs)

testsSrc.startTests()