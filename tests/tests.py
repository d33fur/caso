'''import testsSrc
import pytest
import cppyy

xl: float = 1.0 #left border
xr: float = 3.0 #right border
xs: float = 0.25 #step
x: float = 0

#equation initialization
dydx = cppyy.gbl.std.vector[float] (
    [(-2) * y[0] + x])

testsSrc.varsInit.'''