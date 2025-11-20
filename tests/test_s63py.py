import s63py
import unittest
import os
from pathlib import Path


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

        self.cur_dir = Path(os.path.dirname(__file__))
        self.root_dir = self.cur_dir / ".."
        self.datasets_dir = self.root_dir / "s63lib" / "tests" / "dataset"

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

    def test_zip(self):
        test_unzipped_data = bytearray("This is a test unzipped data!!!!! 50 bytes length","utf-8")

        zipped_data = s63py.zip.zip("test.txt",test_unzipped_data)
        unzipped_data = s63py.zip.unzip(zipped_data)

        self.assertEqual(unzipped_data,test_unzipped_data)

    def test_encrypt_decrypt(self):
        s57datasets_dir = self.datasets_dir / "s57" / "in"
        cellname = "US5OH10M"                 # dataset cell name
        cellfname = cellname + ".000"         # dataset cell filename
        mkey = bytearray.fromhex("3938373635")
        hwid = bytearray.fromhex("3132333438")
        mid = bytearray.fromhex("3031")
        ck1 = bytearray.fromhex("C1CB518E9C")
        ck2 = bytearray.fromhex("421571CC66")
        expday = "20251231"


        # encryption =====
        with open(s57datasets_dir / cellfname, "rb") as f:
            s57cell_1 = f.read()

        cellpermit = s63py.createCellPermit(hwid,ck1,ck2,cellname,expday)
        s57cell_zip_1 = s63py.zip.zip(cellfname,bytearray(s57cell_1))
        s63cell_1 = s63py.encryptCell(s57cell_zip_1,ck1)
        # ================


        # decryption =====
        keys = s63py.extractCellKeysFromCellpermit(cellpermit,hwid)
        self.assertEqual(keys,(ck1,ck2),"exctract cell keys from cellpermit")
        s57cell_zip_2 = s63py.decryptCell(s63cell_1,ck1)
        s57cell_2 = s63py.zip.unzip(s57cell_zip_2)
        self.assertEqual(s57cell_1,s57cell_2,"Encryption/Decryption test")
        # ================

        

if __name__ == "__main__":
    unittest.main()