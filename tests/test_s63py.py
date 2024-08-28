import s63py
import unittest

class TestS63(unittest.TestCase):
    def setUp(self):
        self.hw_id = bytearray.fromhex("3132333438")
        self.m_key = bytearray.fromhex("3938373635")
        self.m_id = bytearray.fromhex("3031")
        self.userpermit = "73871727080876A07E450C043031"
        self.ck1 = bytearray.fromhex("C1CB518E9C")
        self.ck2 = bytearray.fromhex("421571CC66")
        self.cellname = "NO4D0613"
        self.expiry_date = "20000830"
        self.cellpermit = "NO4D061320000830BEB9BFE3C7C6CE68B16411FD09F96982795C77B204F54D48"

    def test_userpermit(self):
        self.assertEqual(
            s63py.createUserPermit(self.m_key,self.hw_id,self.m_id),
            self.userpermit,
            "Create User Permit"
        )

    def test_extract_hwid(self):
        self.assertEqual(
            s63py.extractHwIdFromUserpermit(self.userpermit,self.m_key),
            self.hw_id,
            "Extract HW ID from User Permit"
        )

    def test_create_cellpermit(self):
        self.assertEqual(
            s63py.createCellPermit(self.hw_id,self.ck1,self.ck2,self.cellname,self.expiry_date),
            self.cellpermit,
            "Create Cell Permit"
        )

    def test_extract_cell_keys(self):
        self.assertEqual(
            s63py.extractCellKeysFromCellpermit(self.cellpermit,self.hw_id),
            (self.ck1, self.ck2),
            "Extract Cell Keys from Cell Permit"
        )

if __name__ == "__main__":
    unittest.main()