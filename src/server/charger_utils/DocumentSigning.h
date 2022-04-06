#pragma once

#include <string_view>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <memory>
#include <sstream>
#include <array>
#include <fstream>
#include <utility>
#include <string>
#include "../Logging.h"

#include <iostream>

class DocumentSigning{
private:

  static std::string private_key;
  static std::string certificate;
  static RSA* rsa;
  static Logging& log;
  
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
      data.get(tmparr.get(), 7, '\0');
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

  //hash the data using sha1. Look into using something better.
  std::stringstream hash_sha256(const std::string&& data){
    
    unsigned char* c_data = new unsigned char[data.size()];
    
    for(int i = 0; i < data.size(); i++){
      c_data[i] = data[i];
    }
    unsigned char hash[SHA_DIGEST_LENGTH];
    
    SHA256(c_data, sizeof(data) - 1, hash);

    delete[] c_data;
    
    std::stringstream stream;
    stream << hash;
    
    return stream;
  }

  //sign document using openssl rsa algorithm
  bool RSASign(const unsigned char* Msg,
              size_t MsgLen,
              unsigned char** EncMsg,
              size_t* MsgLenEnc) {

    EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
    EVP_PKEY* priKey  = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(priKey, rsa);

    if (EVP_DigestSignInit(m_RSASignCtx, nullptr, EVP_sha256(), nullptr, priKey) <= 0) {
      log.log("ERROR document signing: Failed to init digest sign.");
      return false;
    }
    if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0) {
      log.log("ERROR document signing: Failed to digest sign update.");
      return false;
    }
    if (EVP_DigestSignFinal(m_RSASignCtx, nullptr, MsgLenEnc) <= 0) {
      log.log("ERROR document signing: Failed to digest sign final.");
      return false;
    }

    *EncMsg = (unsigned char*)malloc(*MsgLenEnc);

    if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0) {
      log.log("ERROR document signing: Failed to digest sign final.");
      return false;
    }
    
    EVP_MD_CTX_reset(m_RSASignCtx);
    return true;
  }
  
public:
  DocumentSigning() noexcept {
    
  }

  //reads in a certificate to put in the document.
  static void setCert(std::string&& cert){

    std::ifstream ifile(cert.c_str());

    if(ifile.is_open()){
      char c;
      while(ifile.get(c)){
	
	certificate += c;
      }
    }
    else
      log.log("ERROR: Could not open document signing certificate.");
  }

  //Reads in a private key to sign the document.
  static void setPrivateKey(std::string&& p_key){
    
    std::ifstream ifile(p_key.c_str());

    if(ifile.is_open()){

      char c;      
      while(ifile.get(c)){
        private_key += c;
      }
      
      const char* c_string = private_key.c_str();
      BIO * keybio = BIO_new_mem_buf((void*)c_string, -1);
      if (keybio==NULL) {
	log.log("ERROR: Couldn't create private key.");
	return;
      }
      rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    }
    else
      log.log("ERROR: Could not open document signing private key.");
  }

  //Takes a file to be signed and signs it.
  bool sign(const std::string&& filename){

    if(certificate == "" || private_key == "")
      return false;

    std::stringstream signature;
    signature << "<Envelope xmlns=\"urn:envelope\">" << std::endl
              << "<emails>" << std::endl;

    std::ifstream ifile(filename);
    if(!ifile.is_open()){
      return false;
    }
    
    std::string tmp = "";
    while(!ifile.eof()){
      ifile >> tmp;
      signature << tmp << std::endl;
      tmp = "";
    }
    ifile.close();

    //Set envelope template
    signature << "</emails>" << std::endl
	      << "<Signature xmlns=\"http://www.w3.org/2000/09/xmldsig#\">" << std::endl
	      << "<SignedInfo>" << std::endl
	      << "<CanonicalizationMethod Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\" />" << std::endl
	      << "<SignatureMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#rsa-sha256\" />" << std::endl
	      << "<Reference URI=\"\">" << std::endl
	      << "<Transforms>" << std::endl
	      << "<Transform Algorithm=\"http://www.w3.org/2000/09/xmldsig#enveloped-signature\" />" << std::endl
	      << "</Transforms>" << std::endl
	      << "<DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#sha256\" />" << std::endl
	      << "</Reference>" << std::endl
	      << "</SignedInfo>" << std::endl
	      << "<KeyInfo>" << std::endl
	      << "</KeyInfo>" << std::endl
	      << "<X509Data>" << std::endl
	      << "</X509Data>" << std::endl
	      << "</Signature>" << std::endl
	      << "</Envelope>\0";
    
    //possibility for poor performance
    //hash the data

    std::stringstream hashed_data = hash_sha256(std::move(signature.str()));
    std::stringstream base64 = base64_Encode(hashed_data);
    std::string search = signature.str();
    
    //lambda for inserting data into a string. Used to update template envelope
    auto stringInsert = [](std::string& str, const std::string&& begFind, const std::string&& endFind, const std::string&& insert) mutable {
			  str.insert(str.find(endFind, str.find(begFind)) + endFind.size(), insert);
			};
    signature.str(std::string());
    stringInsert(search, "<DigestMethod", "/>", "<DigestValue>" + base64.str() + "</DigestValue>");

    stringInsert(search, "</SignedInfo", ">\n", "<SignatureValue></SignatureValue>\n");
    stringInsert(search, "<X509Data", ">", "<X509Certificate></X509Certificate>\n");
    signature << search;
    
    //sign document using private key.
    hashed_data = hash_sha256(std::move(signature.str()));

    std::string plaintext = hashed_data.str();
    unsigned char* encMessage;
    unsigned long encLength = plaintext.size();
    RSASign((const unsigned char*)plaintext.c_str(), plaintext.size(), &encMessage, &encLength);
    std::stringstream signatureValue;
    signatureValue << encMessage;
    base64 = base64_Encode(signatureValue);
    
    stringInsert(search, "<SignatureValue", ">", base64.str());

    //Insert the certificate into the document.
    stringInsert(search, "<X509Certificate", ">", "\n" + std::move(certificate));

    delete[] encMessage;

    std::ofstream ofile("signedUserData.xml");
    ofile << search;
    ofile.close();
    
    return true;
  }
};

std::string DocumentSigning::private_key = "";
std::string DocumentSigning::certificate = "";
Logging& DocumentSigning::log = Logging::createLog();
RSA* DocumentSigning::rsa = nullptr;
