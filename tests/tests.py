import unittest
import subprocess
import os
import ctypes

class TestODELibrary(unittest.TestCase):

    lib = ctypes.cdll.LoadLibrary("../caso.h")

    def testRungeKutta(self):
        result = self.runCppTest("RungeKutta4")
        self.assertEqual(result, 2)

    def testForwardEuler(self):
        # Напишите тест для метода прямого Эйлера
        result = self.runCppTest("forwardEuler")
        self.assertEqual(result, 2)

    def testBackwardEuler(self):
        # Напишите тест для метода обратного Эйлера
        result = self.runCppTest("backwardEuler")
        self.assertEqual(result, 2)

    def testMidpoint(self):
        # Напишите тест для метода средней точки
        result = self.runCppTest("midpoint")
        self.assertEqual(result, 2)

    def testImplicitMidpoint(self):
        # Напишите тест для неявного метода средней точки
        result = self.runCppTest("implicitMidpoint")
        self.assertEqual(result, 2)

    # Добавьте больше тестов для других методов по мере необходимости

    def runCppTest(self, method):
        # Запустите программу на C++ с указанным методом и запишите вывод
        # Подстройте команду в зависимости от вашей системы сборки
        command = ["./../build/caso", method]
        process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        output, _ = process.communicate()
        return output.strip()

if __name__ == '__main__':
    unittest.main()
