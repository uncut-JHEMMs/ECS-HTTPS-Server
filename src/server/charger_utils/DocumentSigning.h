#pragma once

#include <string_view>
#include <openssl/sha.h>
#include <memory>
#include <sstream>
#include <array>
#include <fstream>
#include <utility>

class DocumentSigning{
private:

  static std::string private_key;
  static std::string certificate;
  
  const std::array<char, 64> base64Mapping = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
				      'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
				      'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a',
				      'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
				      'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
				      't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1',
				      '2', '3', '4', '5', '6', '7', '8', '9', '+',
				      '/'};
  //Likely to be innefficient. Should look into updating.
  std::stringstream base64_Encode(std::stringstream& data){

    std::stringstream binary;
    std::unique_ptr<char[]> tmparr = std::make_unique<char[]>(6);
    
    while(!data.eof()){
      data.get(tmparr.get(), 7);
      for(int i = 0; i < 6; i++){
	{
	  binary << std::bitset<8>(tmparr[i]);
	}
      }
    }
    
    std::stringstream base64;
    
    while(!binary.eof()){
      binary.get(tmparr.get(), 7);
      {
	base64 << base64Mapping[std::bitset<6>(tmparr.get()).to_ulong()];
      }
    }
    
    return base64;
  }
  
  std::stringstream hash_sha1(const std::string&& data){

    unsigned char* c_data = new unsigned char[data.size()];
    for(int i = 0; i < data.size(); i++){
      c_data[i] = data[i];
    }
    
    unsigned char hash[SHA_DIGEST_LENGTH];

    SHA1(c_data, sizeof(data) - 1, hash);
    delete[] c_data;
    
    std::stringstream stream;
    stream << hash;
    
    return stream;
  }
  
public:
  DocumentSigning() noexcept {
    
  }

  static void setCertKey(std::string& cert){
    certificate = cert;
  }
  
  static void setPrivateKey(std::string& p_key){
    private_key = p_key;
  }
  
  bool sign(std::string&& filename){

    if(certificate == "" || private_key == "")
      return false;
    
    std::ifstream ifile(filename);
    std::string data = "";
    std::string tmp = "";
    while(!ifile.eof()){
      ifile >> tmp;
      data += tmp;
    }
    
    ifile.close();


    std::stringstream signature;
    signature << "<Envelope xmlns=\"urn:envelope\">\n"
	      << "<emails>" << data << "\n</emails>\n"
	      << "<Signature xmlns=\"http://www.w3.org/2000/09/xmldsig#\">\n"
	      << "<SignedInfo>\n"
	      << "<CanonicalizationMethod Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\" />\n"
	      << "<SignatureMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#rsa-sha1\" />\n"
	      << "<Reference URI=\"\">\n"
	      << "<Transforms>\n"
	      << "<Transform Algorithm=\"http://www.w3.org/2000/09/xmldsig#enveloped-signature\" />\n"
	      << "</Transforms>"
	      << "<DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\" />\n"
	      << "<DigestValue></DigestValue>\n"
	      << "</Reference>\n"
	      << "</SignedInfo>\n"
	      << "<SignatureValue></SignatureValue>\n"
	      << "<KeyInfo>\n"
	      << "</KeyInfo>"
	      << "<X509Data>"
	      << "<X509Certificate>"
	      << "</X509Certificate>"
	      << "</X509Data>"
	      << "</Signature>"
	      << "</Envelope>";
    
    std::string ssData = signature.str();
    
    std::stringstream hashed_data = hash_sha1(std::move(ssData));
    std::stringstream digestValue = base64_Encode(hashed_data);

    
    return true;
  }
};

std::string DocumentSigning::private_key = "";
std::string DocumentSigning::certificate = "";
