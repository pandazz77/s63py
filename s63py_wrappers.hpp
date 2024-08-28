#include "s63.h"
#include "s63client.h"
#include "s63utils.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

using key_pair = std::pair<std::string, std::string>;
using pykey_pair = std::pair<py::bytearray, py::bytearray>;

inline key_pair wrap_keys(const pykey_pair& keys){
    return {std::string(keys.first), std::string(keys.second)};
}

inline pykey_pair wrap_keys(const key_pair &keys){
    return {py::bytearray(keys.first), py::bytearray(keys.second)};
}

// using key_pair = std::pair<std::string, std::string>;
// using key_pair_wrapped = std::pair<int, int>;

// inline std::string str(int i){
//     return hexutils::int_to_bytes(i);
// }

// inline int intof(const std::string &s){
//     return substr_to_uint(s,0,s.length());
// }

// inline key_pair_wrapped toKPW(const key_pair& in){
//     return {intof(in.first), intof(in.second)};
// }

// inline key_pair fromKPW(const key_pair_wrapped& in){
//     return {str(in.first), str(in.second)};
// }

class S63exception : public std::exception {
public:
    explicit S63exception(const char *m) : message{m} {}
    const char *what() const noexcept override { return message.c_str(); }

private:
    std::string message = "";
};

const std::unordered_map<S63Error,std::string> ERRORS{
	{S63_ERR_FILE, "S63_ERR_FILE"},
	{S63_ERR_DATA, "S63_ERR_DATA"},
	{S63_ERR_PERMIT, "S63_ERR_PERMIT"},
	{S63_ERR_KEY, "S63_ERR_KEY"},
	{S63_ERR_ZIP, "S63_ERR_ZIP"},
	{S63_ERR_CRC, "S63_ERR_CRC"}
};

namespace S63WR {
    inline bool validateCellPermit(const py::bytearray &permit, const py::bytearray &hw_id ){
        return S63::validateCellPermit(std::string(permit),std::string(hw_id));
    }

    inline std::string createUserPermit(const py::bytearray &m_key, const py::bytearray& hw_id, const py::bytearray &m_id){
        return S63::createUserPermit(std::string(m_key),std::string(hw_id),std::string(m_id));
    }

    inline py::bytearray extractHwIdFromUserpermit(const std::string &userpermit, const py::bytearray &m_key){
        return py::bytearray(S63::extractHwIdFromUserpermit(userpermit,std::string(m_key)));
    } 

    inline std::string createCellPermit(const py::bytearray &hw_id, const py::bytearray &ck1, const py::bytearray &ck2, const std::string &cellname, const std::string &expiry_date){
        return S63::createCellPermit(std::string(hw_id),std::string(ck1),std::string(ck2),cellname,expiry_date);
    }

    inline pykey_pair extractCellKeysFromCellpermit(const std::string &cellpermit, const py::bytearray &hw_id){
        bool ok;
        auto result = S63::extractCellKeysFromCellpermit(cellpermit,std::string(hw_id),ok);
        if(ok) return wrap_keys(result);
        throw S63exception("extractCellKeys error");
    }

    inline py::bytearray decryptCell(const std::string &path, const pykey_pair &keys){
        std::string buffer;
        S63Error error = S63::decryptCell(path,wrap_keys(keys),buffer);
        if(error == S63_ERR_OK) return py::bytearray(buffer);
        throw S63exception(ERRORS.at(error).c_str());
    }

    inline py::bytearray encryptCell(const py::bytearray &buffer, const py::bytearray &key){
        std::string str_buffer(buffer);
        S63::encryptCell(str_buffer,std::string(key));
        return py::bytearray(str_buffer);
    }

    inline void decryptAndUnzipCellByKey(const std::string &in_path, const pykey_pair &keys, const std::string &out_path){
        S63Error error = S63::decryptAndUnzipCellByKey(in_path,wrap_keys(keys),out_path);
        if(error == S63_ERR_OK) return;
        throw S63exception(ERRORS.at(error).c_str());
    }


    class S63Client_WRAPPED: private S63Client{
        public:
            S63Client_WRAPPED(const py::bytearray& HW_ID, const py::bytearray& M_KEY, const py::bytearray& M_ID) :
                S63Client(std::string(HW_ID),std::string(M_KEY),std::string(M_ID)) {}
            inline const py::bytearray& getHWID() const {
                return py::bytearray(S63Client::getHWID());
            }
            inline const py::bytearray& getMID() const {
                return py::bytearray(S63Client::getMID());
            }
            inline const py::bytearray& getMKEY() const {
                return py::bytearray(S63Client::getMKEY());
            }
            void setHWID(const py::bytearray &HW_ID){
                S63Client::setHWID(std::string(HW_ID));
            }
            void setMID(const py::bytearray &M_ID){
                S63Client::setMID(std::string(M_ID));
            }
            void setMKEY(const py::bytearray &M_KEY){
                S63Client::setMKEY(std::string(M_KEY));
            }

            void installCellPermit(const std::string &cellpermit){
                bool ok = S63Client::installCellPermit(cellpermit);
                if(!ok) throw S63exception("installCellPermit error");
            }

            void importPermitFile(const std::string &path){
                bool ok = S63Client::importPermitFile(path);
                if(!ok) throw S63exception("importPermitFile error");
            }

            std::string getUserpermit(){
                return S63Client::getUserpermit();
            }

            py::bytearray open(const std::string &path){
                return py::bytearray(S63Client::open(path));
            }

            void decryptAndUnzipCell(const std::string &in_path, const std::string &out_path){
                S63Error error = S63Client::decryptAndUnzipCell(in_path, out_path);
                if(error == S63_ERR_OK) return;
                throw S63exception(ERRORS.at(error).c_str());
            }

            void decryptAndUnzipCell(const std::string &in_path, const std::string &cellpermit, const std::string &out_path){
                S63Error error = S63Client::decryptAndUnzipCell(in_path, cellpermit, out_path);
                if(error == S63_ERR_OK) return;
                throw S63exception(ERRORS.at(error).c_str());
            }
    };
};