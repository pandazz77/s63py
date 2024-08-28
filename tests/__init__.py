import unittest
import tests.test_s63py

def s63_suite():
    loader = unittest.TestLoader()
    suite = loader.loadTestsFromModule(tests.test_s63py)
    return suite