# s63py


s63py is python bindings of s63lib (IHO data protection scheme s63 implementation).

## Requirements:
- cmake
- build-essentials

## Example usage:
```py
hw_id = bytearray.fromhex("3132333438")
m_key = bytearray.fromhex("3938373635")
m_id = bytearray.fromhex("3031")

s63_cell_path = "s63/NO4D06/NO4D06.000"
output_cell_path = "s57/NO4D06/NO4D06.000"
cellpermit = "NO4D061320000830BEB9BFE3C7C6CE68B16411FD09F96982795C77B204F54D4"

cell_keys = s63py.extractCellKeysFromCellpermit(cellpermit,hw_id)
s63py.decryptAndUnzipCellByKey(s63_cell_path,cell_keys,output_cell_path)
```

The S63Client class is an example how it might be used in a user-side application, like a chartplotter.

```py
s63client = s63py.S63Client(
    bytearray.fromhex(HW_ID_HEX),
    bytearray.fromhex(M_KEY_HEX),
    bytearray.fromhex(M_ID_HEX)
)

user_permit = s63client.getUserpermit()
s63client.installCellPermit("NO4D061320000830BEB9BFE3C7C6CE68B16411FD09F96982795C77B204F54D4")
# Or by importing all the cellpermits from a PERMITS.TXT file
s63client.importPermitFile("/paths/to/PERMITS.TXT")

s57_decrypted = s63client.open("/path/to/63cell/NO4D06/NO4D06.000")
with open("path/to/s57cell/NO4D06/NO4D06.000","wb") as f:
    f.write(s57_decrypted)
```