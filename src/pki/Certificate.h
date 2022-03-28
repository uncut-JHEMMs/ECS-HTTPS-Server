#pragma once

#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "Certificate_Configuration.h"

class Certificate{

private:
  //certificate configurations
  std::string common_name = "";
  std::string country = "";
  std::string state = "";
  std::string city = "";
  std::string organization = "";
  std::string organizational_unit = "";
  std::string cert_name = "";
  unsigned int days = 60 * 60 * 24 * 365;
  unsigned int bits = 2048;
  unsigned int serial = 1;
  std::string certFname = "";
  std::string keyFname = "";
  
  EVP_PKEY* private_key = nullptr;
  X509* certificate = nullptr;
  bool is_signed = false;
    
  bool CreateX509Cert(){
    RSA* rsa = nullptr;
    X509_NAME* name = nullptr;
  
    //Generate storage for private key structure
    if(private_key == nullptr){
      if((private_key = EVP_PKEY_new()) == nullptr){
	return false;
      }
    }
  
    //Establish space for certificate
    if(certificate == nullptr){
      if((certificate = X509_new()) == nullptr){
	return false;
      }
    }

    name = X509_get_subject_name(certificate);

    //Generate an RSA key
    rsa = RSA_generate_key(bits, RSA_F4, nullptr, nullptr);

    if(!EVP_PKEY_assign_RSA(private_key, rsa))
      return false;
    
    //set certificate information
    if(country != "")
      X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char*)state.c_str(),-1, -1, 0);
    
    if(state != "")
      X509_NAME_add_entry_by_txt(name, "ST", MBSTRING_ASC, (unsigned char*)state.c_str(),-1, -1, 0);
    
    if(city != "")
      X509_NAME_add_entry_by_txt(name, "L", MBSTRING_ASC, (unsigned char*)city.c_str(),-1, -1, 0);
    
    if(organization != "")
      X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char*)organization.c_str(),-1, -1, 0);
    
    if(organizational_unit != "")
      X509_NAME_add_entry_by_txt(name, "OU", MBSTRING_ASC, (unsigned char*)organizational_unit.c_str(),-1, -1, 0);
    
    if(common_name != "")
      X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)common_name.c_str(),-1, -1, 0);

    //set certificate serial number
    ASN1_INTEGER_set(X509_get_serialNumber(certificate), serial);
    
    //set time duration for certificate
    X509_gmtime_adj(X509_get_notBefore(certificate), 0);
    X509_gmtime_adj(X509_get_notAfter(certificate), days);
    
    //create certificate public key from private key.
    X509_set_pubkey(certificate, private_key);
    
    return true;
  }
  
public:

  ~Certificate(){
    if(certificate != nullptr || private_key != nullptr){
      X509_free(certificate);
      EVP_PKEY_free(private_key);
    }
  }

  Certificate(Certificate_Configuration& config){
    common_name = config.common_name;
    state = config.state;
    country = config.country;
    city = config.city;
    organization = config.organization;
    organizational_unit = config.organizational_unit;
    cert_name = config.cert_name;
    days = config.days;
    serial = config.serial;
    bits = config.bits;
    keyFname = config.keyFname;
    certFname = config.certFname;
    
    if(!CreateX509Cert())
      std::cout << "ERROR: Couldn't create certificate.\n";
  }

  Certificate(Certificate_Configuration&& config){
    
    common_name = config.common_name;
    state = config.state;
    country = config.country;
    city = config.city;
    organization = config.organization;
    organizational_unit	= config.organizational_unit;
    cert_name =	config.cert_name;
    days = config.days;
    serial = config.serial;
    bits = config.bits;
    keyFname = config.keyFname;
    certFname =	config.certFname;
    
    config.common_name = "";
    config.state = "";
    config.country = "";
    config.city = "";
    config.organization = "";
    config.organizational_unit = "";
    config.cert_name = "";
    config.days = 0;
    config.serial = 0;
    config.bits = 0;
    config.keyFname = "";
    config.certFname = "";
    
    if(!CreateX509Cert())
      std::cout << "ERROR: Couldn't create certificate.\n";
  }

  Certificate() = delete;
  Certificate(Certificate&) = delete;
  Certificate(Certificate&& cert){
    common_name = cert.common_name;
    state = cert.state;
    country = cert.country;
    city = cert.city;
    organization = cert.organization;
    organizational_unit = cert.organizational_unit;
    cert_name = cert.cert_name;
    days = cert.days;
    serial = cert.serial;
    bits = cert.bits;
    private_key = cert.private_key;
    certificate = cert.certificate;
    keyFname = cert.keyFname;
    certFname =	cert.certFname;
    
    cert.common_name = "";
    cert.state = "";
    cert.country = "";
    cert.city = "";
    cert.organization = "";
    cert.organizational_unit = "";
    cert.cert_name = "";
    cert.days = 0;
    cert.serial = 0;
    cert.bits = 0;
    cert.private_key = nullptr;
    cert.certificate = nullptr;
    cert.keyFname = "";
    cert.certFname = "";
    
    if(!CreateX509Cert())
      std::cout << "ERROR: Couldn't create certificate.\n";
  }

  Certificate& operator=(Certificate&) = delete;
  Certificate& operator=(Certificate&& cert){

    if(this == &cert)
      return *this;

    common_name = cert.common_name;
    state = cert.state;
    country = cert.country;
    city = cert.city;
    organization = cert.organization;
    organizational_unit = cert.organizational_unit;
    cert_name = cert.cert_name;
    days = cert.days;
    serial = cert.serial;
    bits = cert.bits;
    private_key = cert.private_key;
    certificate = cert.certificate;
    keyFname = cert.keyFname;
    certFname =	cert.certFname;
    
    cert.common_name = "";
    cert.state = "";
    cert.country = "";
    cert.city = "";
    cert.organization = "";
    cert.organizational_unit = "";
    cert.cert_name = "";
    cert.days = 0;
    cert.serial = 0;
    cert.bits = 0;
    cert.private_key = nullptr;
    cert.certificate = nullptr;
    cert.keyFname = "";
    cert.certFname = "";
    
    return *this;
  }

  bool sign(const Certificate& signing_cert){

    if(is_signed)
      return true;
    
    is_signed = true;
    X509_NAME* name = nullptr;
    
    //check if the certificate is self signed
    name = X509_get_subject_name(signing_cert.certificate);
    X509_set_issuer_name(certificate, name);
    
    //sign the certificate.
    if(!X509_sign(certificate, signing_cert.private_key, EVP_sha1()))
      return false;
    
    return true;
  }

  bool writeToFiles(){

    FILE* file;
    file = fopen(keyFname.c_str(), "wb");
    
    //write private key to file
    PEM_write_PrivateKey(file, private_key, NULL, NULL, 5, NULL, NULL);
    
    fclose(file);

    file = fopen(certFname.c_str(), "wb");
    
    //Write certificate to file
    PEM_write_X509(file, certificate);
    
    fclose(file);
    
    return true;
  }
};
